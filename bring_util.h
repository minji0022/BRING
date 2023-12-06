#include "bring_def.h"

//################################################################################################# 
//                                  BIGINT 생성 및 소멸관련 함수
//#################################################################################################

/**
 * @brief BIGINT 생성
 * @param[out] bi_dst wordlen의 길이인 큰 정수 메모리 공간 생성
 * @param[in] wordlen 워드 길이 설정
 * @note 부호는 NON_NEGATIVE로 설정
*/
void bi_new(BIGINT** bi_dst, int wordlen); /**** 빅인티저 초기화 함수 ****/ 

/**
 * @brief BIGINT 소멸
 * @param[in] bi_src 입력 BIGINT 삭제
*/
void bi_delete(BIGINT** bi_src); /**** 빅인티저 소멸 함수 ****/

/**
 * @brief 16진수 문자열을 입력 받아 BIGINT set
 * @param[out] bi_dst 출력
 * @param[in] sign 입력 부호
 * @param[in] str 입력 문자열
 * @param[in] base 입력 진수 - 16진수 HEX만 지원
*/
int bi_set_by_string(BIGINT** bi_dst, int sign, char* str, int base); /**** hex 문자열 -> 빅인티저 함수 ****/

/**
 * @brief BIGINT 상위 워드에 0이 채워져 있는 경우, 이를 제거하는 함수
 * @param[in] bi_src 입력
*/
void bi_refine(BIGINT* bi_src); /**** 불필요한 0 지우는 함수 ****/

//################################################################################################# 
//                                  BIGINT 구조체를 활용하는 유틸 함수
//#################################################################################################

/**
 * @brief BIGINT를 16진수로 출력
 * @param[in] bi_src 입력
*/
void bi_print_bigint_hex(BIGINT* bi_src); /**** 빅인티저 출력 함수 ****/ 

/**
 * @brief BIGINT 할당 함수 (bi_dst = bi_src)
 * @param[out] bi_dst 출력
 * @param[in] bi_src 입력
*/
void bi_assign(BIGINT** bi_dst, BIGINT* bi_src); /**** bi_dst <- bi_src Copy Biginteger 부호 유지 ****/

/**
 * @brief BIGINT 부호만 반대로 할당하는 함수 (bi_dst = -bi_src)
 * @param[out] bi_dst 출력
 * @param[in] bi_src 입력
*/
void bi_assign_flip_sign(BIGINT** bi_dst, BIGINT* bi_src); /**** bi_dst <- bi_src Copy Biginteger 부호 변경 ****/

/**
 * @brief 유효한 16진수 값인지 확인
 * @return TRUE or ERR_INVALID_INPUT
*/
int is_valid_hex(char* str); /**** 올바르지 못한 16진수 값 들어오는지 판단 ****/ // 유효하지 않으면 에러 리턴.

/**
 * @brief 배열 복사 함수
*/
void array_copy(word* p_dst, word* p_src, int wordlen); /**** 데이터 배열을 Copy 부호 복사x ****/

/**
 * @brief 설정된 길이의 BIGINT를 랜덤하게 설정하는 함수
 * @param[out] bi_dst 출력
 * @param[in] sign 입력 부호
 * @param[in] wordlen 입력 길이
*/
void bi_gen_rand(BIGINT** bi_src, int sign, int wordlen); /**** 랜덤한 빅인티저 생성 ****/

/**
 * @brief 배열에 랜덤한 값을 할당하는 함수
*/
void array_rand(word* dst, int wordlen); /*** 랜덤한 배열 생성 ****/

/**
 * @brief 0으로 초기화
 * @param[in] bi_src 입력
*/
void bi_set_zero(BIGINT** bi_src); /**** bi_src를 0으로 세팅하는 함수 ****/

/**
 * @brief 1로 초기화
 * @param[in] bi_src 입력
*/
void bi_set_one(BIGINT** bi_src); /**** bi_src를 1로 세팅하는 함수 ****/

/**
 * @brief 0인지 확인 
 * @param[in] bi_src 입력
 * @return 0이라면 1을, 아니라면 0을 반환
*/
int bi_is_zero(BIGINT* bi_src); /**** 빅인티저가 0인지 판별하는 함수 ****/ // true 1, false 0 리턴

/**
 * @brief 1인지 확인 
 * @param[in] bi_src 입력
 * @return 1이라면 1을, 아니라면 0을 반환
*/
int bi_is_one(BIGINT* bi_src); /**** 빅인티저가 1인지 판별하는 함수 ****/ // true 1, false 0 리턴

/**
 * @brief 절댓값 비교 함수
 * @param[in] bi_src1 입력
 * @param[in] bi_src2 입력
 * @return |bi_src1| > |bi_src2|이라면 1을, |bi_src1| < |bi_src2|라면 -1을, |bi_src1| = |bi_src2|라면 0을 반환
*/
int bi_compare_ABS(BIGINT* bi_src1, BIGINT* bi_src2); /**** 두개의 빅인티저 절대값 비교 함수 ****/

/**
 * @brief 크기 비교 함수
 * @param[in] bi_src1 입력
 * @param[in] bi_src2 입력
 * @return bi_src1 > bi_src2이라면 1을, bi_src1 < bi_src2라면 -1을, bi_src1 = bi_src2라면 0을 반환
*/
int bi_compare_bigint(BIGINT* bi_src1, BIGINT* bi_src2); /**** 두개의 빅인티저 값 비교 함수 ****/ 

/**
 * @brief 비트 길이를 반환하는 함수
 * @param[in] bi_src 입력
 * @return 비트 길이
*/
int bi_get_bit_length(BIGINT* bi_src); /**** 빅인티저 비트 길이 반환 함수 ****/ 

/**
 * @brief 워드 길이를 반환하는 함수
 * @param[in] bi_src 입력
 * @return 워드 길이
*/
int bi_get_word_length(BIGINT* bi_src); /**** 빅인티저 워드 길이 반환 함수 ****/ 

/**
 * @brief 길이 비교 함수
 * @param[in] bi_src1 입력
 * @param[in] bi_src2 입력
 * @return bi_src1->wordlen > bi_src2->wordlen이라면 1을, bi_src1->wordlen < bi_src2->wordlen라면 -1을, bi_src1->wordlen = bi_src2->wordlen라면 0을 반환
*/
int bi_compare_length(BIGINT* bi_src1, BIGINT* bi_src2); /**** 두개의 빅인티저 길이 비교 함수 ****/

/**
 * @brief 더 큰 BIGINT의 워드 길이를 반환하는 함수
 * @param[in] bi_src1 입력
 * @param[in] bi_src2 입력
 * @return 워드 길이
*/
int bi_get_max_length(BIGINT* bi_src1, BIGINT* bi_src2); /**** 두개의 빅인티저 길이 중 긴 값 리턴 함수 ****/ 

/**
 * @brief 더 작은 BIGINT의 워드 길이를 반환하는 함수
 * @param[in] bi_src1 입력
 * @param[in] bi_src2 입력
 * @return 워드 길이
*/
int bi_get_min_length(BIGINT* bi_src1, BIGINT* bi_src2); /**** 두개의 빅인티저 길이 중 짧은 값 리턴 함수 ****/ 

/**
 * @brief 절댓값이 1인지 확인하는 함수
 * @param[in] bi_src 입력
 * @return 절댓값이 1이면 1, 아니면 0 리턴
*/
int bi_abs_is_one(BIGINT* bi_src); /**** 빅인티저의 절댓값이 1인지 판별하는 함수 ****/ // true 1, false 0 리턴

/**
 * @brief int 의 비트 길이를 반환하는 함수
 * @param[in] n 입력
 * @return n의 비트 길이
*/
int bi_length_of_n(int n); /**** 정수 n의 비트 길이 리턴하는 함수 ****/

/**
 * @brief 원하는 워드 길이가 될 때까지 상위 워드에 0을 채우는 함수
 * @param[in] bi_src 입력
 * @param[in] len 입력 워드 길이
*/
void bi_fill_zero(BIGINT* bi_src, int len); /**** len만큼 앞을 0으로 채우는 함수 ****/

/**
 * @brief dst = src mod 2^r
 * @param[out] bi_dst 출력
 * @param[in] bi_src 입력
 * @param[in] r 입력
*/
void reductionOf2(BIGINT** bi_dst, BIGINT* bi_src, int r); /**** A mod 2^r 형태. ****/
