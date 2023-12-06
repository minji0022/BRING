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
void bi_new(BIGINT** bi_dst, int wordlen);

/**
 * @brief BIGINT 소멸
 * @param[in] bi_src 입력 BIGINT 삭제
*/
void bi_delete(BIGINT** bi_src);

/**
 * @brief 16진수 문자열을 입력 받아 BIGINT set
 * @param[out] bi_dst 출력
 * @param[in] sign 입력 부호
 * @param[in] str 입력 문자열
 * @param[in] base 입력 진수 - 16진수 HEX만 지원
*/
int bi_set_by_string(BIGINT** bi_dst, int sign, char* str, int base);

/**
 * @brief BIGINT 상위 워드에 0이 채워져 있는 경우, 이를 제거하는 함수
 * @param[in] bi_src 입력
*/
void bi_refine(BIGINT* bi_src);

//################################################################################################# 
//                                  BIGINT 구조체를 활용하는 유틸 함수
//#################################################################################################

/**
 * @brief BIGINT를 16진수로 출력
 * @param[in] bi_src 입력
*/
void bi_print_bigint_hex(BIGINT* bi_src);

/**
 * @brief BIGINT 할당 함수 (bi_dst = bi_src)
 * @param[out] bi_dst 출력
 * @param[in] bi_src 입력
*/
void bi_assign(BIGINT** bi_dst, BIGINT* bi_src);

/**
 * @brief BIGINT 부호만 반대로 할당하는 함수 (bi_dst = -bi_src)
 * @param[out] bi_dst 출력
 * @param[in] bi_src 입력
*/
void bi_assign_flip_sign(BIGINT** bi_dst, BIGINT* bi_src);

/**
 * @brief 유효한 16진수 값인지 확인
 * @return TRUE or ERR_INVALID_INPUT
*/
int is_valid_hex(char* str);

/**
 * @brief 배열 복사 함수
*/
void array_copy(word* p_dst, word* p_src, int wordlen);

/**
 * @brief 설정된 길이의 BIGINT를 랜덤하게 설정하는 함수
 * @param[out] bi_dst 출력
 * @param[in] sign 입력 부호
 * @param[in] wordlen 입력 길이
*/
void bi_gen_rand(BIGINT** bi_src, int sign, int wordlen);

/**
 * @brief 배열에 랜덤한 값을 할당하는 함수
*/
void array_rand(word* dst, int wordlen);

/**
 * @brief 0으로 초기화
 * @param[in] bi_src 입력
*/
void bi_set_zero(BIGINT** bi_src);

/**
 * @brief 1로 초기화
 * @param[in] bi_src 입력
*/
void bi_set_one(BIGINT** bi_src); 

/**
 * @brief 0인지 확인 
 * @param[in] bi_src 입력
 * @return 0이라면 1을, 아니라면 0을 반환
*/
int bi_is_zero(BIGINT* bi_src);

/**
 * @brief 1인지 확인 
 * @param[in] bi_src 입력
 * @return 1이라면 1을, 아니라면 0을 반환
*/
int bi_is_one(BIGINT* bi_src);

/**
 * @brief 절댓값 비교 함수
 * @param[in] bi_src1 입력
 * @param[in] bi_src2 입력
 * @return |bi_src1| > |bi_src2|이라면 1을, |bi_src1| < |bi_src2|라면 -1을, |bi_src1| = |bi_src2|라면 0을 반환
*/
int bi_compare_ABS(BIGINT* bi_src1, BIGINT* bi_src2); 

/**
 * @brief 크기 비교 함수
 * @param[in] bi_src1 입력
 * @param[in] bi_src2 입력
 * @return bi_src1 > bi_src2이라면 1을, bi_src1 < bi_src2라면 -1을, bi_src1 = bi_src2라면 0을 반환
*/
int bi_compare_bigint(BIGINT* bi_src1, BIGINT* bi_src2);

/**
 * @brief 비트 길이를 반환하는 함수
 * @param[in] bi_src 입력
 * @return 비트 길이
*/
int bi_get_bit_length(BIGINT* bi_src);

/**
 * @brief 워드 길이를 반환하는 함수
 * @param[in] bi_src 입력
 * @return 워드 길이
*/
int bi_get_word_length(BIGINT* bi_src);

/**
 * @brief 길이 비교 함수
 * @param[in] bi_src1 입력
 * @param[in] bi_src2 입력
 * @return bi_src1->wordlen > bi_src2->wordlen이라면 1을, bi_src1->wordlen < bi_src2->wordlen라면 -1을, bi_src1->wordlen = bi_src2->wordlen라면 0을 반환
*/
int bi_compare_length(BIGINT* bi_src1, BIGINT* bi_src2);

/**
 * @brief 더 큰 BIGINT의 워드 길이를 반환하는 함수
 * @param[in] bi_src1 입력
 * @param[in] bi_src2 입력
 * @return 워드 길이
*/
int bi_get_max_length(BIGINT* bi_src1, BIGINT* bi_src2);

/**
 * @brief 더 작은 BIGINT의 워드 길이를 반환하는 함수
 * @param[in] bi_src1 입력
 * @param[in] bi_src2 입력
 * @return 워드 길이
*/
int bi_get_min_length(BIGINT* bi_src1, BIGINT* bi_src2);

/**
 * @brief 절댓값이 1인지 확인하는 함수
 * @param[in] bi_src 입력
 * @return 절댓값이 1이면 1, 아니면 0 리턴
*/
int bi_abs_is_one(BIGINT* bi_src); 

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
void bi_fill_zero(BIGINT* bi_src, int len);

/**
 * @brief dst = src mod 2^r
 * @param[out] bi_dst 출력
 * @param[in] bi_src 입력
 * @param[in] r 입력
*/
void reductionOf2(BIGINT** bi_dst, BIGINT* bi_src, int r);
