#include "bring_arith.h"

int main(){
    BIGINT* num0 = NULL;
    BIGINT* num1 = NULL;
    BIGINT* result1 = NULL;
    printf("=============  빅인티저 생성 중 ============== \n");
    bi_set_by_string(&num0, NEGATIVE, "100000000000000000000000", HEXDECIMAL);
    bi_set_by_string(&num1, NEGATIVE, "ffffffffffffffffffffffff", HEXDECIMAL);
    bi_print_bigint_hex(num0);
    bi_print_bigint_hex(num1);

    printf("=============  뺄셈 검증 예시  ============== \n");
    BI_Sub_zxy(&result1, num0, num1);
    bi_print_bigint_hex(result1);
    bi_print_bigint_hex(num1);

    bi_delete(&num0);
    bi_delete(&num1);
    bi_delete(&result1);
    return 0;
}