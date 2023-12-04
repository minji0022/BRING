#include "bring_arith.h"

int main(){
    int n = 100;
    while (n>0){
        BIGINT* A = NULL;
        BIGINT* N = NULL;
        BIGINT* T = NULL;
        BIGINT* R = NULL;

        //printf("=============  빅인티저 생성 중 ============== \n");
        bi_gen_rand(&N, NON_NEGATIVE, 10);
        bi_gen_rand(&A, NON_NEGATIVE, 19);
        //int wordlenOfN = N->wordlen;
        //bi_print_bigint_hex(N);

        //printf("=============  바렛 검증 예시  ============== \n");
        bi_BR_pre_computed(&T, N);
        BI_Barret_Reduction(&R, A, N, T);
        //bi_print_bigint_hex(T);

        // printf("print( 2**(%d*2*%d) // ", WORD_BIT_SIZE ,wordlenOfN);
        // bi_print_bigint_hex(N);
        // printf("== ");
        // bi_print_bigint_hex(T);
        // printf(")\n");

        printf("print( ");
        bi_print_bigint_hex(A);
        printf(" %% ");
        bi_print_bigint_hex(N);
        printf(" == ");
        bi_print_bigint_hex(R);
        printf(")\n");

        bi_delete(&A);
        bi_delete(&N);
        bi_delete(&T);
        bi_delete(&R);
        n--;
    }

    return 0;
}