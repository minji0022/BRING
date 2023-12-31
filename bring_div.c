#include "bring_arith.h"

//===============================================================================================//
//                                       사용자용 함수 구역
//                                     사용자가 직접 사용할 함수
//===============================================================================================//

/** 
 * @brief BI_Div_zxy 함수는 두 큰 정수를 나누는 연산을 수행
 * @details 이 함수는 두 큰 정수를 나누어 몫과 나머지를 계산
 * @param[out] bi_quotient - 나누기의 결과로 얻어진 몫이 저장될 BIGINT 포인터의 포인터
 * @param[out] bi_remainder - 나누기의 결과로 얻어진 나머지가 저장될 BIGINT 포인터의 포인터
 * @param[in] bi_src1 - 나누어질 큰 정수인 BIGINT 구조체
 * @param[in] bi_src2 - 나누는 큰 정수인 BIGINT 구조체
 * @return 성공 시 FUNC_SUCCESS를 반환하고, 오류가 발생한 경우 오류 코드를 반환
 */
int BI_Div_zxy(BIGINT** bi_quotient, BIGINT** bi_remainder, BIGINT* bi_src1, BIGINT* bi_src2){
    // Error: 입력이 유효하지 않은 경우 - NULL_POINTER_ERROR
    if (bi_src1 == NULL || bi_src2 == NULL) {
        printf("[WARNING] : A 또는 B의 값이 존재하지 않음\n");
        return NULL_POINTER_ERROR;
    }   

    // Error: 입력이 유효하지 않은 경우 - NON_NEGATIVE 정수만 입력 가능 - ERR_INVALID_INPUT
    if (bi_src1->sign || bi_src2->sign) {
        printf("[WARNING] : 계산 대상으로 음이 아닌 정수만 입력 가능합니다\n");
        return ERR_INVALID_INPUT;
    }       

    // Case 1: A < B
    if (bi_compare_bigint(bi_src1, bi_src2) == -1) {
        bi_set_zero(bi_quotient); // 몫 : 0
        bi_assign(bi_remainder, bi_src1); // 나머지 : A
        return FUNC_SUCCESS;
    }

    int n = bi_src1->wordlen;

    // remainder set zero
    bi_set_zero(bi_remainder);

    bi_new(bi_quotient, n);

    for (int i = n-1; i >= 0; i--) {
        BIGINT* tQ = NULL;
        BIGINT* tR = NULL;
        BIGINT* tAi = NULL;

        bi_new(&tAi, 1);

        tAi->p[0] = bi_src1->p[i];

        // r = (2 * r) + ai
        bi_left_word_shift(*bi_remainder, 1);
        BI_Add_zxy(&tR, *bi_remainder, tAi);

        // Div Core
        bi_Divc_zxy(&tQ, bi_remainder, tR, bi_src2);

        (*bi_quotient)->p[i] = tQ->p[0];
        
        // temp variable delete..
        bi_delete(&tQ);
        bi_delete(&tR);
        bi_delete(&tAi);
    }

    bi_refine(*bi_quotient);
    return FUNC_SUCCESS;
}

/** 
 * @brief [모듈러 함수] BI_Mod_zxy 함수는 큰 정수의 모듈러 연산을 수행
 * @details 이 함수는 큰 정수를 다른 큰 정수로 나눈 나머지를 계산
 * @param[out] bi_dst - 모듈러 연산의 결과로 얻어진 나머지가 저장될 BIGINT 포인터의 포인터
 * @param[in] bi_src1 - 나누어질 큰 정수인 BIGINT 구조체
 * @param[in] bi_src2 - 나누는 큰 정수인 BIGINT 구조체
 * @return 성공 시 FUNC_SUCCESS를 반환하고, 오류가 발생한 경우 오류 코드를 반환
 */
int BI_Mod_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2){
    BIGINT* tQ = NULL;
    
    // Div 함수에서 나머지만 취함
    BI_Div_zxy(&tQ, bi_dst, bi_src1, bi_src2);

    bi_delete(&tQ);
    return FUNC_SUCCESS;
}

/** 
 * @brief [모듈러 갱신 함수] BI_Mod_zxy 함수는 큰 정수의 모듈러 연산을 수행
 * @details 이 함수는 큰 정수를 다른 큰 정수로 나눈 나머지를 계산
 * @param[in] bi_src1 - 나누어질 큰 정수인 BIGINT 구조체
 * @param[in] bi_src2 - 나누는 큰 정수인 BIGINT 구조체
 * @return 성공 시 FUNC_SUCCESS를 반환하고, 오류가 발생한 경우 오류 코드를 반환
 */
int BI_Mod_xy(BIGINT** bi_src1, BIGINT* bi_src2){
    BIGINT* tmp = NULL;
    
    // 모듈러 함수 실행
    int zxy_flag = BI_Mod_zxy(&tmp, *bi_src1, bi_src2);    
    bi_assign(bi_src1, tmp);

    bi_delete(&tmp);

    // return value check
    if(zxy_flag != FUNC_SUCCESS) {
        return zxy_flag;
    }

    return FUNC_SUCCESS;
}
//===============================================================================================//

//===============================================================================================//
//                                       개발자용 함수 구역
//                                  사용자는 직접 사용하지 않는 함수
//===============================================================================================//

/**
 * @brief bi_Divc_zxy 함수는 큰 정수의 모듈러 연산을 수행
 * @details 이 함수는 큰 정수를 다른 큰 정수로 나눈 몫과 나머지를 계산
 * @param[out] bi_quotient - 나누기의 결과로 얻어진 몫이 저장될 BIGINT 포인터의 포인터
 * @param[out] bi_remainder - 나누기의 결과로 얻어진 나머지가 저장될 BIGINT 포인터의 포인터
 * @param[in] bi_src1 - 나누어질 큰 정수인 BIGINT 구조체. (bi_src1)의 길이는 m+1
 * @param[in] bi_src2 - 나누는 큰 정수인 BIGINT 구조체. (bi_src2)의 길이는 m 이 되게끔 넣어줌.
 */
void bi_Divc_zxy(BIGINT** bi_quotient, BIGINT** bi_remainder, BIGINT* bi_src1, BIGINT* bi_src2){
    bi_new(bi_quotient, 1);

    int m = bi_src2->wordlen;
    int k = 0;  
    BIGINT* tA = NULL;
    BIGINT* tB = NULL;
    BIGINT* tQ = NULL;
    BIGINT* tR = NULL;
    
    // case 1: A < B인 경우
    if (bi_compare_bigint(bi_src1, bi_src2) == -1) {
        bi_set_zero(bi_quotient); // 몫 : 0
        bi_assign(bi_remainder, bi_src1); // 나머지 : A
        return ;
    }

    // Compute k \in Z >=0 such that (2^k) * B_{m−1} \in [2^{w−1} , 2^w)
    while (bi_src2->p[m-1]<<k < WORD_SIZE_CHECK) {
        k++;
    }
    
    // A', B' <- 2^k * A, 2^k * B
    bi_left_bit_shift_zx(&tA, bi_src1, k); 
    bi_left_bit_shift_zx(&tB, bi_src2, k); 

    // DivCC
    bi_Divcc_zxy(&tQ, &tR, tA, tB);

    // Q, R 갱신
    bi_assign(bi_quotient, tQ);
    bi_right_bit_shift_zx(bi_remainder, tR, k);

    // temp variable delete..
    bi_delete(&tA);
    bi_delete(&tB);
    bi_delete(&tQ);
    bi_delete(&tR);
}

/**
 * @brief bi_Divcc_zxy 함수는 큰 정수의 몫과 나머지를 계산
 * @details 이 함수는 큰 정수를 다른 큰 정수로 나눈 몫과 나머지를 계산
 * @param[out] bi_quotient - 나누기의 결과로 얻어진 몫이 저장될 BIGINT 포인터의 포인터
 * @param[out] bi_remainder - 나누기의 결과로 얻어진 나머지가 저장될 BIGINT 포인터의 포인터
 * @param[in] bi_src1 - 나누어질 큰 정수인 BIGINT 구조체
 * @param[in] bi_src2 - 나누는 큰 정수인 BIGINT 구조체
 */
void bi_Divcc_zxy(BIGINT** bi_quotient, BIGINT** bi_remainder, BIGINT* bi_src1, BIGINT* bi_src2){
    int n = bi_src1->wordlen; // A의 길이
    int m = bi_src2->wordlen; // B의 길이
    BIGINT* BQ = NULL;
    BIGINT* tR = NULL;
    bi_new(bi_quotient, 1);

    // 길이가 같은 경우, if n = m
    if (n == m) {
        (*bi_quotient)->p[0] = bi_src1->p[m-1] / bi_src2->p[m-1];
    }
    
    // if n = m+1
    else if (n == m+1) {
        if (bi_src1->p[m] == bi_src2->p[m-1]){
            (*bi_quotient)->p[0] = MAX_OF_WORD;
        }
        else { // longdiv 써야 함.
            (*bi_quotient)->p[0] = bi_longdiv(bi_src1->p[m], bi_src1->p[m-1], bi_src2->p[m-1]);
        }
    }
    
    BI_Mul_zxy(&BQ, bi_src2, *bi_quotient); // BQ 구하기

    BI_Sub_zxy(bi_remainder, bi_src1, BQ); // R <- A - BQ
    
    while ((*bi_remainder)->sign == NEGATIVE) {
        (*bi_quotient)->p[0] = (*bi_quotient)->p[0] - 1;
        BI_Add_zxy(&tR, *bi_remainder, bi_src2);
        bi_assign(bi_remainder, tR);
    }
    // temp variable delete..
    bi_delete(&BQ); 
    bi_delete(&tR);    
}

/**
 * @brief bi_longdiv 함수는 두 워드의 나눗셈을 수행
 * @details 이 함수는 두 워드의 나눗셈 연산을 수행하여 몫을 반환
 * @param[in] a1 - 높은 자리 워드
 * @param[in] a0 - 낮은 자리 워드
 * @param[in] b - 나눌 수
 * @return 나눗셈의 몫을 반환
 */
word bi_longdiv(word a1, word a0, word b) {
    word x = WORD_SIZE_CHECK;
    word q = 0;
    word r = a1;

    for (int i = WORD_BIT_SIZE-1; i >= 0; i--) {
        if (r >= WORD_SIZE_CHECK) {
            // 몫, 나머지 계산
            q = q + (x >> (WORD_BIT_SIZE-1-i));
            r = (r << 1) + ((a0 >> i) & 0x1) - b;
        }
        else {
            r = (r << 1) + ((a0 >> i) & 0x1);
            if (r >= b) {
                q = q + (x >> (WORD_BIT_SIZE-1-i));
                r = r - b;
            }
        }
    }
    return q;
}