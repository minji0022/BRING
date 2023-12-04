#include "bring_arith.h"

int main(){
    int n = 300;
    while (n>0){
        BIGINT* num1 = NULL;
        BIGINT* num2 = NULL;
        BIGINT* result = NULL;

        //printf("=============  빅인티저 생성 중 ============== \n");
        bi_gen_rand(&num1, NON_NEGATIVE, 25);
        bi_gen_rand(&num2, NON_NEGATIVE, 8);

        BI_Mul_zxy(&result, num1, num2);

        printf("print( ");
        bi_print_bigint_hex(num1);
        printf(" * ");
        bi_print_bigint_hex(num2);
        printf(" == ");
        bi_print_bigint_hex(result);
        printf(")\n");

        bi_delete(&num1);
        bi_delete(&num2);
        bi_delete(&result);
        n--;
    }

    return 0;
}