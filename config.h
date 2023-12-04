
// ------------------------------------------------------------
// 기본 설정 - 워드 단위 설정
// ------------------------------------------------------------
/**
 * @brief 워드 단위 설정
 * @note If WORD == 0, then "typedef unsigned char word;"
 * Elif WORD == 1, then "typedef unsigned int word;"
 * Elif WORD == 2, then "typedef unsigned long long word;"
*/
#define WORD 2

// ------------------------------------------------------------
// 검증
// ------------------------------------------------------------
/**
 * @brief 검증 모드(연산) 설정
 * @test ${0: Verify ADD(+) Operation
 * @test ${1: Verify SUB(-) Operation
 * @test ${2: Verify MUL(*) Operation
 * @test ${3: Verify DIV(/) Operation
 * @test ${4: Verify MOD(%) Operation
 * @test ${5: Verify SQR(**) Operation
 * @test ${6: Verify EXP_MOD Operation
 * @test ${7: Verify Fast REDUCTION Operation
 */
#define VERIFY_MODE 2

/**
 * @brief 연산 구현 정확성 검증 / 성능 측정(속도 측정) 설정
 * 0: 연산 검증  
 * 1: 연산 검증 & 성능 측정
 */
#define BENCHMARK_FLAG 1

/**
 * @brief 검증시 반복 횟수 (BENCHMARK_FLAG를 1로 설정 시, 성능 측정 반복 횟수가 됨)
 * @note If BENCHMARK_FLAG == 1, then recommend REPEAT_COUNT >= 1000.
*/
#define REPEAT_COUNT 1000

/**
 * @brief 검증시 입력 1(Src1)의 워드 길이
 * @note SRC1_WORD_SIZE < MAX_BINT_LEN(=64)
*/
#define SRC1_WORD_SIZE 60

/**
 * @brief 검증시 입력 1(Src1)의 부호 정보 
 * @note NON_NEGATIVE / NEGATIVE
*/
#define SRC1_SIGN NON_NEGATIVE

/**
 * @brief 검증시 입력 2(Src2)의 워드 길이
 * @note SRC2_WORD_SIZE < MAX_BINT_LEN(=64)
*/
#define SRC2_WORD_SIZE 45

/**
 * @brief 검증시 입력 2(Src2)의 부호 정보 
 * @note NON_NEGATIVE / NEGATIVE
*/
#define SRC2_SIGN NON_NEGATIVE
