#include "bring_arith.h"

int main() {
    int n = 50;

    while(n>0){
        BIGINT* num0 = NULL;
        BIGINT* num1 = NULL;
        
        BIGINT* result_add = NULL;
        BIGINT* result_sub = NULL;
        BIGINT* result_mul = NULL;

        bi_gen_rand(&num0, NON_NEGATIVE, 15);
        bi_gen_rand(&num1, NON_NEGATIVE, 12);

        BI_Add_zxy(&result_add, num0, num1);
        BI_Sub_zxy(&result_sub, num0, num1);
        BI_Mul_zxy(&result_mul, num0, num1);

        printf("print(");
        bi_print_bigint_hex_lb(num0);
        printf(" + ");
        bi_print_bigint_hex_lb(num1);
        printf(" == ");
        bi_print_bigint_hex_lb(result_add);
        printf(")");
        puts("");

        printf("print(");
        bi_print_bigint_hex_lb(num0);
        printf(" - ");
        bi_print_bigint_hex_lb(num1);
        printf(" == ");
        bi_print_bigint_hex_lb(result_sub);
        printf(")");
        puts("");

        printf("print(");
        bi_print_bigint_hex_lb(num0);
        printf(" * ");
        bi_print_bigint_hex_lb(num1);
        printf(" == ");
        bi_print_bigint_hex_lb(result_mul);
        printf(")");

        puts("");
        n--;
    }
    
    return 0;
}