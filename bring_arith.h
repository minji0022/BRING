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
int BI_Sqr_zx(BIGINT** bi_dst, BIGINT* bi_src1); // z <- x^2 제곱 함수
int BI_Exp_zx(BIGINT** bi_dst, BIGINT* bi_src, int n); // z = x^n 지수승 함수
int BI_ExpMul_zx(BIGINT** bi_dst, BIGINT* bi_src, int n); // z = n*x 스칼라 곱셈 함수
int BI_Div_zxy(BIGINT** bi_quotient, BIGINT* bi_remainder, BIGINT* bi_src1, BIGINT* bi_src2); // z*q + r = x / y 나눗셈 함수

//################################################################################################# 
//                            유형 2. 연산대상 빅인티저에 연산 결과를 갱신하는 연산 함수
//#################################################################################################
// !!!! 갱신형 연산 함수들은 필요시 구현할 예정.
int BI_Add_xy(BIGINT* bi_src1, BIGINT* bi_src2); // x <- x+y 덧셈 함수 
int BI_Sub_xy(BIGINT* bi_src1, BIGINT* bi_src2); // x <- x-y 뺄셈 함수
int BI_Mul_xy(BIGINT* bi_src1, BIGINT* bi_src2); // x <- x*y 곱셈 함수
int BI_Sqr_x(BIGINT* bi_src1); // x <- x^2 제곱 함수
int BI_Exp_x(BIGINT* bi_src, int n); // x = x^n 지수승 함수
int BI_ExpMul_x(BIGINT* bi_src, int n); // x = n*x 스칼라 곱셈 함수


//===============================================================================================//
//                                       개발자용 함수 구역(코어 함수)
//                                  사용자는 직접 사용하지 않는 함수
//
// !!!! 우선 유형 1. 새로운 빅인티저에 연산 결과를 저장하는 연산 함수 기준으로 코어 함수 생성
//################################################################################################# 
//                                           덧셈 관련 함수 
//#################################################################################################
int bi_Add_w(word* dst_bData, word src1_Data, word src2_Data, int carry_in); /**** 단일 워드 덧셈 ****/
void bi_Add_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2); /**** 다중 워드 덧셈 ****/
//################################################################################################# 
//                                           뺄셈 관련 함수 
//#################################################################################################
int bi_Sub_w(word* dst_bData, word src1_Data, word src2_Data, int carry_in); /**** 단일 워드 뺄셈 ****/
void bi_Sub_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2); /**** 다중 워드 뺄셈 ****/
//################################################################################################# 
//                                           곱셈 관련 함수 
//#################################################################################################
unsigned long long bi_Mul_w(word src1_Data, word src2_Data); /**** 단일 워드 곱셈 ****/
void bi_Mul_Schoolbook_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2); /**** 스쿨북 곱셈 ****/
void bi_Mul_PS_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2); /**** PS 곱셈 ****/
//################################################################################################# 
//                                           제곱 관련 함수 
//#################################################################################################
unsigned long long bi_Sqr_w(word src1_Data); /**** 단일 워드 제곱 ****/
void bi_Sqr_PS_zx(BIGINT** bi_dst, BIGINT* bi_src1); /**** PS 제곱 ****/
//################################################################################################# 
//                                           나눗셈 관련 함수 
//#################################################################################################
word bi_longdiv(word a1, word a0, word b);
void bi_Divcc_zxy(BIGINT** bi_quotient, BIGINT** bi_remainder, BIGINT* bi_src1, BIGINT* bi_src2);
void bi_Divc_zxy(BIGINT** bi_quotient, BIGINT** bi_remainder, BIGINT* bi_src1, BIGINT* bi_src2);
//################################################################################################# 
//                                           shift 관련 함수 
//#################################################################################################
void bi_Shift_R2L_r_words(BIGINT* bi_src, int r); /**** A << r words ****/
void bi_Shift_L2R_r_words(BIGINT* bi_src, int r); /**** A >> r words ****/
void bi_Shift_R2L_r_bits_x(BIGINT* bi_src, int r); /**** A << r bits ****/
void bi_Shift_L2R_r_bits_x(BIGINT* bi_src, int r); /**** A >> r bits ****/
void bi_Shift_R2L_r_bits_zx(BIGINT** bi_dst, BIGINT* bi_src, int r); /**** A << r bits ****/
void bi_Shift_L2R_r_bits_zx(BIGINT** bi_dst, BIGINT* bi_src, int r); /**** A >> r bits ****/
// !!!!! 배열만 받아서 시프트하는 연산도 구현필요할듯
//################################################################################################# 
//                                           지수승 관련 함수 
//#################################################################################################
void bi_Exp_L2R_zx(BIGINT** bi_dst, BIGINT* bi_src, int n); /**** Left-to-Right exponentiation ****/
void bi_Exp_MnS_zx(BIGINT** bi_dst, BIGINT* bi_src, int n); /**** Multiply-and-Squaring exponentiation ****/
void bi_ExpMul_L2R_zx(BIGINT** bi_dst, BIGINT* bi_src, int n); /**** Left-to-Right exponentiation mul  ****/
void bi_ExpMul_MnS_zx(BIGINT** bi_dst, BIGINT* bi_src, int n); /**** Multiply-and-Squaring exponentiation mul ****/
//################################################################################################# 
//                                          모듈러 관련 함수 
//#################################################################################################
void bi_Barret_Reduction(BIGINT** bi_result, BIGINT* bi_src1, BIGINT* bi_N, BIGINT* bi_T);