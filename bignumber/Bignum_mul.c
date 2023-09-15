#include "Bignum_arith.h"

//===============================================================================================//
//                                       사용자용 함수 구역
//                                     사용자가 직접 사용할 함수
//
int BI_Mul(BIGINT* bi_Dst, BIGINT* bi_Src1, BIGINT* bi_Src2){
    // 두 빅인티저 중 0이 있는 경우
    if (bi_is_zero(bi_Src1) || bi_is_zero(bi_Src2)) {
        bi_Dst->wordlen = 1;
        bi_Dst->sign = NON_NEGATIVE;
        bi_Dst->bData = (word*)realloc(bi_Dst->bData, sizeof(word));
        bi_Dst->bData[0] = 0x0;
        return FUNC_SUCCESS;
    }
    // 두 빅인티저 중 +- 1이 있는 경우.
    if (bi_is_one(bi_Src1)) {
        for(int i = 0; i < bi_Src2->wordlen; i++){
                bi_Dst->bData[i] = bi_Src2->bData[i]; 
            }
        if (bi_Src1->sign != bi_Src2->sign) bi_Dst->sign = NEGATIVE;
        return bi_refine(bi_Dst);    
        }
    if (bi_is_one(bi_Src2)) {
        for(int i = 0; i < bi_Src1->wordlen; i++){
                bi_Dst->bData[i] = bi_Src1->bData[i]; 
            }
        if (bi_Src1->sign != bi_Src2->sign) bi_Dst->sign = NEGATIVE;
        return bi_refine(bi_Dst);  
        }
    // 그 외 경우
    return bi_Mul_Schoolbook(bi_Dst, bi_Src1, bi_Src2); // 이 부분이 bi_Mul_Schoolbook 또는 bi_Mul_Karatsuba.
}
//===============================================================================================//
//                                       개발자용 함수 구역
//                                  사용자는 직접 사용하지 않는 함수
//
/**** 단일 워드 곱셈 ****/
unsigned long long bi_Mul_w(word Src1_Data, word Src2_Data){
    unsigned int A1, A0, B1, B0;
    unsigned int T1, T0, T;
    unsigned int C1, C0;
    unsigned long long C;

    A1 = (Src1_Data >> 16);
    A0 = Src1_Data & ((1 << 16) - 1);
    B1 = (Src2_Data >> 16);
    B0 = Src2_Data & ((1 << 16) - 1);

    T1 = A1*B0;
    T0 = A0*B1;

    T0 = (T1 + T0) & 0xFFFFFFFF;
    T1 = T0 < T1; // T1 : 0 또는 1.
  
    C1 = A1*B1;
    C0 = A0*B0;
    
    T = C0;
    C0 = (C0 + (T0 << 16)) & 0xFFFFFFFF;
    C1 = C1 + (T1 << 16) + (T0 >> 16) + (C0 < T);

    C = ((unsigned long long)C1 << 32) + C0;
    //printf("C = %llx ", C);

    return C;
    
}

/**** 스쿨북 곱셈 ****/
int bi_Mul_Schoolbook(BIGINT* bi_Dst, BIGINT* bi_Src1, BIGINT* bi_Src2){
    // bi_Dst 0으로 초기화한 상태
    int i, j;
    unsigned long long UV;
    for (i = 0; i < bi_Src1->wordlen; i++){
        unsigned int U = 0x00;
        for (j = 0; j< bi_Src2->wordlen; j++){
            UV = bi_Dst->bData[i+j] + bi_Mul_w(bi_Src1->bData[i], bi_Src2->bData[j]) + U;
            //UV = bi_Dst->bData[i+j] + (unsigned long long)(bi_Src1->bData[i])*bi_Src2->bData[j] + U;
            U = UV >> 32;
            bi_Dst->bData[i+j] = UV & 0xFFFFFFFF;
        }
        bi_Dst->bData[i+j] = U;
    }

    if (bi_Src1->sign != bi_Src2->sign) bi_Dst->sign = NEGATIVE;
    bi_refine(bi_Dst);

    return FUNC_SUCCESS;
}

/**** PS 곱셈 ****/
int bi_Mul_PS(BIGINT* bi_Dst, BIGINT* bi_Src1, BIGINT* bi_Src2){
    word R0 = 0;
    word R1 = 0;
    word R2 = 0;
    int k = 0;
    int i = 0;
    unsigned int U,V;
    unsigned long long UV;

    for (k = 0; k < bi_Src1->wordlen + bi_Src2->wordlen - 1; k++){ //for (k = 0; k < 2*(bi_Src1->wordlen)-1; k++) 
        //unsigned int carry = 0;
        //printf("%08X \n", bi_Src1->bData[i]);
        for (i = 0; i <= k && i < bi_Src1->wordlen; i++ ) {
            unsigned int carry = 0;
            UV = (unsigned long long)(bi_Src1->bData[i])*bi_Src2->bData[k-i];
            U = UV >> 32;
            V = UV & 0xFFFFFFFF;
            carry = bi_Add_w(&R0, R0, V, carry);
            carry = bi_Add_w(&R1, R1, U, carry);
            R2 = R2 + carry;
        }
        //printf("%08X ", R0);
        bi_Dst->bData[k] = R0;
        R0 = R1;
        R1 = R2;
        R2 = 0;
    }
    //printf("%08X \n", R0);
    bi_Dst->bData[k] = R0;
    if (bi_Src1->sign != bi_Src2->sign) bi_Dst->sign = NEGATIVE;
    bi_refine(bi_Dst);
    return FUNC_SUCCESS;
}

/**** Karatsuba 곱셈 ****/
/*
int bi_Mul_Karatsuba(BIGINT* bi_Dst, BIGINT* bi_Src1, BIGINT* bi_Src2){
    unsigned int len;
    len = (bi_get_max_length(bi_Src1, bi_Src2) + 1) >> 1;
    
    return FUNC_SUCCESS;
}*/

/**** 단일 워드 제곱 ****/
// 시프트 연산 방식 수정 필요
// 마지막 C + T 2워드 + 2워드 덧셈이므로 덧셈 함수 사용하기.

unsigned long long bi_Sqr_w(word Src1_Data){
    unsigned int A1, A0;
    unsigned long long T;
    unsigned int C1, C0;
    unsigned long long C;

    A1 = (Src1_Data >> 16);
    A0 = Src1_Data & ((1 << 16) - 1);
    printf("A1 = %08X ", A1);
    printf("A0 = %08X\n", A0);

    C1 = A1*A1;
    C0 = A0*A0;
    printf("C1 = A1*A1 %08X ", C1);
    printf("C0 = A0*A0 %08X\n", C0);
    
    C = ((unsigned long long)C1 << 32) + C0;
    printf("C = ((unsigned long long)C1 << 32) + C0 = %llX\n", C);

    T = A0*A1;
    printf("T = A0*A1 = %llX\n", T);
    T = T << 17; //T = T << (16 +1)
    printf("T = (unsigned long long)T << 17 = %llX\n", T);
    C = C + T;

    return C;
    
}


int bi_Sqr_PS(BIGINT* bi_Dst, BIGINT* bi_Src1){
    word R0 = 0;
    word R1 = 0;
    word R2 = 0;
    int k = 0;
    int i = 0;
    unsigned int U,V;
    unsigned long long UV;
    unsigned long long UV2;

    for (k = 0; k < 2*(bi_Src1->wordlen) - 1; k++){ // 2*(bi_Src1->wordlen) 이게 맞는지 확인 필요.
        for (i = 0; i <= k/2 && i < bi_Src1->wordlen; i++ ) { // i <= k/2
            if (k-i < bi_Src1->wordlen){
                unsigned int carry = 0;
                UV = (unsigned long long)(bi_Src1->bData[i])*bi_Src1->bData[k-i];
                if (i < k-i) { 
                    UV2 = (2*UV) & 0xFFFFFFFFFFFFFFFF;
                    if (UV2 < UV) carry = 1; 
                    R2 = R2 + carry;
                    UV = UV2;
                    //break;                
                }
                U = UV >> 32;
                V = UV & 0xFFFFFFFF;
                carry = bi_Add_w(&R0, R0, V, 0);
                carry = bi_Add_w(&R1, R1, U, carry);
                R2 = R2 + carry;
            }
        }
        bi_Dst->bData[k] = R0;
        R0 = R1;
        R1 = R2;
        R2 = 0;
    }
    bi_Dst->bData[k] = R0;

    bi_Dst->sign = NON_NEGATIVE;
    bi_refine(bi_Dst);

    return FUNC_SUCCESS;
}
