#include "bring_arith.h"

//===============================================================================================//
//                                       사용자용 함수 구역
//                                     사용자가 직접 사용할 함수
//===============================================================================================//

/**
* @brief [모듈러 지수승 함수] src1 ^ src2 mod M
* @param[out] bi_dst 모듈러 지수승 결과 (= src1 ^ src2 mod M)
* @param[in] bi_src1 입력 src1
* @param[in] bi_src2 입력 src2
* @param[in] bi_M 입력 M
* @return Success or Error Code
*/
int BI_ExpMod_zx(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2, BIGINT* bi_M){
    // Error: 입력이 유효하지 않은 경우 - NULL_POINTER_ERROR
    if (bi_src1 == NULL || bi_src2 == NULL || bi_M == NULL) {
        printf("[WARNING] : x 또는 n 또는 M의 값이 존재하지 않음\n");
        return NULL_POINTER_ERROR;
    } 

    // Error: 입력이 유효하지 않은 경우 - NON_NEGATIVE 정수만 입력 가능 - ERR_INVALID_INPUT
    if (bi_src1->sign == NEGATIVE || bi_src2->sign == NEGATIVE || bi_M->sign == NEGATIVE) {
        printf("[WARNING] : 모듈러 지수승 값은 음이 아닌 정수만 입력 가능\n");
        return ERR_INVALID_INPUT;
    }

    // case 1: bi_src2가 0인 경우
    if (bi_is_zero(bi_src2)) {
        // 1 <- X^0
        bi_set_one(bi_dst); // 0^0도 1로 정의 함
        return FUNC_SUCCESS;
    }

    // case 2: bi_src1가 1인 경우
    if (bi_is_one(bi_src1)) {
        // 1 <- 1^n
        bi_set_one(bi_dst); 
        return FUNC_SUCCESS;
    } 

    // case 3: bi_src1가 0인 경우
    if (bi_is_zero(bi_src1)) {
        // 0 <- 0^n (n != 0)
        bi_set_zero(bi_dst); 
        return FUNC_SUCCESS;
    }

    // case 4: 이외의 나머지 경우 (4.1 또는 4.2 중 하나 선택)
    // 4-1. Left-to-Right Exp Mod : 부채널 공격에 취약
    bi_Exp_L2R_zx(bi_dst, bi_src1, bi_src2, bi_M);
    
    // 4-2. Multiply-and-Squaring Exp Mod : 부채널 공격에 내성을 가지지만 속도가 4-1과 비교하여 느림
    //bi_Exp_MnS_zx(bi_dst, bi_src1, bi_src2, bi_M);

    return FUNC_SUCCESS;
}

/**
* @brief [Fast reduction function] src mod N 모듈러 연산
* @param[out] bi_dst 모듈러 결과 (= src mod N)
* @param[in] bi_src 입력 src
* @param[in] bi_N 입력 N 
* @param[in] bi_T 입력 사전 계산 값 T
* @return Success or Error Code
* @note 함수 사용 전 사전 계산 값 bi_T를 계산하는 함수 bi_BR_pre_computed를 실행해야 함.
* 1) bi_BR_pre_computed
* 2) BI_Barret_Reduction
* @note bi_src->wordlen <= (bi_N->wordlen * 2)를 만족해야 사용 가능
*/
int BI_Barret_Reduction(BIGINT** bi_dst, BIGINT* bi_src, BIGINT* bi_N, BIGINT* bi_T){
    // Error: 입력이 유효하지 않은 경우 - NULL_POINTER_ERROR
    if (bi_src == NULL || bi_N == NULL || bi_T == NULL) {
        printf("[WARNING] : bi_src 또는 bi_N 또는 bi_T의 값이 존재하지 않음\n");
        return NULL_POINTER_ERROR;
    }

    // Error: 입력이 유효하지 않은 경우 - NON_NEGATIVE 정수만 입력 가능 - ERR_INVALID_INPUT
    if (bi_src->sign == NEGATIVE || bi_N->sign == NEGATIVE || bi_T->sign == NEGATIVE) {
        printf("[WARNING] : Barret Reduction 값은 음이 아닌 정수만 입력 가능\n");
        return ERR_INVALID_INPUT;
    }    

    int n = bi_N->wordlen; 

    // Error: 입력이 유효하지 않은 경우 - bi_src->wordlen <= (bi_N->wordlen * 2)를 만족해야 사용 가능 - SIZE_ERROR
    if (bi_src->wordlen > (bi_N->wordlen << 1)) {
        printf("[WARNING] : bi_src의 워드 길이는 bi_N 워드 길이의 2배 이하이어야 합니다.\n");
        printf("현재 bi_N의 워드 길이 : %d, bi_src의 워드 길이 : %d\n", n, bi_src->wordlen);
        return SIZE_ERROR;
    }

    BIGINT* Q_hat1 = NULL;
    BIGINT* Q_hat2 = NULL;
    BIGINT* temp_R = NULL;

    bi_right_bit_shift_zx(&Q_hat1, bi_src, (n-1) << SHIFT_SIZE); // Q_hat1 <-  A >> w(n-1)
    BI_Mul_xy(&Q_hat1, bi_T); // Q_hat1 <- Q_hat * T
    bi_right_bit_shift_zx(&Q_hat2, Q_hat1, (n+1) << SHIFT_SIZE); // Q_hat2 <- Q_hat1 >> w(n-1)
    BI_Mul_zxy(&temp_R, bi_N, Q_hat2); // temp_R <- N * Q_hat2
    BI_Sub_zxy(bi_dst, bi_src, temp_R); // R <- A - R;

    bi_delete(&Q_hat1);
    bi_delete(&Q_hat2);
    bi_delete(&temp_R);

    // R < N: 나머지는 나누는 수보다 작아야 한다.
    while (bi_compare_bigint(*bi_dst, bi_N) >= 0) {
        // R <- R - N
        BI_Sub_xy(bi_dst, bi_N); 
    }

    return FUNC_SUCCESS;
}

/**
 * @brief Precomputation Function for Barrett reduction
 * bi_T = WORD^(2 * bi_N->wordlen) / bi_N
*/
void bi_BR_pre_computed(BIGINT** bi_T, BIGINT* bi_N){
    BIGINT* bi_W_2n = NULL;
    BIGINT* temp = NULL;

    bi_set_one(&bi_W_2n);
    
    bi_left_word_shift(bi_W_2n, 2*bi_N->wordlen);

    BI_Div_zxy(bi_T, &temp, bi_W_2n, bi_N);
    
    // variable delete..
    bi_delete(&bi_W_2n);
    bi_delete(&temp);    
}


//===============================================================================================//

//===============================================================================================//
//                                       개발자용 함수 구역
//                                  사용자는 직접 사용하지 않는 함수
//===============================================================================================//

/**
 * @brief 왼쪽 비트 시프트 연산
 *  dst = src << r
 * @note 왼쪽 워드 시프트 연산 따로 존재
*/
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

/**
 * @brief 오른쪽 비트 시프트 연산
 *  dst = src >> r
 * @note 오른쪽 워드 시프트 연산 따로 존재
*/
void bi_right_bit_shift_zx(BIGINT** bi_dst, BIGINT* bi_src, int r){ 
    int SLen = bi_src->wordlen;
    int word_shift = r / WORD_BIT_SIZE; // r 비트를 word(32비트)로 나눈 횟수
    int bit_shift = r % WORD_BIT_SIZE; // 나머지 비트 이동

    // case 1) r >= wn
    if (r >= SLen * WORD_BIT_SIZE) {
        bi_set_zero(bi_dst); // bi_dst = 0이 됨.
        return;
    }    

    bi_new(bi_dst, SLen - word_shift); // 부호 : NON_NEGATIVE로 설정됨

    // case 2) r = wk (k<n)
    if (bit_shift == 0) {  
        for (int i = 0; i < (*bi_dst)->wordlen; i++){
            (*bi_dst)->p[i] = bi_src->p[word_shift + i];
        }
    } // case 3) r = wk + r'
    else {
        for (int i = 0; i < (*bi_dst)->wordlen - 1; i++){
            word shitfed_word = (bi_src->p[word_shift+1+i] << (WORD_BIT_SIZE - bit_shift)) 
                | (bi_src->p[word_shift+i] >> bit_shift);
            (*bi_dst)->p[i] = shitfed_word;
        }
        (*bi_dst)->p[(*bi_dst)->wordlen - 1] = bi_src->p[SLen-1] >> bit_shift;
    }
    bi_refine(*bi_dst);    
} 

/**
 * @brief [갱신 함수] 왼쪽 워드 시프트 연산
 *  src = src << (r * WORD_BIT_SIZE)
 * @note 왼쪽 비트 시프트 연산 따로 존재
*/
void bi_left_word_shift(BIGINT* bi_src, int r){ 
    int last_wordlen = bi_src->wordlen;
    int i;
    bi_src->wordlen = bi_src->wordlen + r;
    bi_src->p = (word*)realloc(bi_src->p, sizeof(word)*bi_src->wordlen);

    /* a[wordlen + r - 1] ... a[r] 채우기 */
    for (i = last_wordlen - 1; i >= 0; i--){    
        bi_src->p[i + r] = bi_src->p[i];
    }

    /* a[r - 1] ... a[0] 채우기 */
    for (i = 0; i < r; i++){    
        bi_src->p[i] = 0;
    }
}

/**
 * @brief 왼쪽 워드 시프트 연산
 *  dst = src << (r * WORD_BIT_SIZE)
 * @note 왼쪽 비트 시프트 연산 따로 존재
*/
void bi_left_word_shift_zx(BIGINT** bi_dst, BIGINT* bi_src, int r){
    int s_wordlen = bi_src->wordlen;
    int i;
    int d_wordlen = bi_src->wordlen + r;

    bi_new(bi_dst, d_wordlen);

    /* a[wordlen + r - 1] ... a[r] 채우기 */
    for (i = s_wordlen - 1; i >= 0; i--){    
        (*bi_dst)->p[i + r] = bi_src->p[i];
    }

    /* a[r - 1] ... a[0] 채우기 */
    for (i = 0; i < r; i++){    
        (*bi_dst)->p[i] = 0;
    }
}

/**
 * @brief [갱신 함수] 오른쪽 워드 시프트 연산
 *  src = src >> (r * WORD_BIT_SIZE)
 * @note 오른쪽 비트 시프트 연산 따로 존재
*/
void bi_right_word_shift(BIGINT* bi_src, int r){ 
    /* A >> r words */
    for (int i = 0; i < bi_src->wordlen-r; i++){
        bi_src->p[i] = bi_src->p[r + i];
    }

    bi_src->wordlen = bi_src->wordlen - r;
    bi_src->p = (word*)realloc(bi_src->p, sizeof(word)*bi_src->wordlen);
}

/**
 * @brief 오른쪽 워드 시프트 연산
 *  dst = src >> (r * WORD_BIT_SIZE)
 * @note 오른쪽 비트 시프트 연산 따로 존재
*/
void bi_right_word_shift_zx(BIGINT** bi_dst, BIGINT* bi_src, int r){
    int i = 0;
    bi_new(bi_dst, bi_src->wordlen - r);

    for (; r < (*bi_dst)->wordlen; r++){
        (*bi_dst)->p[i] = bi_src->p[r];
        i++;
    }
}

//################################################################################################# 
//                                           지수승 관련 함수 
//#################################################################################################

/**
 * @brief [모듈러 지수승 함수] Left-to-Right Algorithm.
 * dst = src1 ^ src2 mod M
 * @note 부채널 공격에 취약 (실행 시간이 Constant가 아님)
*/
void bi_Exp_L2R_zx(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2, BIGINT* bi_M){
    // word index
    int word_cnt = bi_src2->wordlen - 1; 
    int bit_cnt = WORD_BIT_SIZE-1; // 하나의 워드 내에서 비트 시프트할 횟수

    bi_new(bi_dst, 1); 

    // init dst = 1
    (*bi_dst)->p[0] = 1;

    // 모든 워드에서 진행
    while (word_cnt >= 0){ 
        BIGINT *temp1 = NULL; 
        BIGINT *temp2 = NULL; 
        
        // temp1 <- t^2
        bi_Sqrc_zy(&temp1, *bi_dst); 

        // temp2 <- t^2 mod M
        BI_Mod_zxy(&temp2, temp1, bi_M); 

        // temp1 이후에 다시 사용하기 위해 비워주기.
        bi_delete(&temp1); 
        
        // case 1: n의 자릿수 비트가 1인 경우
        if(((bi_src2->p[word_cnt] >> bit_cnt) & 0x1) == 1){ 
            bi_Mul_Karatsuba(&temp1, temp2, bi_src1, KARA_FLAG);  // temp1 <- t*x
            BI_Mod_zxy(bi_dst, temp1, bi_M); // bi_dst <- t*x mod M
        } // case 2: n의 자릿수 비트가 0인 경우
        else {
            bi_assign(bi_dst, temp2); // bi_dst <- 계산해둔 t^2 mod M 복사
        }
        
        bit_cnt--; // 하나의 워드 내에서 비트 시프트할 횟수 -1하는 작업.
        
        // 하나의 워드 내에서 비트 시프트를 다 했으면 워드 인데스 1 감소시킨 후 비트 시프트할 횟수 다시 초기화
        if(bit_cnt < 0) { 
            word_cnt--; 
            bit_cnt = WORD_BIT_SIZE-1; 
        }

        // temp variable delete..
        bi_delete(&temp1);
        bi_delete(&temp2);
    }
}

/**
 * @brief [모듈러 지수승 함수] Muliply-and-Squaring Algorithm.
 * dst = src1 ^ src2 mod M
 * @note 부채널 공격에 취약하지 않지만, bi_Exp_L2R_zx과 비교하여 속도가 느림.
*/
void bi_Exp_MnS_zx(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2, BIGINT* bi_M){
    // word index
    int word_cnt = bi_src2->wordlen - 1;
    int bit_cnt = WORD_BIT_SIZE-1; // 하나의 워드 내에서 비트 시프트할 횟수
    BIGINT *tdst = NULL;

    bi_new(bi_dst, 1);

    // bi_dst를 1로 초기화
    (*bi_dst)->p[0] = 1;

    // x^n mod M 에서 n 값을 t1에 복사
    bi_assign(&tdst, bi_src1); 

    // 현재 조건을 만족하면, Fast reduction Algorithm인 Barrett Reduction 사용.
    BIGINT* T = NULL;
    // Barrett 사전 계산
    bi_BR_pre_computed(&T, bi_M);

    // 모든 워드에서 진행
    while (word_cnt >= 0){ 
        BIGINT *temp0 = NULL; 
        BIGINT *temp1 = NULL; 
        
        // case 1: n의 자릿수 비트가 1인 경우
        if(((bi_src2->p[word_cnt] >> bit_cnt) & 0x1) == 1){ 
            bi_Mul_Karatsuba(&temp0, *bi_dst, tdst, KARA_FLAG); // temp0 <- t0*t1
            
            // 현재 조건을 만족하면, Fast reduction Algorithm인 Barrett Reduction 사용.
            // bi_dst <- t0 * t1 mod M
            if(temp0->wordlen <= (2 * bi_M->wordlen)) {
                BI_Barret_Reduction(bi_dst, temp0, bi_M, T);
            }
            else {
                BI_Mod_zxy(bi_dst, temp0, bi_M); 
            }

            bi_Sqrc_zy(&temp1, tdst); // temp1 = t1^2

            // 현재 조건을 만족하면, Fast reduction Algorithm인 Barrett Reduction 사용.
            // tdst = t1^2 mod M
            if(temp1->wordlen <= (2 * bi_M->wordlen)) {
                BI_Barret_Reduction(&tdst, temp1, bi_M, T);
            }
            else {
                BI_Mod_zxy(&tdst, temp1, bi_M); 
            }
        } // case 2: n의 자릿수 비트가 0인 경우
        else {
            bi_Mul_Karatsuba(&temp1, *bi_dst, tdst, KARA_FLAG); // temp1 <- t0*t1
            
            // 현재 조건을 만족하면, Fast reduction Algorithm인 Barrett Reduction 사용.
            // tdst = t1^2 mod M
            if(temp1->wordlen <= (2 * bi_M->wordlen)) {
                BI_Barret_Reduction(&tdst, temp1, bi_M, T);
            }
            else {
                BI_Mod_zxy(&tdst, temp1, bi_M);
            }

            bi_Sqrc_zy(&temp0, *bi_dst); // temp0 = t0^2
            
            // 현재 조건을 만족하면, Fast reduction Algorithm인 Barrett Reduction 사용.
            // bi_dst <- t0 * t1 mod M
            if(temp0->wordlen <= (2 * bi_M->wordlen)) {
                BI_Barret_Reduction(bi_dst, temp0, bi_M, T);
            }
            else {
                BI_Mod_zxy(bi_dst, temp0, bi_M); 
            }
        }
        
        bit_cnt--; 

        // 하나의 워드 내에서 비트 시프트를 다 했으면 워드 인데스 1 감소시킨 후 비트 시프트할 횟수 다시 초기화
        if(bit_cnt < 0) { 
            word_cnt--; 
            bit_cnt = WORD_BIT_SIZE-1; 
        }

        // temp variable delete..
        bi_delete(&temp0);
        bi_delete(&temp1);
    }

    // variable delete..
    bi_delete(&T);
    bi_delete(&tdst);
}
