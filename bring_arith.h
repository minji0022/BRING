#include "bring_util.h"
//===============================================================================================//
//                                       사용자용 함수 구역
//                                     사용자가 직접 사용할 함수
//
//################################################################################################# 
//                            유형 1. 새로운 빅인티저에 연산 결과를 저장하는 연산 함수
//#################################################################################################
int BI_Add_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2); // z <- x+y 덧셈 함수 
int BI_Sub_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2); // z <- x-y 뺄셈 함수
int BI_Mul_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2); // z <- x*y 곱셈 함수
int BI_Sqr_zx(BIGINT** bi_dst, BIGINT* bi_src); // z <- x^2 제곱 함수
int BI_Div_zxy(BIGINT** bi_quotient, BIGINT** bi_remainder, BIGINT* bi_src1, BIGINT* bi_src2);
int BI_Mod_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2); // z <- x % y 모듈러 함수
int BI_ExpMod_zx(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2, BIGINT* bi_M);  // z = x^n 지수승 함수

//################################################################################################# 
//                            유형 2. 연산대상 빅인티저에 연산 결과를 갱신하는 연산 함수
//#################################################################################################
// !!!! 갱신형 연산 함수들은 필요시 구현할 예정.
int BI_Add_xy(BIGINT** bi_src1, BIGINT* bi_src2); // x <- x+y 덧셈 함수 





//===============================================================================================//
//                                       개발자용 함수 구역(코어 함수)
//                                  사용자는 직접 사용하지 않는 함수
//
//################################################################################################# 
//                                           덧셈 관련 함수 
//#################################################################################################
// bi_Add_w의 리턴 : 캐리
int bi_Add_w(word* p_dst, word p_src1, word p_src2, int carry_in); /**** 단일 워드 덧셈 ****/ 
void bi_Add_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2); /**** 다중 워드 덧셈 ****/
//################################################################################################# 
//                                           뺄셈 관련 함수 
//#################################################################################################
// bi_Sub_w의 리턴 : 캐리
int bi_Sub_w(word* p_dst, word p_src1, word p_src2, int borrow_in); /**** 단일 워드 뺄셈 ****/
void bi_Sub_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2); /**** 다중 워드 뺄셈 ****/
//################################################################################################# 
//                                           곱셈 관련 함수 
//#################################################################################################
void bi_Mul_w(BIGINT** bi_dst, word p_src1, word p_src2); /**** 단일 워드 곱셈 ****/
void bi_Mul_Schoolbook_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2); /**** 스쿨북 곱셈 ****/
//################################################################################################# 
//                                           제곱 관련 함수 
//#################################################################################################
void bi_Sqr_w(BIGINT** bi_dst, word p_src1); /**** 단일 워드 제곱 ****/
void bi_Sqrc_zy(BIGINT** bi_dst, BIGINT* bi_src1); /**** 다중 워드 제곱 ****/
//################################################################################################# 
//                                           나눗셈 관련 함수 
//#################################################################################################
word bi_longdiv(word a1, word a0, word b);
void bi_Divcc_zxy(BIGINT** bi_quotient, BIGINT** bi_remainder, BIGINT* bi_src1, BIGINT* bi_src2);
void bi_Divc_zxy(BIGINT** bi_quotient, BIGINT** bi_remainder, BIGINT* bi_src1, BIGINT* bi_src2);
//################################################################################################# 
//                                           shift 관련 함수 
//#################################################################################################
void bi_left_word_shift(BIGINT* bi_src, int r); /**** A << r words ****/
void bi_right_word_shift(BIGINT* bi_src, int r); /**** A >> r words ****/
void bi_left_bit_shift_zx(BIGINT** bi_dst, BIGINT* bi_src, int r); /**** A << r bits ****/
void bi_right_bit_shift_zx(BIGINT** bi_dst, BIGINT* bi_src, int r); /**** A >> r bits ****/
//################################################################################################# 
//                                           지수승 관련 함수 
//#################################################################################################
void bi_Exp_L2R_zx(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2, BIGINT* bi_M); /**** Left-to-Right exponentiation ****/
void bi_Exp_MnS_zx(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2, BIGINT* bi_M); /**** Multiply-and-Square exponetiation****/
//################################################################################################# 
//                                          Fast_reduction 관련 함수 
//#################################################################################################
// !!!! bi_T를 사전 계산해주는 함수 필요함.
void bi_Barret_Reduction(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_N, BIGINT* bi_T);
