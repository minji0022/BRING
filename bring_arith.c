#include "bring_arith.h"

int BI_ExpMod_zx(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2, BIGINT* bi_M){
    if (bi_src1 == NULL || bi_src2 == NULL || bi_M == NULL) {
        printf("[WARNING] : x 또는 n 또는 M의 값이 존재하지 않음\n");
        return NULL_POINTER_ERROR;
    } 
    if (bi_is_zero(bi_src2)) {
        bi_set_one(bi_dst); /* 1 <- X^0 */ /* 0^0도 1로 정의 함 */
        return FUNC_SUCCESS;
    }
    if (bi_is_one(bi_src1)) {
        bi_set_one(bi_dst); /* 1 <- 1^n */
        return FUNC_SUCCESS;
    } 
    if (bi_is_zero(bi_src1)) {
        bi_set_zero(bi_dst); /* 0 <- 0^n (n != 0) */
        return FUNC_SUCCESS;
    }
    bi_Exp_L2R_zx(bi_dst, bi_src1, bi_src2, bi_M); /* 이 외 나머지 */
    // 부호 정보 할당
    if (bi_src1->sign == NEGATIVE && (bi_M->p[0] & 0x1)) { // 음수의 홀수 제곱인 경우만 결과가 음수.
        (*bi_dst)->sign = NEGATIVE;
    }  
    return FUNC_SUCCESS;
}



//  dst = src << r
// 갱신형은 나중에 고려해보는 걸로...
void bi_left_bit_shift_zx(BIGINT** bi_dst, BIGINT* bi_src, int r) { /**** B = A << r bits ****/
    int n = bi_src->wordlen;
    int word_shift = r / WORD_BIT_SIZE; // word_shift == k

    bi_new(bi_dst, n + word_shift + 1); // 부호 : NON_NEGATIVE로 설정됨
    
    // Case 1: r = wk
    if(r % WORD_BIT_SIZE == 0) {
        for(int i = word_shift; i < (*bi_dst)->wordlen - 1; i++){
            (*bi_dst)->p[i] = (word)(bi_src->p[i-word_shift]);
        }
    } // Case 2: r = wk + r'
    else {
        int bit_shift = r % WORD_BIT_SIZE;

        // Case 2-1: j = 0
        (*bi_dst)->p[word_shift] = (word)(bi_src->p[0] << bit_shift);

        // Case 2-2: j = 1, ..., n - 1
        for(int j = 1; j < n; j++) {
            (*bi_dst)->p[j+word_shift] = (word)(((bi_src->p[j]) << bit_shift) | ((bi_src->p[j-1]) >> (WORD_BIT_SIZE - bit_shift)));
        }

        // Case 2-3: j = n
        (*bi_dst)->p[((*bi_dst)->wordlen)-1] = (word)(bi_src->p[n-1] >> (WORD_BIT_SIZE - bit_shift));
    }
    bi_refine(*bi_dst);
} 

void bi_right_bit_shift_zx(BIGINT** bi_dst, BIGINT* bi_src, int r){ /**** B = A >> r bits ****/
    int SLen = bi_src->wordlen;
    int word_shift = r / WORD_BIT_SIZE; // r 비트를 word(32비트)로 나눈 횟수
    int bit_shift = r % WORD_BIT_SIZE; // 나머지 비트 이동

    if (r >= SLen * WORD_BIT_SIZE) {  /* case 1) r >= wn */
        bi_set_zero(bi_dst); // bi_dst = 0이 됨.
        return;
    }    

    bi_new(bi_dst, SLen - word_shift); // 부호 : NON_NEGATIVE로 설정됨

    if (bit_shift == 0) {          /* case 2) r = wk (k<n) */
        for (int i = 0; i < (*bi_dst)->wordlen; i++){
            (*bi_dst)->p[i] = bi_src->p[word_shift + i];
        }
    }
    else {                              /* case 3) r = wk + r' */
        for (int i = 0; i < (*bi_dst)->wordlen - 1; i++){
            word shitfed_word = (bi_src->p[word_shift+1+i] << (WORD_BIT_SIZE - bit_shift)) 
                | (bi_src->p[word_shift+i] >> bit_shift);
            (*bi_dst)->p[i] = shitfed_word;
        }
        (*bi_dst)->p[(*bi_dst)->wordlen - 1] = bi_src->p[SLen-1] >> bit_shift;
    }
    bi_refine(*bi_dst);    
} 

/* Shift Right-to-Left R-words */
// 최대 범위 넘어가는거 에러뜨도록 해야하나?
/* wordlen이 0이 될 경우, 데이터 변화가 없음. 0 판별할 때 부호정보 대신 길이 정보만 사용해야 함.*/
/* 남는 데이터는 어떻게 처리하나? 그냥 냅두나? */
void bi_left_word_shift(BIGINT* bi_src, int r){  /**** A << r words ****/
    int last_wordlen = bi_src->wordlen;
    int i;
    bi_src->wordlen = bi_src->wordlen + r;
    bi_src->p = (word*)realloc(bi_src->p, sizeof(word)*bi_src->wordlen);

    for (i = last_wordlen - 1; i >= 0; i--){    /* a[wordlen + r - 1] ... a[r] 채우기 */
        bi_src->p[i + r] = bi_src->p[i];
    }
    for (i = 0; i < r; i++){    /* a[r - 1] ... a[0] 채우기 */
        bi_src->p[i] = 0;
    }
}

/* Shift Left-to-Right R-words */
void bi_right_word_shift(BIGINT* bi_src, int r){ /**** A >> r words ****/
    for (int i = 0; i < bi_src->wordlen; i++){
        bi_src->p[i] = bi_src->p[r + i];
    }

    bi_src->wordlen = bi_src->wordlen - r;
    bi_src->p = (word*)realloc(bi_src->p, sizeof(word)*bi_src->wordlen);
}

//################################################################################################# 
//                                           지수승 관련 함수 
//#################################################################################################
void bi_Exp_L2R_zx(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2, BIGINT* bi_M){

    bi_new(bi_dst, 1);
    (*bi_dst)->p[0] = 1;

    int length = bi_get_bit_length(bi_src2);
    int word_cnt = bi_src2->wordlen - 1;
    int bit_cnt = WORD_BIT_SIZE-1;
    while (word_cnt >= 0){
        BIGINT *temp1 = NULL;
        BIGINT *temp2 = NULL;

        //line 3
        bi_Sqrc_zy(&temp1, *bi_dst); // temp1 = t^2
        BI_Mod_zxy(&temp2, temp1, bi_M); // temp2 = t^2 mod M
        bi_delete(&temp1); // temp1 비워주기.

        //line 4
        if((bi_src2->p[word_cnt] >> bit_cnt) & 0x1 == 1){
            bi_Mul_Schoolbook_zxy(&temp1, temp2, bi_src1); // 이 부분을 코어로 할지, 사용자 함수로 할지
            BI_Mod_zxy(bi_dst, temp1, bi_M);
            //bi_assign(bi_dst, temp1);
        }
        else bi_assign(bi_dst, temp2);
        bit_cnt--;

        if(bit_cnt < 0) {
            word_cnt--;
            bit_cnt = WORD_BIT_SIZE-1;
        }

        bi_delete(&temp1);
        bi_delete(&temp2);

    }
}