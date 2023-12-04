#include "bring_arith.h"

//===============================================================================================//
//                                       사용자용 함수 구역
//                                     사용자가 직접 사용할 함수
//===============================================================================================//

/**
* @details [곱셈 함수] 두 큰 정수(BIGINT)를 곱하는 연산
* @param[out] bi_dst 곱셈 결과 (= src1 * src2)
* @param[in] bi_src1 입력 src1
* @param[in] bi_src2 입력 src2
* @return Success or Error Code
*/
int BI_Mul_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2) {
    // Error: 입력이 유효하지 않은 경우 - NULL_POINTER_ERROR
    if(bi_src1 == NULL || bi_src2 == NULL) { 
        printf("[WARNING] : A 또는 B의 값이 존재하지 않음\n");
        return NULL_POINTER_ERROR;
    } 

    // Case 1: 두 수 중 하나라도 0인 경우, 곱셈 결과는 0.
    if(bi_is_zero(bi_src1) || bi_is_zero(bi_src2)) {
        bi_set_zero(bi_dst);
        return FUNC_SUCCESS;
    }
    // Case 2-1: dst = 1(or -1) * src2
    if(bi_abs_is_one(bi_src1)) {
        if(bi_src1->sign == NEGATIVE) {
            bi_assign_flip_sign(bi_dst, bi_src2);
        }
        else {
            bi_assign(bi_dst, bi_src2);
        }
        return FUNC_SUCCESS;
    }
    // Case 2-2: dst = src1 * 1(or -1)
    if(bi_abs_is_one(bi_src2)) {
        if(bi_src2->sign == NEGATIVE) {
            bi_assign_flip_sign(bi_dst, bi_src1);
        }
        else {
            bi_assign(bi_dst, bi_src1);    
        }
        return FUNC_SUCCESS;
    }

    //bi_dst 할당. 이게 필요 없나? schoolbook에서는 처음에 c=0 으로 초기화 해 줌.
    bi_new(bi_dst, (bi_src1->wordlen + bi_src2->wordlen + 1));

    // 추후 카라추바 곱셈으로 변경 예정
    //bi_Mul_Schoolbook_zxy(bi_dst, bi_src1, bi_src2);
    bi_Mul_Karatsuba(bi_dst, bi_src1, bi_src2, 8);
    
    if((bi_src1->sign == NEGATIVE && bi_src2->sign == NON_NEGATIVE) || (bi_src1->sign == NON_NEGATIVE && bi_src2->sign == NEGATIVE)) {
        (*bi_dst)->sign = NEGATIVE;
    }

    return FUNC_SUCCESS;
}

int BI_Mul_xy(BIGINT** bi_src1, BIGINT* bi_src2) {
    BIGINT* tmp = NULL;
    
    BI_Mul_zxy(&tmp, *bi_src1, bi_src2);
    bi_assign(bi_src1, tmp);

    bi_delete(&tmp);
    return FUNC_SUCCESS;
}

/**
* @details [제곱 함수] 큰 정수(BIGINT)를 제곱하는 연산
* @param[out] bi_dst 제곱 결과 (= src * src)
* @param[in] bi_src 입력 src
* @return Success or Error Code
*/
int BI_Sqr_zx(BIGINT** bi_dst, BIGINT* bi_src) {
    // Error: 입력이 유효하지 않은 경우 - NULL_POINTER_ERROR
    if (bi_src == NULL) {
        return NULL_POINTER_ERROR;
    } 

    // Case 1: 입력이 0인 경우, 제곱 결과는 0.
    if (bi_is_zero(bi_src)) {
        bi_set_zero(bi_dst);
        return FUNC_SUCCESS;
    }
    // Case 2: dst = (1(or -1))^2
    if (bi_abs_is_one(bi_src)) {
        bi_set_one(bi_dst);
        return FUNC_SUCCESS;
    }
    
    // bi_new(bi_dst, 1);
    // printf("here");
    bi_Sqrc_zy(bi_dst, bi_src); /* 그 외 경우 */
    (*bi_dst)->sign = NON_NEGATIVE;

    return FUNC_SUCCESS;
}

int BI_Sqr_x(BIGINT** bi_src) {
    BIGINT* tmp = NULL;
    
    BI_Sqr_zx(&tmp, *bi_src);
    bi_assign(bi_src, tmp);

    bi_delete(&tmp);
    return FUNC_SUCCESS;
}
//===============================================================================================//
//                                       개발자용 함수 구역
//                                  사용자는 직접 사용하지 않는 함수
//===============================================================================================//

void bi_Mul_w(BIGINT** bi_dst, word p_src1, word p_src2){
    int w_div_2 = WORD_BIT_SIZE / 2;

    word src1_0 = p_src1 & (((word)1 << w_div_2) - 1);
    word src1_1 = p_src1 >> w_div_2;
    word src2_0 = p_src2 & (((word)1 << w_div_2) - 1);
    word src2_1 = p_src2 >> w_div_2;

    word t0 = src1_1 * src2_0;
    word t1 = src1_0 * src2_1;

    t0 = t0 + t1;
    t1 = (word)(t0 < t1);
    
    word c1 = src1_1 * src2_1;
    word c0 = src1_0 * src2_0;

    word t = c0;
    c0 = c0 + (t0 << w_div_2);
    c1 = c1 + (t1 << w_div_2) + (t0 >> w_div_2) + (word)(c0 < t);

    (*bi_dst)->p[0] = c0;
    (*bi_dst)->p[1] = c1;
}

void bi_Mul_Schoolbook_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2) {
    BIGINT* tmp = NULL;
    BIGINT* lshfit_tmp = NULL;

    bi_set_zero(bi_dst);

    bi_new(&tmp, 2);
    bi_new(&lshfit_tmp, 2);

    for(int j = 0; j < bi_src1->wordlen; j++) {
        for(int i = 0; i < bi_src2->wordlen; i++) {
            bi_Mul_w(&tmp, bi_src1->p[j], bi_src2->p[i]);
            bi_left_bit_shift_zx(&lshfit_tmp, tmp, (i + j) << SHIFT_SIZE);
            BI_Add_xy(bi_dst, lshfit_tmp);
        }
    }

    // tmp variable... delete
    bi_delete(&tmp);     bi_delete(&lshfit_tmp);
}

void concatenateBigInt(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2) {
    (*bi_dst)->wordlen = bi_src1->wordlen + bi_src2->wordlen;
    (*bi_dst)->p = (word*)realloc((*bi_dst)->p, (*bi_dst)->wordlen * sizeof(int));

    for (int i = 0; i < bi_src1->wordlen; ++i) {
        (*bi_dst)->p[i] = bi_src1->p[i];
    }

    for (int i = 0; i < bi_src2->wordlen; ++i) {
        (*bi_dst)->p[bi_src1->wordlen + i] = bi_src2->p[i];
    }
}

void bi_Mul_ImprovedSchoolbook_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2) {
    BIGINT* tmp0 = NULL;
    BIGINT* tmp1 = NULL;
    BIGINT* tmp = NULL;
    BIGINT* mul_tmp0 = NULL;
    BIGINT* mul_tmp1 = NULL;
    BIGINT* lshfit_tmp = NULL;

    bi_new(&tmp0, 0);
    
    bi_new(&tmp1, WORD_BIT_SIZE);
    bi_new(&tmp, 0);
    bi_new(&mul_tmp0, 0);
    bi_new(&mul_tmp1, 0);
    bi_new(&lshfit_tmp, 0);
    
    bi_set_zero(bi_dst);

    for(int i = 0; i < 2 * bi_src2->wordlen; i++) {
        tmp0->p = NULL;
        memset(tmp1->p, 0, WORD_BIT_SIZE);
        for(int k = 0; k < bi_src1->wordlen; k++) {
            bi_Mul_w(&mul_tmp0, bi_src1->p[2*k], bi_src2->p[i]);
            concatenateBigInt(&tmp0, tmp0, mul_tmp0);
            tmp0->wordlen += mul_tmp0->wordlen;
            bi_Mul_w(&mul_tmp1, bi_src1->p[2*k + 1], bi_src2->p[i]);
            concatenateBigInt(&tmp1, tmp1, mul_tmp1);
            tmp1->wordlen += mul_tmp1->wordlen;
        }
        BI_Add_zxy(&tmp, tmp0, tmp1);
        bi_left_bit_shift_zx(&lshfit_tmp, tmp, WORD_BIT_SIZE * i);
        BI_Add_xy(bi_dst, lshfit_tmp);
    }
}

void bi_Sqr_w(BIGINT** bi_dst, word p_src1) {
    word A0, A1, C1, C0 = 0;
    int w_div_2 = WORD_BIT_SIZE >> 1;
    BIGINT* C = NULL;
    BIGINT* T = NULL;
    BIGINT* T_tmp = NULL;

    A1 = p_src1 >> w_div_2;
    A0 = p_src1 & (((word)1 << w_div_2) - 1);

    C1 = A1 * A1;
    C0 = A0 * A0;

    bi_new(&C, 2);
    C->p[0] = C0;
    C->p[1] = C1;
    
    bi_new(&T_tmp, 1);
    T_tmp->p[0] = A0 * A1;
    bi_left_bit_shift_zx(&T, T_tmp, w_div_2 + 1);
    BI_Add_xy(&C, T);
    bi_assign(bi_dst, C);

    bi_delete(&C);
    bi_delete(&T);
    bi_delete(&T_tmp);
}

void bi_Sqrc_zy(BIGINT** bi_dst, BIGINT* bi_src1) {
    BIGINT* C1 = NULL;
    BIGINT* C2 = NULL;
    BIGINT* C2_tmp = NULL;

    bi_set_zero(&C1);
    bi_set_zero(&C2);
    for(int j = 0; j < bi_src1->wordlen; j++) {
        BIGINT* T1 = NULL;
        BIGINT* T1_tmp = NULL;

        bi_Sqr_w(&T1, (bi_src1->p[j]));
        bi_left_bit_shift_zx(&T1_tmp, T1, (j << (1 + SHIFT_SIZE)));
        //bi_left_bit_shift_zx(&T1_tmp, T1, (2 * j * WORD_BIT_SIZE));

        BI_Add_xy(&C1, T1_tmp);
        for(int i = j + 1; i < bi_src1->wordlen; i++) {
            BIGINT* T2 = NULL;
            BIGINT* T2_tmp = NULL;

            bi_new(&T2, 2);

            bi_Mul_w(&T2, bi_src1->p[j], bi_src1->p[i]);
            bi_left_bit_shift_zx(&T2_tmp, T2, (i + j) << SHIFT_SIZE);
            //bi_left_bit_shift_zx(&T2_tmp, T2, (i + j) * WORD_BIT_SIZE);
            BI_Add_xy(&C2, T2_tmp);
        
            bi_delete(&T2);
            bi_delete(&T2_tmp);
        }
        bi_delete(&T1);
        bi_delete(&T1_tmp);
    }
    bi_left_bit_shift_zx(&C2_tmp, C2, 1);
    BI_Add_zxy(bi_dst, C1, C2_tmp);

    bi_delete(&C1);
    bi_delete(&C2);
    bi_delete(&C2_tmp);
}

void bi_Mul_Karatsuba(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2, int flag){
    
    BIGINT* temp_mul = NULL;
    BIGINT* temp_C = NULL;
    BIGINT *A0 = NULL, *A1 = NULL, *B0 = NULL, *B1 = NULL;
    BIGINT *T0 = NULL, *T1 = NULL, *T1_1 = NULL;
    BIGINT *S = NULL, *S0 = NULL, *S1 = NULL;

    if (flag >= bi_get_min_length(bi_src1, bi_src2)) {
        BI_Mul_zxy(bi_dst, bi_src1, bi_src2); 
    }

    int l = (bi_get_max_length(bi_src1, bi_src2) + 1) >> 1;
    //int lw = l << SHIFT_SIZE;
    int lw = l * WORD_BIT_SIZE;
    
    printf("************ lw = %d ***************\n", lw);
    bi_right_bit_shift_zx(&A1, bi_src1, lw); // A1 <- A >> lw
    reductionOf2(&A0, bi_src1, lw); // A0 <- A mod 2^lw
    bi_right_bit_shift_zx(&B1, bi_src2, lw); // B1 <- B >> lw
    reductionOf2(&B0, bi_src2, lw); // B0 <- B mod 2^lw

    printf("************************************\n");
    bi_print_bigint_hex(A1);
    bi_print_bigint_hex(B1);
    
    printf("flag : %d,     A1's wordlen' : %d,     B1's wordlen : %d\n", flag, A1->wordlen, B1->wordlen);
    bi_print_bigint_hex(*bi_dst);

    bi_Mul_Karatsuba(&T1, A1, B1, flag); // T1 <- A1*B1
    printf("Not here\n");
    bi_Mul_Karatsuba(&T0, A0, B0, flag); // T0 <- A0*B0

    bi_left_word_shift_zx(&T1_1, T1, l<<1); // T1_1 <- T1 << 2lw

    BI_Add_zxy(&temp_C, T1_1, T0); // C <- T1 + T0

    BI_Sub_zxy(&S1, A0, A1); // S1 <- A0 - A1
    BI_Sub_zxy(&S0, B1, B0); // S1 <- B1 - B0
    
    int S_sign = (S1->sign) ^ (S0->sign);

    S1->sign = NON_NEGATIVE; S0->sign = NON_NEGATIVE;
    bi_Mul_Karatsuba(&S, S1, S0, flag); // S <- S1*S0
    S->sign = S_sign;

    BI_Add_xy(&S, T1); // S <- S + T1
    BI_Add_xy(&S, T0); // S <- S + T1
    bi_left_word_shift(S, l); // S <- S <<lw
    BI_Add_zxy(bi_dst, temp_C, S); // C <- C + S


    bi_delete(&temp_C); bi_delete(&temp_mul);
    bi_delete(&A0); bi_delete(&A1);
    bi_delete(&B0); bi_delete(&B1);
    bi_delete(&T0); bi_delete(&T1); bi_delete(&T1_1);
    bi_delete(&S0); bi_delete(&S1); bi_delete(&S);

}