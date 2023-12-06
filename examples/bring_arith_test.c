// -------------------------------------------------------------------------------
//  BRING이 제공하는 연산 in arith.c 예제 코드
//  
//  [ 지원 연산 ]
//  - 덧셈 [+ 갱신]
//  - 뺄셈 [+ 갱신]
//  - 곱셈 [+ 갱신]
//  - 나눗셈
//  - 제곱 [+ 갱신]
//  - 모듈러 [+ 갱신]
//  - 모듈러 지수승
// -------------------------------------------------------------------------------

#include "../bring_arith.h"

int main(){
    // -------------------------------------------------------------------------------
    // STEP 1. BIGINT SET: 16진수 문자열을 입력 받아 BIGINT 변수에 할당
    // -------------------------------------------------------------------------------
    // [ bi_set_by_string 주의사항 ]
    // - 입력: (할당할 BIGINT 구조체 더블 포인터, 부호 정보, 16진수 문자열, HEXDECIMAL)
    // - 출력: SUCCESS or ERROR CODE (int 변수로 반환값을 저장한 뒤, 에러 코드를 처리할 추가 코드 작성 필요)
    // -------------------------------------------------------------------------------    
    
    // 0. BIGINT 변수를 NULL로 초기화하여 선언
    BIGINT* num0 = NULL;
    BIGINT* num1 = NULL;

    // 1. BIGINT Set
    printf("=============  빅인티저 생성  ============== \n");
    int set_flag_0 = bi_set_by_string(&num0, NON_NEGATIVE, "1234567890abcdef1234567890abcdef1234567890abcdef", HEXDECIMAL);
    int set_flag_1 = bi_set_by_string(&num1, NON_NEGATIVE, "f2144faddc497d9ef6324912fd367840ee509a2032aedb1c0a890d133b45f596", HEXDECIMAL);
    
    // 1.1 유효하지 않은 입력이 발생할 경우 오류 메시지 출력 및 프로그램 종료
    if(set_flag_0 == INVALID_CHAR_ERROR) {
        puts("============= Input 1 : wrong! ==============");
        return 0;
    } 
    else if (set_flag_1 == INVALID_CHAR_ERROR) {
        puts("============== Input 2 : wrong! ==============");
        return 0;
    }

    // 1.2 예제 코드 확인을 위한 num0, num1 출력
    printf("A = ");   bi_print_bigint_hex(num0);      puts("");
    printf("B = ");   bi_print_bigint_hex(num1);      puts("");

    // 2. main 함수 제일 마지막에 Set해준 BIGINT 변수 삭제
    
    // -------------------------------------------------------------------------------
    // Function 1. 덧셈
    // -------------------------------------------------------------------------------
    // - 입력: (덧셈 결과를 저장할 BIGINT 구조체 더블 포인터, 입력 BIGINT 포인터 num0, 입력 BIGINT 포인터 num1)
    // - 출력: SUCCESS or ERROR CODE (int 변수로 반환값을 저장한 뒤, 에러 코드를 처리할 추가 코드 작성 필요)
    // -------------------------------------------------------------------------------
    
    // 0. BIGINT 변수를 NULL로 초기화하여 선언
    BIGINT* result_add = NULL;

    // 1. 덧셈 연산
    printf("\n=============  덧셈 검증 예시  ============== \n");
    int add_flag = BI_Add_zxy(&result_add, num0, num1);
    
    // 1.1 덧셈 함수 연산 실패 시, 실패 메시지 출력 및 프로그램 종료
    if(add_flag != FUNC_SUCCESS) {
        puts("============= Add Fail ==============");
        return 0;
    } 
    // 1.2 덧셈 결과 값 출력
    printf("A + B = ");     bi_print_bigint_hex(result_add);    printf("\n");

    // 2. BIGINT 변수 삭제
    bi_delete(&result_add);

    // -------------------------------------------------------------------------------
    // Function 2. 뺄셈
    // -------------------------------------------------------------------------------
    // - 입력: (뺄셈 결과를 저장할 BIGINT 구조체 더블 포인터, 입력 BIGINT 포인터 num0, 입력 BIGINT 포인터 num1)
    // - 출력: SUCCESS or ERROR CODE (int 변수로 반환값을 저장한 뒤, 에러 코드를 처리할 추가 코드 작성 필요)
    // -------------------------------------------------------------------------------
    
    // 0. BIGINT 변수를 NULL로 초기화하여 선언
    BIGINT* result_sub = NULL;

    // 1. 뺄셈 연산
    printf("\n=============  뺄셈 검증 예시  ============== \n");
    int sub_flag = BI_Sub_zxy(&result_sub, num0, num1);
    
    // 1.1 뺄셈 함수 연산 실패 시, 실패 메시지 출력 및 프로그램 종료
    if(sub_flag != FUNC_SUCCESS) {
        puts("============= Sub Fail ==============");
        return 0;
    } 
    // 1.2 뺄셈 결과 값 출력
    printf("A - B = ");     bi_print_bigint_hex(result_sub);    printf("\n");

    // 2. BIGINT 변수 삭제
    bi_delete(&result_sub);

    // -------------------------------------------------------------------------------
    // Function 3. 곱셈
    // -------------------------------------------------------------------------------
    // - 입력: (곱셈 결과를 저장할 BIGINT 구조체 더블 포인터, 입력 BIGINT 포인터 num0, 입력 BIGINT 포인터 num1)
    // - 출력: SUCCESS or ERROR CODE (int 변수로 반환값을 저장한 뒤, 에러 코드를 처리할 추가 코드 작성 필요)
    // -------------------------------------------------------------------------------
    
    // 0. BIGINT 변수를 NULL로 초기화하여 선언
    BIGINT* result_mul = NULL;

    // 1. 곱셈 연산
    printf("\n=============  곱셈 검증 예시  ============== \n");
    int mul_flag = BI_Mul_zxy(&result_mul, num0, num1);
    
    // 1.1 곱셈 함수 연산 실패 시, 실패 메시지 출력 및 프로그램 종료
    if(mul_flag != FUNC_SUCCESS) {
        puts("============= Mul Fail ==============");
        return 0;
    } 
    // 1.2 곱셈 결과 값 출력
    printf("A * B = ");     bi_print_bigint_hex(result_mul);    printf("\n");

    // 2. BIGINT 변수 삭제
    bi_delete(&result_mul);

    // -------------------------------------------------------------------------------
    // Function 4. 나눗셈
    // -------------------------------------------------------------------------------
    // - 입력: (나눗셈 몫을 저장할 BIGINT 구조체 더블 포인터, 나눗셈 나머지를 저장할 BIGINT 구조체 더블 포인터, 입력 BIGINT 포인터 num0 나눠지는 수, 입력 BIGINT 포인터 num1 나눌 수)
    // - 출력: SUCCESS or ERROR CODE (int 변수로 반환값을 저장한 뒤, 에러 코드를 처리할 추가 코드 작성 필요)
    // -------------------------------------------------------------------------------
    
    // 0. BIGINT 변수를 NULL로 초기화하여 선언
    BIGINT* q = NULL;
    BIGINT* r = NULL;    

    // 1. 나눗셈 연산
    printf("\n=============  나눗셈 검증 예시  ============== \n");
    int div_flag = BI_Div_zxy(&q, &r, num0, num1);
    
    // 1.1 나눗셈 함수 연산 실패 시, 실패 메시지 출력 및 프로그램 종료
    if(div_flag != FUNC_SUCCESS) {
        puts("============= Div Fail ==============");
        return 0;
    } 
    // 1.2 나눗셈 결과 값 출력
    printf("몫 q = ");     bi_print_bigint_hex(q);     printf("\n");
    printf("나머지 r = ");     bi_print_bigint_hex(r);     printf("\n");

    // 2. BIGINT 변수 삭제
    bi_delete(&q);
    bi_delete(&r);

    // -------------------------------------------------------------------------------
    // Function 5. 제곱
    // -------------------------------------------------------------------------------
    // - 입력: (제곱 결과를 저장할 BIGINT 구조체 더블 포인터, 입력 BIGINT 포인터 num0)
    // - 출력: SUCCESS or ERROR CODE (int 변수로 반환값을 저장한 뒤, 에러 코드를 처리할 추가 코드 작성 필요)
    // -------------------------------------------------------------------------------
    
    // 0. BIGINT 변수를 NULL로 초기화하여 선언
    BIGINT* result_sqr = NULL;

    // 1. 제곱 연산
    printf("\n=============  제곱 검증 예시  ============== \n");
    int sqr_flag = BI_Sqr_zx(&result_sqr, num0);

    // 1.1 제곱 함수 연산 실패 시, 실패 메시지 출력 및 프로그램 종료
    if(sqr_flag != FUNC_SUCCESS) {
        puts("============= Sqr Fail ==============");
        return 0;
    } 
    // 1.2 제곱 결과 값 출력
    printf("A * A = ");     bi_print_bigint_hex(result_sqr);    printf("\n");

    // 2. BIGINT 변수 삭제
    bi_delete(&result_sqr);

    // -------------------------------------------------------------------------------
    // Function 6. 모듈러
    // -------------------------------------------------------------------------------
    // - 입력: (모듈러 결과를 저장할 BIGINT 구조체 더블 포인터, 입력 BIGINT 포인터 num0 나눠지는 수, 입력 BIGINT 포인터 num1 나눌 수)
    // - 출력: SUCCESS or ERROR CODE (int 변수로 반환값을 저장한 뒤, 에러 코드를 처리할 추가 코드 작성 필요)
    // -------------------------------------------------------------------------------
    
    // 0. BIGINT 변수를 NULL로 초기화하여 선언
    BIGINT* result_mod = NULL;

    // 1. 모듈러 연산
    printf("\n=============  모듈러 검증 예시  ============== \n");
    int mod_flag = BI_Mod_zxy(&result_mod, num0, num1);

    // 1.1 모듈러 함수 연산 실패 시, 실패 메시지 출력 및 프로그램 종료
    if(mod_flag != FUNC_SUCCESS) {
        puts("============= Mod Fail ==============");
        return 0;
    } 
    // 1.2 모듈러 결과 값 출력
    printf("나머지 r = ");     bi_print_bigint_hex(result_mod);     printf("\n");

    // 2. BIGINT 변수 삭제
    bi_delete(&result_mod);

    // -------------------------------------------------------------------------------
    // Function 6. 모듈러 - Fast Reduction: Barrett Reduction
    // -------------------------------------------------------------------------------
    // BI_Barret_Reduction
    // - 입력: (모듈러 결과를 저장할 BIGINT 구조체 더블 포인터, 입력 BIGINT 포인터 num0 나눠지는 수, 입력 BIGINT 포인터 num1 나눌 수)
    // - 출력: SUCCESS or ERROR CODE (int 변수로 반환값을 저장한 뒤, 에러 코드를 처리할 추가 코드 작성 필요)
    // - 주의사항
    //      1. BI_Barret_Reduction 함수 연산 이전에, 사전 계산 필요.
    //      2. 나눠지는 수의 워드 길이 <= (나누는 수의 워드 길이 * 2)를 만족하는 경우에만 사용 가능
    //      3. 일반적으로 모듈러 지수승에서, 조건 2를 만족할 때 일반 모듈러 연산 대신 사용.
    //          - 일반 모듈러 연산 함수 BI_Mod_zxy보다 항상 속도가 빠르다고 할 수는 없다.
    // -------------------------------------------------------------------------------
    
    // 0. BIGINT 변수를 NULL로 초기화하여 선언
    BIGINT* T = NULL;
    BIGINT* result_barr = NULL;

    // 1. 사전 계산 Pre-Computation
    bi_BR_pre_computed(&T, num1);

    // 2. 모듈러 (Fast Alg. Barrett Reduction) 연산
    printf("\n=============  모듈러 (Barrett Reduction) 검증 예시  ============== \n");
    int barr_flag = BI_Barret_Reduction(&result_barr, num0, num1, T);

    // 2.1 모듈러 함수 연산 실패 시, 실패 메시지 출력 및 프로그램 종료
    if(barr_flag != FUNC_SUCCESS) {
        puts("============= Mod (Barrett Reduction) Fail ==============");
        return 0;
    } 
    // 2.2 모듈러 결과 값 출력
    printf("나머지 r = ");     bi_print_bigint_hex(result_barr);    printf("\n");

    // 3. BIGINT 변수 삭제
    bi_delete(&T);
    bi_delete(&result_barr);

    // -------------------------------------------------------------------------------
    // Function 7. 모듈러 지수승
    // -------------------------------------------------------------------------------
    // result = num0 ^ num1 mod num2
    // - 입력: (모듈러 지수승 결과를 저장할 BIGINT 구조체 더블 포인터, 입력 BIGINT 포인터 num0, 입력 BIGINT 포인터 num1, 입력 BIGINT 포인터 num2)
    // - 출력: SUCCESS or ERROR CODE (int 변수로 반환값을 저장한 뒤, 에러 코드를 처리할 추가 코드 작성 필요)
    // - 주의사항
    //      1. 모듈러 지수승은 이전의 연산과는 달리 입력 값이 3개이다.
    // -------------------------------------------------------------------------------
    
    // 0.0 BIGINT 변수를 NULL로 초기화하여 선언
    BIGINT* result_exp_mod = NULL;
    BIGINT* num2 = NULL;

    // 0.1. 입력값이 3개이므로, 예제를 위해 3번째 변수 할당
    int set_flag_2 = bi_set_by_string(&num2, NON_NEGATIVE, "a1b2c3d4e5f6a7b8c9d0", HEXDECIMAL);
    
    // 0.2 유효하지 않은 입력이 발생할 경우 오류 메시지 출력 및 프로그램 종료
    if(set_flag_2 == INVALID_CHAR_ERROR) {
        puts("============= Input 3 : wrong! ==============");
        return 0;
    } 

    // 1. 모듈러 지수승 연산
    printf("\n=============  모듈러 지수승 검증 예시  ============== \n");
    int exp_mod_flag = BI_ExpMod_zx(&result_exp_mod, num0, num1, num2);

    // 1.1 모듈러 지수승 함수 연산 실패 시, 실패 메시지 출력 및 프로그램 종료
    if(exp_mod_flag != FUNC_SUCCESS) {
        puts("============= Exp Mod Fail ==============");
        return 0;
    } 
    // 1.2 예제를 위해 세번째 변수 값 출력
    printf("C = "); bi_print_bigint_hex(num2);  printf("\n");
    // 1.3 모듈러 결과 값 출력
    bi_print_bigint_hex(result_exp_mod);    printf("\n");

    // 2. BIGINT 변수 삭제
    bi_delete(&num2);
    bi_delete(&result_exp_mod);


    // -------------------------------------------------------------------------------
    // (line 8) 2. main 함수 제일 마지막에 Set해준 BIGINT 변수 삭제
    // -------------------------------------------------------------------------------
    bi_delete(&num0);
    bi_delete(&num1);

    return 0;
}