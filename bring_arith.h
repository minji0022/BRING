#include "bring_util.h"
//===============================================================================================//
//                                       사용자용 함수 구역
//                                     사용자가 직접 사용할 함수
//
//################################################################################################# 
//                            유형 1. 새로운 빅인티저에 연산 결과를 저장하는 연산 함수
//#################################################################################################
/**
* @details [덧셈 함수] 두 큰 정수(BIGINT)를 더하는 연산
* @param[out] bi_dst BIGINT 구조체 더블 포인터 덧셈 결과 (= src1 + src2)
* @param[in] bi_src1 BIGINT 구조체 포인터 입력 src1
* @param[in] bi_src2 BIGINT 구조체 포인터 입력 src2
* @return Success or Error Code
*/
int BI_Add_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2); 

/**
* @details 두 큰 정수(BIGINT)를 빼는 연산
* @param[out] bi_dst BIGINT 구조체 더블 포인터 뺄셈 결과 (= src1 - src2)
* @param[in] bi_src1 BIGINT 구조체 포인터 입력 src1
* @param[in] bi_src2 BIGINT 구조체 포인터 입력 src2
* @return Success or Error Code
*/
int BI_Sub_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2); 

/**
* @details [곱셈 함수] 두 큰 정수(BIGINT)를 곱하는 연산
* @param[out] bi_dst BIGINT 구조체 더블 포인터 곱셈 결과 (= src1 * src2)
* @param[in] bi_src1 BIGINT 구조체 포인터 입력 src1
* @param[in] bi_src2 BIGINT 구조체 포인터 입력 src2
* @return Success or Error Code
*/
int BI_Mul_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2); 

/**
* @details [제곱 함수] 큰 정수(BIGINT)를 제곱하는 연산
* @param[out] bi_dst BIGINT 구조체 더블 포인터 제곱 결과 (= src * src)
* @param[in] bi_src BIGINT 구조체 포인터 입력 src
* @return Success or Error Code
*/
int BI_Sqr_zx(BIGINT** bi_dst, BIGINT* bi_src);

/** 
 * @brief BI_Div_zxy 함수는 두 큰 정수를 나누는 연산을 수행
 * @details 이 함수는 두 큰 정수를 나누어 몫과 나머지를 계산
 * @param[out] bi_quotient - 나누기의 결과로 얻어진 몫이 저장될 BIGINT 포인터의 포인터
 * @param[out] bi_remainder - 나누기의 결과로 얻어진 나머지가 저장될 BIGINT 포인터의 포인터
 * @param[in] bi_src1 - 나누어질 큰 정수인 BIGINT 구조체
 * @param[in] bi_src2 - 나누는 큰 정수인 BIGINT 구조체
 * @return 성공 시 FUNC_SUCCESS를 반환하고, 오류가 발생한 경우 오류 코드를 반환
 */
int BI_Div_zxy(BIGINT** bi_quotient, BIGINT** bi_remainder, BIGINT* bi_src1, BIGINT* bi_src2);

/** 
 * @brief [모듈러 함수] BI_Mod_zxy 함수는 큰 정수의 모듈러 연산을 수행
 * @details 이 함수는 큰 정수를 다른 큰 정수로 나눈 나머지를 계산
 * @param[out] bi_dst - 모듈러 연산의 결과로 얻어진 나머지가 저장될 BIGINT 포인터의 포인터
 * @param[in] bi_src1 - 나누어질 큰 정수인 BIGINT 구조체
 * @param[in] bi_src2 - 나누는 큰 정수인 BIGINT 구조체
 * @return 성공 시 FUNC_SUCCESS를 반환하고, 오류가 발생한 경우 오류 코드를 반환
 */
int BI_Mod_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2);

/**
* @brief [모듈러 지수승 함수] src1 ^ src2 mod M
* @param[out] bi_dst BIGINT 구조체 더블 포인터 모듈러 지수승 결과 (= src1 ^ src2 mod M)
* @param[in] bi_src1 BIGINT 구조체 포인터 입력 src1
* @param[in] bi_src2 BIGINT 구조체 포인터 입력 src2
* @param[in] bi_M BIGINT 구조체 포인터 입력 M
* @return Success or Error Code
*/
int BI_ExpMod_zx(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2, BIGINT* bi_M);

/**
* @brief [Fast reduction function] src mod N 모듈러 연산
* @param[out] bi_dst BIGINT 구조체 더블 포인터 모듈러 결과 (= src mod N)
* @param[in] bi_src BIGINT 구조체 포인터 입력 src
* @param[in] bi_N BIGINT 구조체 포인터 입력 N 
* @param[in] bi_T BIGINT 구조체 포인터 입력 사전 계산 값 T
* @return Success or Error Code
* @note 함수 사용 전 사전 계산 값 bi_T를 계산하는 함수 bi_BR_pre_computed를 실행해야 함.
* 1) bi_BR_pre_computed
* 2) BI_Barret_Reduction
* @note bi_src->wordlen <= (bi_N->wordlen * 2)를 만족해야 사용 가능
*/
int BI_Barret_Reduction(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_N, BIGINT* bi_T);

/**
 * @brief Precomputation Function for Barrett reduction
 * @param[out] bi_T BIGINT 구조체 더블 포인터 출력 T 
 * @param[in] bi_N BIGINT 구조체 포인터 입력 N 
 * @details bi_T = WORD^(2 * bi_N->wordlen) / bi_N
*/
void bi_BR_pre_computed(BIGINT** bi_T, BIGINT* bi_N);

//################################################################################################# 
//                            유형 2. 연산대상 빅인티저에 연산 결과를 갱신하는 연산 함수
//#################################################################################################
/**
* @details [덧셈 갱신 함수] 두 큰 정수(BIGINT)를 더하는 연산 (갱신: src1 += src2)
* @param[in,out] bi_src1 BIGINT 구조체 더블 포인터 입력 src1, 출력 src1 = src1 + src2
* @param[in] bi_src2 BIGINT 구조체 포인터 입력 src2
* @return Success or Error Code
*/
int BI_Add_xy(BIGINT** bi_src1, BIGINT* bi_src2);

/**
* @details [갱신 함수] 두 큰 정수(BIGINT)를 빼는 연산
* @param[in, out] bi_src1 BIGINT 구조체 더블 포인터 입력, 출력 src1 (= src1 - src2)
* @param[in] bi_src2 BIGINT 구조체 포인터 입력 src2
* @return Success or Error Code
*/
int BI_Sub_xy(BIGINT** bi_src1, BIGINT* bi_src2);

/**
* @details [곱셈 갱신 함수] 두 큰 정수(BIGINT)를 곱하는 연산
* @param[in, out] bi_src1 BIGINT 구조체 더블 포인터 입력, 출력 src1 (= src1 * src2)
* @param[in] bi_src2 BIGINT 구조체 포인터 입력 src2
* @return Success or Error Code
*/
int BI_Mul_xy(BIGINT** bi_src1, BIGINT* bi_src2);

/**
* @details [제곱 갱신 함수] 큰 정수(BIGINT)를 제곱하는 연산 
* @param[in, out] bi_src BIGINT 구조체 더블 포인터 입력, 출력 src (= src * src)
* @return Success or Error Code
*/
int BI_Sqr_x(BIGINT** bi_src); // x <- x^2  

/** 
 * @brief [모듈러 갱신 함수] BI_Mod_zxy 함수는 큰 정수의 모듈러 연산을 수행
 * @details 이 함수는 큰 정수를 다른 큰 정수로 나눈 나머지를 계산
 * @param[in, out] bi_src1 - 나누어질 큰 정수인 BIGINT 구조체 더블 포인터
 * @param[in] bi_src2 - 나누는 큰 정수인 BIGINT 구조체 포인터
 * @return 성공 시 FUNC_SUCCESS를 반환하고, 오류가 발생한 경우 오류 코드를 반환
 */
int BI_Mod_xy(BIGINT** bi_src1, BIGINT* bi_src2);

//===============================================================================================//
//                                       개발자용 함수 구역(코어 함수)
//                                  사용자는 직접 사용하지 않는 함수
//
//################################################################################################# 
//                                           덧셈 관련 함수 
//#################################################################################################
/**
* @details 단일 워드 덧셈 Single Word Addition
* @param[out] p_dst 단일 워드 덧셈 결과 (= src1 + src2 + carry_in)
* @param[in] p_src1 입력 src1
* @param[in] p_src2 입력 src2
* @param[in] carry_in 입력 carry
* @return int carry_out
* @note bi_Add_w의 리턴 : carry
*/
int bi_Add_w(word* p_dst, word p_src1, word p_src2, int carry_in);

/**
* @details 다중 워드 덧셈 Multi Word Addition
* @param[out] bi_dst BIGINT 구조체 더블 포인터 다중 워드 덧셈 결과 (= src1 + src2)
* @param[in] bi_src1 BIGINT 구조체 포인터 입력 src1
* @param[in] bi_src2 BIGINT 구조체 포인터 입력 src2
* @note src1, src2 모두 양수로 보고 계산, src1의 길이가 src2의 길이보다 크거나 같도록 함.
*/
void bi_Add_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2);


//################################################################################################# 
//                                           뺄셈 관련 함수 
//#################################################################################################
/**
* @details 단일 워드 뺄셈 Single Word Subtraction
* @param[out] p_dst 단일 워드 뻴셈 결과 (= src1 - src2 - borrow_in)
* @param[in] p_src1 입력 src1
* @param[in] p_src2 입력 src2
* @param[in] borrow_in 입력 borrow
* @return int borrow_out
* @note bi_Sub_w 리턴 : borrow. src1의 길이가 src2의 길이보다 크거나 같도록 함.
*/
int bi_Sub_w(word* p_dst, word p_src1, word p_src2, int borrow_in);


/**
* @details 다중 워드 뺄셈 Multi Word Subtraction
* @param[out] bi_dst BIGINT 구조체 더블 포인터 다중 워드 뺄셈 결과 (= src1 - src2)
* @param[in] bi_src1 BIGINT 구조체 포인터 입력 src1
* @param[in] bi_src2 BIGINT 구조체 포인터 입력 src2
* @note src1, src2 모두 양수로 보고 계산, src1의 길이가 src2의 길이보다 크거나 같도록 함.
*/
void bi_Sub_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2);


//################################################################################################# 
//                                           곱셈 관련 함수 
//#################################################################################################
/**
* @details 단일 워드 곱셈
* @param[out] bi_dst BIGINT 구조체 더블 포인터 단일 워드 곱셈 결과
* @param[in] p_src1 입력 src1
* @param[in] p_src2 입력 src2
*/
void bi_Mul_w(BIGINT** bi_dst, word p_src1, word p_src2);

/**
* @details schoolbook 곱셈
* @param[out] bi_dst BIGINT 구조체 더블 포인터 다중 워드 곱셈 결과 (= src1 * src2)
* @param[in] bi_src1 BIGINT 구조체 포인터 입력 src1
* @param[in] bi_src2 BIGINT 구조체 포인터 입력 src2
*/
void bi_Mul_Schoolbook_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2);

/**
* @details Karatsuba 곱셈
* @param[out] bi_dst BIGINT 구조체 더블 포인터 다중 워드 곱셈 결과 (= src1 * src2)
* @param[in] bi_src1 BIGINT 구조체 포인터 입력 src1
* @param[in] bi_src2 BIGINT 구조체 포인터 입력 src2
* @param[in] flag schoolbook 곱셈 워드 단위 지정 Flag
*/
void bi_Mul_Karatsuba(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2, int flag);


//################################################################################################# 
//                                           제곱 관련 함수 
//#################################################################################################
/**
* @details 단일 워드 제곱
* @param[out] bi_dst BIGINT 구조체 더블 포인터 단일 워드 제곱 결과 (= src1 * src1)
* @param[in] p_src1 입력 src1
*/
void bi_Sqr_w(BIGINT** bi_dst, word p_src1);

/**
* @details 다중 워드 제곱
* @param[out] bi_dst BIGINT 구조체 더블 포인터 다중 워드 제곱 결과 (= src1 * src1)
* @param[in] bi_src1 BIGINT 구조체 포인터 입력 src1
*/
void bi_Sqrc_zy(BIGINT** bi_dst, BIGINT* bi_src1);


//################################################################################################# 
//                                           나눗셈 관련 함수 
//#################################################################################################
/**
 * @brief bi_longdiv 함수는 두 워드의 나눗셈을 수행
 * @details 이 함수는 두 워드의 나눗셈 연산을 수행하여 몫을 반환
 * @param[in] a1 - 높은 자리 워드
 * @param[in] a0 - 낮은 자리 워드
 * @param[in] b - 나눌 수
 * @return 나눗셈의 몫을 반환
 */
word bi_longdiv(word a1, word a0, word b);

/**
 * @brief bi_Divcc_zxy 함수는 큰 정수의 몫과 나머지를 계산
 * @details 이 함수는 큰 정수를 다른 큰 정수로 나눈 몫과 나머지를 계산
 * @param[out] bi_quotient - 나누기의 결과로 얻어진 몫이 저장될 BIGINT 포인터의 포인터
 * @param[out] bi_remainder - 나누기의 결과로 얻어진 나머지가 저장될 BIGINT 포인터의 포인터
 * @param[in] bi_src1 - 나누어질 큰 정수인 BIGINT 구조체
 * @param[in] bi_src2 - 나누는 큰 정수인 BIGINT 구조체
 */
void bi_Divcc_zxy(BIGINT** bi_quotient, BIGINT** bi_remainder, BIGINT* bi_src1, BIGINT* bi_src2);

/**
 * @brief bi_Divc_zxy 함수는 큰 정수의 모듈러 연산을 수행
 * @details 이 함수는 큰 정수를 다른 큰 정수로 나눈 몫과 나머지를 계산
 * @param[out] bi_quotient - 나누기의 결과로 얻어진 몫이 저장될 BIGINT 포인터의 포인터
 * @param[out] bi_remainder - 나누기의 결과로 얻어진 나머지가 저장될 BIGINT 포인터의 포인터
 * @param[in] bi_src1 - 나누어질 큰 정수인 BIGINT 구조체. (bi_src1)의 길이는 m+1
 * @param[in] bi_src2 - 나누는 큰 정수인 BIGINT 구조체. (bi_src2)의 길이는 m 이 되게끔 넣어줌.
 */
void bi_Divc_zxy(BIGINT** bi_quotient, BIGINT** bi_remainder, BIGINT* bi_src1, BIGINT* bi_src2);


//################################################################################################# 
//                                           shift 관련 함수 
//#################################################################################################
/**
 * @brief [갱신 함수] 왼쪽 워드 시프트 연산
 * @param[in, out] bi_src BIGINT 구조체 포인터 입력, 출력 bi_src
 * @param[in] r 이동할 워드 수(정수) 입력
 *  src = src << (r * WORD_BIT_SIZE)
 * @note 왼쪽 비트 시프트 연산 따로 존재
*/
void bi_left_word_shift(BIGINT* bi_src, int r);

/**
 * @brief [갱신 함수] 오른쪽 워드 시프트 연산
 * @param[in,out] bi_src BIGINT 구조체 포인터 입력, 출력 bi_src
 * @param[in] r 이동할 워드 수(정수) 입력
 *  src = src >> (r * WORD_BIT_SIZE)
 * @note 오른쪽 비트 시프트 연산 따로 존재
*/
void bi_right_word_shift(BIGINT* bi_src, int r);

/**
 * @brief 왼쪽 워드 시프트 연산
 * @param[out] bi_dst BIGINT 구조체 더블 포인터 출력 bi_dst
 * @param[in] bi_src BIGINT 구조체 포인터 입력 bi_src
 * @param[in] r 이동할 워드 수(정수) 입력
 *  dst = src << (r * WORD_BIT_SIZE)
 * @note 왼쪽 비트 시프트 연산 따로 존재
*/
void bi_left_word_shift_zx(BIGINT** bi_dst, BIGINT* bi_src, int r);

/**
 * @brief 오른쪽 워드 시프트 연산
 * @param[out] bi_dst BIGINT 구조체 더블 포인터 출력 bi_dst
 * @param[in] bi_src BIGINT 구조체 포인터 입력 bi_src
 * @param[in] r 이동할 워드 수(정수) 입력
 *  dst = src >> (r * WORD_BIT_SIZE)
 * @note 오른쪽 비트 시프트 연산 따로 존재
*/
void bi_right_word_shift_zx(BIGINT** bi_dst, BIGINT* bi_src, int r);

/**
 * @brief 왼쪽 비트 시프트 연산
 * @param[out] bi_dst BIGINT 구조체 더블 포인터 출력 bi_dst
 * @param[in] bi_src BIGINT 구조체 포인터 입력 bi_src
 * @param[in] r 이동할 비트 수(정수) 입력
 *  dst = src << r
 * @note 왼쪽 워드 시프트 연산 따로 존재
*/
void bi_left_bit_shift_zx(BIGINT** bi_dst, BIGINT* bi_src, int r);

/**
 * @brief 오른쪽 비트 시프트 연산
 * @param[out] bi_dst BIGINT 구조체 더블 포인터 출력 bi_dst
 * @param[in] bi_src BIGINT 구조체 포인터 입력 bi_src
 * @param[in] r 이동할 비트 수(정수) 입력
 *  dst = src >> r
 * @note 오른쪽 워드 시프트 연산 따로 존재
*/
void bi_right_bit_shift_zx(BIGINT** bi_dst, BIGINT* bi_src, int r);


//################################################################################################# 
//                                           지수승 관련 함수 
//#################################################################################################
/**
 * @brief [모듈러 지수승 함수] Left-to-Right Algorithm.
 * @param[out] bi_dst BIGINT 구조체 더블 포인터 출력 bi_dst
 * @param[in] bi_src1 BIGINT 구조체 포인터 입력 bi_src1 - 밑
 * @param[in] bi_src2 BIGINT 구조체 포인터 입력 bi_src2 - 지수
 * @param[in] bi_M BIGINT 구조체 포인터 입력 bi_M - 모듈러 값
 * dst = src1 ^ src2 mod M
 * @note 부채널 공격에 취약 (실행 시간이 Constant가 아님)
*/
void bi_Exp_L2R_zx(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2, BIGINT* bi_M);

/**
 * @brief [모듈러 지수승 함수] Muliply-and-Squaring Algorithm.
 * @param[out] bi_dst BIGINT 구조체 더블 포인터 출력 bi_dst
 * @param[in] bi_src1 BIGINT 구조체 포인터 입력 bi_src1 - 밑
 * @param[in] bi_src2 BIGINT 구조체 포인터 입력 bi_src2 - 지수
 * @param[in] bi_M BIGINT 구조체 포인터 입력 bi_M - 모듈러 값
 * dst = src1 ^ src2 mod M
 * @note 부채널 공격에 취약하지 않지만, bi_Exp_L2R_zx과 비교하여 속도가 느림.
*/
void bi_Exp_MnS_zx(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2, BIGINT* bi_M);
