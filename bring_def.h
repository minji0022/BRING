//// 빅인티저 구조체 정의하는 헤더파일 ////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "config.h"

//################################################################################################# 
//                            에러 코드 정의 DEFINITION OF ERROR CODE
//#################################################################################################

// DEFINITION OF ERROR CODE 
#define FUNC_SUCCESS               100 // 함수 정상 작동
#define FUNC_ERROR                 101 // 함수 이상 작동
#define NULL_POINTER_ERROR         102 // 널 포인터 에러.
#define SIZE_ERROR                 103 // 사이즈 에러.
#define INVALID_CHAR_ERROR         104 // 잘못된 문자열 입력 에러.
#define ERR_INVALID_INPUT          105 // 잘못된 입력 에러.

//################################################################################################# 
//                                  BIGINT에 사용되는 정의들
//#################################################################################################

// BIGINT 부호 정의
#define NON_NEGATIVE    0
#define NEGATIVE        1

typedef unsigned long long ull;
typedef unsigned char byte;

// word : unsigned char (w=8) , unsigned short : (w=16), unsigned int (w=32), unsigned long long(w=64)
#if WORD == 0
    typedef unsigned char word;
    #define MAX_OF_WORD 0xff
    #define WORD_SIZE_CHECK 0x80
    #define SHIFT_SIZE 3
    #define KARA_FLAG 32
#elif WORD == 1
    typedef unsigned int word;
    #define MAX_OF_WORD 0xffffffff
    #define WORD_SIZE_CHECK 0x80000000
    #define SHIFT_SIZE 5
    #define KARA_FLAG 4
#elif WORD == 2
    typedef unsigned long long word;
    #define MAX_OF_WORD 0xffffffffffffffff
    #define WORD_SIZE_CHECK 0x8000000000000000
    #define SHIFT_SIZE 6
    #define KARA_FLAG 4
#endif

// 진수 정의
#define BINARY              2
#define DECIMAL             10
#define HEXDECIMAL          16

#define WORD_BYTE_SIZE  (sizeof(word)) // 워드의 바이트 사이즈 
#define WORD_BIT_SIZE   (sizeof(word)*8) // 워드의 비트 사이즈 bit length of a word (w ∈ {8, 32, 64})

#define TRUE                       1
#define FALSE                      0

//################################################################################################# 
//                                    BIGINT 구조체 정의
//#################################################################################################

// BIGINT 구조체 정의
typedef struct{
    int sign; // 부호
    int wordlen; // 배열 A의 길이
    word* p; // 배열 A의 주소
} BIGINT;
