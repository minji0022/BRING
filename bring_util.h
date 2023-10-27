#include "bring_def.h"

//################################################################################################# 
//                                  BIGINT 생성 및 소멸관련 함수
//#################################################################################################
// 강의 자료에서 bi_new = bi_new
void bi_new(BIGINT** bi_dst, int wordlen); /**** 빅인티저 초기화 함수 ****/ 
void bi_delete(BIGINT** bi_src); /**** 빅인티저 소멸 함수 ****/
int bi_set_by_string(BIGINT** bi_dst, int sign, char* str, int base); /**** hex 문자열 -> 빅인티저 함수 ****/
void bi_refine(BIGINT* bi_src); /**** 불필요한 0 지우는 함수 ****/

//################################################################################################# 
//                                  BIGINT 구조체를 활용하는 유틸 함수
//#################################################################################################
/* ///// 크기 비교 함수의 경우 src1이 크거나 길면 1, 같으면 0, 작으면 -1 리턴 ///// */
// 강의 자료에서 bi_show_hex = bi_print_bigint_hex
void bi_print_bigint_hex(BIGINT* bi_src); /**** 빅인티저 출력 함수 ****/ 
void bi_assign(BIGINT** bi_dst, BIGINT* bi_src); /**** bi_dst <- bi_src Copy Biginteger 부호 유지 ****/
void bi_assign_flip_sign(BIGINT** bi_dst, BIGINT* bi_src); /**** bi_dst <- bi_src Copy Biginteger 부호 변경 ****/
int is_valid_hex(char* str); /**** 올바르지 못한 16진수 값 들어오는지 판단 ****/ // 유효하지 않으면 에러 리턴.

// word** dst_p, word* src_p, int wordlen라 해야하나
void array_copy(word* p_dst, word* p_src, int wordlen); /**** 데이터 배열을 Copy 부호 복사x ****/
void bi_gen_rand(BIGINT** bi_src, int sign, int wordlen); /**** 랜덤한 빅인티저 생성 ****/
void array_rand(word* dst, int wordlen); /*** 랜덤한 배열 생성 ****/
void bi_set_zero(BIGINT** bi_src); /**** bi_src를 0으로 세팅하는 함수 ****/
void bi_set_one(BIGINT** bi_src); /**** bi_src를 1로 세팅하는 함수 ****/
int bi_is_zero(BIGINT* bi_src); /**** 빅인티저가 0인지 판별하는 함수 ****/ // true 1, false 0 리턴
int bi_is_one(BIGINT* bi_src); /**** 빅인티저가 1인지 판별하는 함수 ****/ // true 1, false 0 리턴
int bi_compare_ABS(BIGINT* bi_src1, BIGINT* bi_src2); /**** 두개의 빅인티저 절대값 비교 함수 ****/
int bi_compare_bigint(BIGINT* bi_src1, BIGINT* bi_src2); /**** 두개의 빅인티저 값 비교 함수 ****/ 
int bi_get_bit_length(BIGINT* bi_src); /**** 빅인티저 비트 길이 반환 함수 ****/ 
int bi_get_word_length(BIGINT* bi_src); /**** 빅인티저 워드 길이 반환 함수 ****/ 
int bi_compare_length(BIGINT* bi_src1, BIGINT* bi_src2); /**** 두개의 빅인티저 길이 비교 함수 ****/
int bi_get_max_length(BIGINT* bi_src1, BIGINT* bi_src2); /**** 두개의 빅인티저 길이 중 긴 값 리턴 함수 ****/ 
int bi_abs_is_one(BIGINT* bi_src); /**** 빅인티저의 절댓값이 1인지 판별하는 함수 ****/ // true 1, false 0 리턴
int bi_length_of_n(int n); /**** 정수 n의 비트 길이 리턴하는 함수 ****/
void bi_fill_zero(BIGINT* bi_src, int len); /**** len만큼 앞을 0으로 채우는 함수 ****/
/**** 이 밑부분은 아직 구현 안함 ****/
// void bi_mod(BIGINT** bi_dst, BIGINT* bi_src, int r);
// void bi_reset_bigint(BIGINT* bi_src); /**** 빅인티저 데이터 전체 리셋하는 함수 ****/

