#include "Bignum_arith.h"

int main() {
    BIGINT* num0 = NULL;
    BIGINT* num1 = NULL;
    BIGINT* num2 = NULL;
    BIGINT* num3 = NULL;
    BIGINT* num4 = NULL;
    BIGINT* num5 = NULL;
    BIGINT* num6 = NULL;
    BIGINT* num7 = NULL;
    BIGINT* num8 = NULL;
    BIGINT* num9 = NULL;
    BIGINT* num10 = NULL;
    BIGINT* num11 = NULL;
    BIGINT* num12 = NULL;

    printf("=============  빅인티저 생성 중 ============== \n");
    bi_set_by_string(&num0, NON_NEGATIVE, "10000000000000000");
    bi_set_by_string(&num1, NON_NEGATIVE, "FFFFFFFFFFFFFFFF");
    bi_set_by_string(&num2, NON_NEGATIVE, "1");
    bi_set_by_string(&num3, NEGATIVE, "FFFFFFFFFFFFFFFF");
    bi_set_by_string(&num4, NEGATIVE, "1");
    bi_set_by_string(&num5, NON_NEGATIVE, "0");
    printf("=============  빅인티저 생성 완료 ============== \n");

    printf("=============  빅인티저 덧, 뺄셈 연산 시작 ============== \n");
    bi_create(&num6, bi_get_max_length(num1,num2)+1);
    BI_Add(num6, num2, num1);
    printf("FFFFFFFF FFFFFFFF + 1 = ");
    bi_print_bigint(num6);

    bi_create(&num7, bi_get_max_length(num3,num4)+1);
    BI_Add(num7, num4, num3);
    printf("-FFFFFFFF FFFFFFFF + -1 = ");
    bi_print_bigint(num7);

    bi_create(&num8, bi_get_max_length(num6,num7)+1);
    BI_Add(num8, num6, num7);
    printf("위의 두 값의 합 = ");
    bi_print_bigint(num8);

    bi_create(&num9, bi_get_max_length(num1,num6));
    BI_Sub(num9, num6, num1);
    printf("(FFFFFFFF FFFFFFFF + 1) - (FFFFFFFF FFFFFFFF) = ");
    bi_print_bigint(num9);
    
    /*
    bi_create(&num9, bi_get_max_length(num1,num6));
    BI_Sub(num9, num0, num1);
    printf("(1 00000000 00000000) - (FFFFFFFF FFFFFFFF) = ");
    bi_print_bigint(num9);*/

    bi_create(&num10, bi_get_max_length(num0,num1));
    BI_Sub(num10, num1, num0);
    printf("(FFFFFFFF FFFFFFFF) - 1 00000000 00000000 = ");
    bi_print_bigint(num10);

    bi_create(&num11, bi_get_max_length(num1,num2));
    BI_Sub(num11, num2, num1);
    printf("1 - FFFFFFFF FFFFFFFF = ");
    bi_print_bigint(num11);

    bi_create(&num12, bi_get_max_length(num1,num1)+1);
    BI_Add(num12, num1, num1);
    printf("FFFFFFFF FFFFFFFF + FFFFFFFF FFFFFFFF = ");
    bi_print_bigint(num12);
    printf("=============  빅인티저 덧, 뺄셈 연산 종료 ============== \n");

    BIGINT* num13 = NULL;
    BIGINT* num14 = NULL;
    BIGINT* num15 = NULL;
    BIGINT* num16 = NULL;

    printf("=============  빅인티저 곱셈 연산 시작 ============== \n");
    printf("num13 = ");
    bi_set_by_string(&num13, NON_NEGATIVE, "BD91C935C85617B079C6F2728B987CE488BB17B4644D5F8B9C23AF955AB74663");
    printf("num14 = ");
    bi_set_by_string(&num14, NEGATIVE, "09745AE9B45ECD62652851E0D1EF8A496B9C64E24CE34D0DED336E398902D302");
    
    bi_create(&num15, bi_get_length(num13)+bi_get_length(num14));
    BI_Mul(num15, num2, num5);
    printf("num 13 * num14 = ");
    bi_print_bigint(num15);
    printf("=============  빅인티저 곱셈 연산 종료 ============== \n");
    //=======================================
    bi_destroy_bigint(&num0);
    bi_destroy_bigint(&num1), bi_destroy_bigint(&num2), bi_destroy_bigint(&num3);
    bi_destroy_bigint(&num4), bi_destroy_bigint(&num5), bi_destroy_bigint(&num6);
    bi_destroy_bigint(&num7), bi_destroy_bigint(&num8);
    bi_destroy_bigint(&num9);
    bi_destroy_bigint(&num10), bi_destroy_bigint(&num11), bi_destroy_bigint(&num12);
    bi_destroy_bigint(&num13), bi_destroy_bigint(&num14), bi_destroy_bigint(&num15);
    bi_destroy_bigint(&num16);

    return 0;

}
