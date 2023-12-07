// -------------------------------------------------------------------------------
//  BRING의 에러 코드 확인
// ------------------------- [ DEFINITION OF ERROR CODE ] -------------------------
// #define FUNC_SUCCESS               100 // 함수 정상 작동
// #define FUNC_ERROR                 101 // 함수 이상 작동
// #define NULL_POINTER_ERROR         102 // 널 포인터 에러.
// #define SIZE_ERROR                 103 // 사이즈 에러.
// #define INVALID_CHAR_ERROR         104 // 잘못된 문자열 입력 에러.
// #define ERR_INVALID_INPUT          105 // 잘못된 입력 에러.
// -------------------------------------------------------------------------------

#include "../bring_arith.h"

int main(){
    // -------------------------------------------------------------------------------
    // FUNC_SUCCESS 100 - 함수 정상 작동
    // -------------------------------------------------------------------------------
    // 덧셈 갱신 함수 BI_Add_xy 예제
    // -------------------------------------------------------------------------------    
    
    // 0. BIGINT 변수를 NULL로 초기화하여 선언
    BIGINT* num1 = NULL;        BIGINT* num2 = NULL;

    // 1. 예시를 위해 5워드 음이 아닌 정수 num1, 음의 정수 num2를 랜덤한 수 생성
    bi_gen_rand(&num1, NON_NEGATIVE, 5);
    bi_gen_rand(&num2, NEGATIVE, 5);

    // 2. 예시를 위해 덧셈 갱신 함수 실행
    int func_flag = BI_Add_xy(&num1, num2);
    
    // 3. FUNC_SUCCESS Check !
    if(func_flag == FUNC_SUCCESS) {
        printf(" >> FUNC_SUCCESS \n\n");
    }

    // 4. BIGINT 변수 삭제
    bi_delete(&num1);       bi_delete(&num2);

    // -------------------------------------------------------------------------------
    // FUNC_ERROR 101 - 함수 이상 작동
    // -------------------------------------------------------------------------------
    // 제공 연산 중, 오류 발생 시 출력되는 값
    // -------------------------------------------------------------------------------    
    

    // -------------------------------------------------------------------------------
    // NULL_POINTER_ERROR 102 - 널 포인터 에러.
    // -------------------------------------------------------------------------------
    // 제공 연산 중, 오류 발생 시 출력되는 값
    // line 53 - 67 주석 해제 후, 오류 확인 가능. NULL_POINTER_ERROR는 segmentation fault 유발.
    // -------------------------------------------------------------------------------    
    
    // // 0. BIGINT 변수를 NULL로 초기화하여 선언
    // num1 = NULL;        num1 = NULL;

    // // 1. 예시를 위해 널 포인터 그대로 입력
    
    // // 2. 예시를 위해 덧셈 갱신 함수 실행
    // func_flag = BI_Add_xy(&num1, num2);
    
    // // 3. NULL_POINTER_ERROR Check !
    // if(func_flag == NULL_POINTER_ERROR) {
    //     printf(" >> NULL_POINTER_ERROR \n\n");
    // }

    // // 4. BIGINT 변수 삭제
    // bi_delete(&num1);       bi_delete(&num2);

    // -------------------------------------------------------------------------------
    // SIZE_ERROR 103 - 사이즈 에러
    // -------------------------------------------------------------------------------
    // BI_Barret_Reduction 연산 중, 입력 워드 길이가 조건을 만족하지 않는 경우 오류 발생
    // 만족 조건: num1->wordlen <= (num2->wordlen * 2)
    // -------------------------------------------------------------------------------    
    
    // 0. BIGINT 변수를 NULL로 초기화하여 선언
    num1 = NULL;        num1 = NULL;
    BIGINT* dst = NULL;        BIGINT* T = NULL;

    // 1. 예시를 위해 15워드 음이 아닌 정수 num1, 5워드 음이 아닌 정수 num2를 랜덤한 수 생성
    bi_gen_rand(&num1, NON_NEGATIVE, 15);
    bi_gen_rand(&num2, NON_NEGATIVE, 5);
    
    // 2. Barrett Reduction의 사전 계산 함수
    bi_BR_pre_computed(&T, num1);

    // 2. 예시를 위해 덧셈 갱신 함수 실행
    func_flag = BI_Barret_Reduction(&dst, num1, num2, T);
    
    // 3. SIZE_ERROR Check !
    if(func_flag == SIZE_ERROR) {
        printf(" >> SIZE_ERROR \n\n");
    }

    // 4. BIGINT 변수 삭제
    bi_delete(&num1);      bi_delete(&num2);
    bi_delete(&dst);       bi_delete(&T);

    // -------------------------------------------------------------------------------
    // INVALID_CHAR_ERROR 104 - 잘못된 문자열 입력 에러
    // -------------------------------------------------------------------------------
    // bi_set_by_string 연산 중, 유효하지 않은 문자열이 입력된 경우 오류 발생
    // 유효한 문자: 0, 1, 2, ..., 8, 9, a, b, c, d, e, f.
    // 주의사항: 공백도 유효하지 않은 문자열로 처리.
    // -------------------------------------------------------------------------------    
    
    // 0. BIGINT 변수를 NULL로 초기화하여 선언
    num1 = NULL;

    // 1. 예시를 위해 음이 아닌 정수 num1 생성
    func_flag = bi_set_by_string(&num1, NON_NEGATIVE, "TEAM PINOKIO", HEXDECIMAL);
    
    // 2. INVALID_CHAR_ERROR Check !
    if(func_flag == INVALID_CHAR_ERROR) {
        printf(" >> INVALID_CHAR_ERROR \n\n");
    }

    // 3. BIGINT 변수 삭제
    bi_delete(&num1);  

    // -------------------------------------------------------------------------------
    // ERR_INVALID_INPUT 105 - 잘못된 입력 에러
    // -------------------------------------------------------------------------------
    // BI_Div_zxy, BI_ExpMod_zx, BI_Barret_Reduction 연산 중, 음의 정수 입력 시 오류 발생
    // -------------------------------------------------------------------------------    
    
    // 0. BIGINT 변수를 NULL로 초기화하여 선언
    BIGINT* q = NULL;       BIGINT* r = NULL;
    num1 = NULL;    num1 = NULL;

    // 1. 예시를 위해 5워드 음의 정수 num1, 5워드 음이 아닌 정수 num2를 랜덤한 수 생성
    bi_gen_rand(&num1, NEGATIVE, 5);
    bi_gen_rand(&num2, NON_NEGATIVE, 5);

    // 1. 예시를 위해 음이 아닌 정수 num1 생성
    func_flag = BI_Div_zxy(&q, &r, num1, num2);
    
    // 2. INVALID_CHAR_ERROR Check !
    if(func_flag == ERR_INVALID_INPUT) {
        printf(" >> ERR_INVALID_INPUT \n\n");
    }

    // 3. BIGINT 변수 삭제
    bi_delete(&num1);       bi_delete(&num2);  
    bi_delete(&q);          bi_delete(&r);  


}