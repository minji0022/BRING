#include "bring_util.h"

int main(){
    BIGINT* num0 = NULL;
    BIGINT* num1 = NULL;

    printf("=============  빅인티저 생성 중 ============== \n");
    bi_set_by_string(&num0, NON_NEGATIVE, "fffffff0000000000000000", HEXDECIMAL);
    bi_print_bigint_hex(num0);
    int n = bi_get_bit_length(num0);
    printf("비트 길이 = %d\n", n);
    bi_assign_flip_sign(&num1, num0);
    bi_print_bigint_hex(num1);
    printf("%d ", num1->sign);
    bi_delete(&num0);
    bi_delete(&num1);
    printf("%lu", WORD_BIT_SIZE);
    return 0;
}