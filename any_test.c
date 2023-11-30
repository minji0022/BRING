#include "bring_arith.h"

int main() {
    int n = 50;

    while(n>0){
        BIGINT* num0 = NULL;

        BIGINT* result = NULL;

        bi_gen_rand(&num0, NON_NEGATIVE, 50);

        bi_left_bit_shift_zx(&result, num0, 100);

        printf("print(");
        bi_print_bigint_hex_lb(num0);
        printf(" << 100 == ");
        bi_print_bigint_hex_lb(result);
        printf(")");

        puts("");
        n--;
    }
    
    return 0;
}