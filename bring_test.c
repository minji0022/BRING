#include "bring_arith.h"

int main(){
    BIGINT* num0 = NULL;
    BIGINT* num1 = NULL;
    
    BIGINT* result_add = NULL;
    BIGINT* result_sub = NULL;
    BIGINT* result_mul = NULL;

    printf("=============  빅인티저 생성 중 ============== \n");
    // bi_set_by_string(&num0, NON_NEGATIVE, "12343202", HEXDECIMAL);
    // bi_set_by_string(&num1, NON_NEGATIVE, "403d76423", HEXDECIMAL);
    
    int set_flag_1 = bi_set_by_string(&num0, NON_NEGATIVE, "22d6b982d07258407f63917bb38631812e5cd7737c2c2481c3e69486bb8", HEXDECIMAL);
    int set_flag_2 = bi_set_by_string(&num1, NON_NEGATIVE, "7e3c13c8912b75381Fbe34ec439636e01267788", HEXDECIMAL);
    
    // 유효하지 않은 입력이 발생할 경우 오류 메시지 출력 및 프로그램 종료
    if(set_flag_1 == INVALID_CHAR_ERROR) {
        puts("============= Input 1 : wrong! ==============");
        return 0;
    } 
    else if (set_flag_2 == INVALID_CHAR_ERROR) {
        puts("============== Input 2 : wrong! ==============");
        return 0;
    }

    bi_print_bigint_hex(num0);
    bi_print_bigint_hex(num1);

    printf("=============  덧셈 검증 예시  ============== \n");
    BI_Add_zxy(&result_add, num0, num1);
    bi_print_bigint_hex(result_add);

    printf("=============  뺄셈 검증 예시  ============== \n");
    BI_Sub_zxy(&result_sub, num0, num1);
    bi_print_bigint_hex(result_sub);

    printf("=============  곱셈 검증 예시  ============== \n");
    BI_Mul_zxy(&result_mul, num0, num1);
    bi_print_bigint_hex(result_mul);

    bi_delete(&num0);
    bi_delete(&num1);
    bi_delete(&result_add);
    bi_delete(&result_sub);
    bi_delete(&result_mul);

    return 0;
}