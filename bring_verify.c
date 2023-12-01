#include "bring_arith.h"

int main() {
    int n = 1000;

    while(n>0){

        BIGINT* num0 = NULL;
        BIGINT* num1 = NULL;
        
        BIGINT* q = NULL;
        BIGINT* r = NULL;
        BIGINT* res_mul = NULL;

        bi_gen_rand(&num0, NON_NEGATIVE, 15);
        bi_gen_rand(&num1, NON_NEGATIVE, 11);

        BI_Div_zxy(&q, &r, num0, num1);
        // /*
        printf("print(");
        bi_print_bigint_hex(num0);
        printf(" // ");
        bi_print_bigint_hex(num1);
        printf(" == ");
        bi_print_bigint_hex(q);
        printf(")");
        puts("");

        printf("print(");
        bi_print_bigint_hex(num0);
        printf(" %% ");
        bi_print_bigint_hex(num1);
        printf(" == ");
        bi_print_bigint_hex(r);
        printf(")");
        puts("");
        // */
        // BI_Mul_zxy(&res_mul, num0, num1);

        // printf("print(");
        // bi_print_bigint_hex(num0);
        // printf(" * ");
        // bi_print_bigint_hex(num1);
        // printf(" == ");
        // bi_print_bigint_hex(res_mul);
        // printf(")");
        // puts("");
        // printf("%d\n", n);

        n--;
        
        bi_delete(&num0);
        bi_delete(&num1);
        bi_delete(&q);
        bi_delete(&r);
        bi_delete(&res_mul);

    }

    return 0;
}