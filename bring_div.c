#include "bring_arith.h"
//===============================================================================================//
//                                       사용자용 함수 구역
//                                     사용자가 직접 사용할 함수
int BI_Div_zxy(BIGINT** bi_quotient, BIGINT** bi_remainder, BIGINT* bi_src1, BIGINT* bi_src2){
    if (bi_src1 == NULL || bi_src2 == NULL) {
        return NULL_POINTER_ERROR;
    }   

    // A < B
    if (bi_compare_bigint(bi_src1, bi_src2) == -1) {
        bi_set_zero(bi_quotient); // 몫 : 0
        bi_assign(bi_remainder, bi_src1); // 나머지 : A
        return FUNC_SUCCESS;
    }

    int n = bi_src1->wordlen;
    bi_set_zero(bi_remainder); // line 4
    bi_new(bi_quotient, n);

    for (int i = n-1; i >= 0; i--) {
        BIGINT* tQ = NULL;
        BIGINT* tR = NULL;
        BIGINT* tAi = NULL;
        bi_new(&tAi, 1);

        tAi->p[0] = bi_src1->p[i];

        bi_left_word_shift(*bi_remainder, 1); // line 6
        BI_Add_zxy(&tR, *bi_remainder, tAi); // line 6

        bi_Divc_zxy(&tQ, bi_remainder, tR, bi_src2); // line 7
        (*bi_quotient)->p[i] = tQ->p[0];
        
        bi_delete(&tQ); // 이 부분들 필요한지 고민해봐야 함.
        bi_delete(&tR); // 이 부분들 필요한지 고민해봐야 함.
        bi_delete(&tAi); // 이 부분들 필요한지 고민해봐야 함.
    }
    bi_refine(*bi_quotient);
    return FUNC_SUCCESS;
}

//===============================================================================================//

//===============================================================================================//
//                                       개발자용 함수 구역
//                                  사용자는 직접 사용하지 않는 함수
//
/*
A(bi_src1)의 길이는 m+1
B(bi_src2)의 길이는 m 이 되게끔 넣어줌.
*/
void bi_Divc_zxy(BIGINT** bi_quotient, BIGINT** bi_remainder, BIGINT* bi_src1, BIGINT* bi_src2){
    bi_new(bi_quotient, 1);

    int m = bi_src2->wordlen;
    int k = 0;  
    BIGINT* tA = NULL;
    BIGINT* tB = NULL;
    BIGINT* tQ = NULL;
    BIGINT* tR = NULL;
    
    // A < B인 경우
    if (bi_compare_bigint(bi_src1, bi_src2) == -1) {
        bi_set_zero(bi_quotient); // 몫 : 0
        bi_assign(bi_remainder, bi_src1); // 나머지 : A
        return ;
    }

    while (bi_src2->p[m-1]<<k < 0x80000000) {
        k++;
    }
    
    bi_left_bit_shift_zx(&tA, bi_src1, k); // line 5
    bi_left_bit_shift_zx(&tB, bi_src2, k); // line 5

    bi_Divcc_zxy(&tQ, &tR, tA, tB); // line 6
    bi_assign(bi_quotient, tQ); // line 7 몫 복사
    bi_right_bit_shift_zx(bi_remainder, tR, k); // line 7

    bi_delete(&tA); // 이 부분들 필요한지 고민해봐야 함.
    bi_delete(&tB); // 이 부분들 필요한지 고민해봐야 함.
    bi_delete(&tQ); // 이 부분들 필요한지 고민해봐야 함.
    bi_delete(&tR); // 이 부분들 필요한지 고민해봐야 함.   
}

void bi_Divcc_zxy(BIGINT** bi_quotient, BIGINT** bi_remainder, BIGINT* bi_src1, BIGINT* bi_src2){
    int n = bi_src1->wordlen; // A의 길이
    int m = bi_src2->wordlen; // B의 길이
    BIGINT* BQ = NULL;
    BIGINT* tR = NULL;
    bi_new(bi_quotient, 1);

    // 길이가 같은 경우, if n = m
    if (n == m) {
        //printf("DIVCC n=m인 경우 계산\n");
        (*bi_quotient)->p[0] = bi_src1->p[m-1] / bi_src2->p[m-1];
    }
    
    // if n = m+1
    else if (n == m+1) {
        //printf("DIVCC n=m+1인 경우 계산\n");
        if (bi_src1->p[m] == bi_src2->p[m-1]){
            //printf("DIVCC case1 계산\n");
            (*bi_quotient)->p[0] = 0xFFFFFFFF;
        }
        else { // longdiv 써야 함.
            //bi_quotient->p[0] = ((bi_src1->p[m]<<32) + bi_src1->p[m-1]) / bi_src2->p[m-1];
            //printf("DIVCC case2 계산\n");
            (*bi_quotient)->p[0] = bi_longdiv(bi_src1->p[m], bi_src1->p[m-1], bi_src2->p[m-1]);
        }
    }
    
    //bi_Mul_PS(&BQ, bi_src2, bi_quotient); // BQ 구하기
    BI_Mul_zxy(&BQ, bi_src2, *bi_quotient); // BQ 구하기
    BI_Sub_zxy(bi_remainder, bi_src1, BQ); // R <- A - BQ
    
    while ((*bi_remainder)->sign == NEGATIVE) {
        (*bi_quotient)->p[0] = (*bi_quotient)->p[0] - 1;
        BI_Add_zxy(&tR, *bi_remainder, bi_src2);
        bi_assign(bi_remainder, tR);
    }
    bi_delete(&BQ); // 이 부분들 필요한지 고민해봐야 함.
    bi_delete(&tR); // 이 부분들 필요한지 고민해봐야 함.   
    
}

word bi_longdiv(word a1, word a0, word b) {
    word x = 0x80000000;
    word q = 0;
    word r = a1;

    for (int i = 31; i >= 0; i--) {
        if (r >= 0x80000000) {
            q = q + (x >> (31-i));
            r = (r << 1) + ((a0 >> i) & 0x1) - b;
        }
        else {
            r = (r << 1) + ((a0 >> i) & 0x1);
            if (r >= b) {
                q = q + (x >> (31-i));
                r = r - b;
            }
        }
    }
    return q;
}