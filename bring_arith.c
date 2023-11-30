#include "bring_arith.h"

//  dst = src << r
// 갱신형은 나중에 고려해보는 걸로...
void bi_left_bit_shift_zx(BIGINT** bi_dst, BIGINT* bi_src, int r) { /**** B = A << r bits ****/
    int n = bi_src->wordlen;
    int word_shift = r / WORD_BIT_SIZE; // word_shift == k

    bi_new(bi_dst, n + word_shift + 1); // 부호 : NON_NEGATIVE로 설정됨
    
    // Case 1: r = wk
    if(r % WORD_BIT_SIZE == 0) {
        for(int i = word_shift; i < (*bi_dst)->wordlen; i++){
            (*bi_dst)->p[i] = bi_src->p[i-word_shift];
        }
    } // Case 2: r = wk + r'
    else {
        int bit_shift = r % WORD_BIT_SIZE;

        // Case 2-1: j = 0
        (*bi_dst)->p[word_shift] = bi_src->p[0] << bit_shift;

        // Case 2-2: j = 1, ..., n - 1
        for(int j = 1; j < n; j++) {
            (*bi_dst)->p[j+word_shift] = ((bi_src->p[j]) << bit_shift) | ((bi_src->p[j-1]) >> (WORD_BIT_SIZE - bit_shift));
        }

        // Case 2-3: j = n
        (*bi_dst)->p[((*bi_dst)->wordlen)-1] = bi_src->p[n-1] >> (WORD_BIT_SIZE - bit_shift);
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