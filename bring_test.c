#include "bring_arith.h"

int main(){
    BIGINT* num0 = NULL;
    BIGINT* num1 = NULL;
    
    BIGINT* result_add = NULL;
    BIGINT* result_sub = NULL;

    printf("=============  빅인티저 생성 중 ============== \n");
    // bi_set_by_string(&num0, NON_NEGATIVE, "100000000000000000000000", HEXDECIMAL);
    // bi_set_by_string(&num1, NON_NEGATIVE, "ffffffffffffffffffffffff", HEXDECIMAL);
    bi_set_by_string(&num0, NON_NEGATIVE, "c6b3fc441f7247e6be0c5a1e10227fb81085e7c2", HEXDECIMAL);
    bi_set_by_string(&num1, NON_NEGATIVE, "fa81c7ccfe5d9ddd8114e887f0900b00044b9dcf", HEXDECIMAL);
    bi_print_bigint_hex(num0);
    bi_print_bigint_hex(num1);

    printf("=============  덧셈 검증 예시  ============== \n");
    BI_Add_zxy(&result_add, num0, num1);
    bi_print_bigint_hex(result_add);

    printf("=============  뺄셈 검증 예시  ============== \n");
    BI_Sub_zxy(&result_sub, num0, num1);
    bi_print_bigint_hex(result_sub);
    // bi_print_bigint_hex(num1);

    bi_delete(&num0);
    bi_delete(&num1);
    bi_delete(&result_add);
    bi_delete(&result_sub);

    return 0;
}