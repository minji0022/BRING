#include "bring_arith.h"

int main() {
    int n = 50;

    while(n>0){
        BIGINT* num0 = NULL;
        BIGINT* num1 = NULL;
        
        BIGINT* q = NULL;
        BIGINT* r = NULL;

        bi_gen_rand(&num0, NON_NEGATIVE, 15);
        bi_gen_rand(&num1, NON_NEGATIVE, 10);

        BI_Div_zxy(&q, &r, num0, num1);

        printf("print(");
        bi_print_bigint_hex_lb(num0);
        printf(" // ");
        bi_print_bigint_hex_lb(num1);
        printf(" == ");
        bi_print_bigint_hex_lb(q);
        printf(")");
        puts("");

        printf("print(");
        bi_print_bigint_hex_lb(num0);
        printf(" %% ");
        bi_print_bigint_hex_lb(num1);
        printf(" == ");
        bi_print_bigint_hex_lb(r);
        printf(")");
        puts("");
        n--;
        
        bi_delete(&num0);
        bi_delete(&num1);
        bi_delete(&q);
        bi_delete(&r);

    }
    
    return 0;
}