//// 빅인티저 구조체 정의하는 헤더파일 ////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//################################################################################################# 
//                                  BIGINT에 사용되는 정의들
//#################################################################################################

/************** BIGINT 부호 **************/
// !!!! 이전 라이브러리에서 ZERO 부호를 사용하지 않고 wordlen == 0으로 0 판별했었음
// !!!! 교수님 강의자료에서는 길이가 1이고 데이터 영역에 0으로 채워져있으면 0임. --> 우리는 이 방식 사용할 예정
#define NON_NEGATIVE    0
#define NEGATIVE        1

// 2048bit = 256byte...
// !!!! word 하나당 4바이트 저장하니까 2048비트까지 저장하려면 MAX_BINT_LEN : 64로 하면됨.
// 정적 메모리 할당 방식에서 사용하던 값 : 배열의 길이.
#define MAX_BINT_LEN 64

/************** 단위 **************/
// !!!! 나중에 사용자가 word를 선택할 수 있게 해야하는데 어떻게 해야할까??????
//word : unsigned char (w=8) , unsigned short : (w=16), unsigned int (w=32), unsigned long long(w=64)
#define WORD 0 // !!!! --> 나중에 이부분 수정해야함.
typedef unsigned long long ull;
typedef unsigned char byte;

#if WORD == 0
    typedef unsigned char word;
#elif WORD == 1
    typedef unsigned int word;
#elif WORD == 2
    typedef unsigned long long word;
#endif

/**
 * @brief SET VERIFY_MODE
 * @test ${0: Verify ADD(+) Operation
 * @test ${1: Verify SUB(-) Operation
 * @test ${2: Verify MUL(*) Operation
 * @test ${3: Verify DIV(/) Operation
 * @test ${4: Verify MOD(%) Operation
 * @test ${5: Verify SQR(**) Operation
 * @test ${6: Verify EXP_MOD Operation
 * @test ${7: Verify Fast REDUCTION Operation
 */
#define VERIFY_MODE 5
#define REPEAT_COUNT 1000

#define BINARY          2
#define DECIMAL         10
#define HEXDECIMAL      16
#define WORD_BYTE_SIZE (sizeof(word)) // 워드의 바이트 사이즈 // 1,2,4,8 바이트 가능함.
#define WORD_BIT_SIZE (sizeof(word)*8) // 워드의 비트 사이즈 bit length of a word (w ∈ {8, 16, 32, 64})

/************** 함수들의 작동 여부 확인을 위한 definitions **************/
#define TRUE                       1
#define FALSE                      0
#define FUNC_SUCCESS               100 // 함수 정상 작동
#define FUNC_ERROR                 101 // 함수 이상 작동
#define POINTER_ERROR              102 // 이미 사용중인 포인터 
#define NULL_POINTER_ERROR         103 // 널 포인터 에러.
#define SIZE_ERROR                 104 // 사이즈 에러.
#define INVALID_CHAR_ERROR         105 // 잘못된 문자열 들어갔을 때 생기는 에러.
#define ERR_INVALID_INPUT          106 // 잘못된 인풋 들어간 경우.

/************** BIGINT 구조체 정의 **************/

typedef struct{
    int sign; // 부호
    int wordlen; // 배열 A의 길이
    word* p; // 배열 A의 주소
} BIGINT;


