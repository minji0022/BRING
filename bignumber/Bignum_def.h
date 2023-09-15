//// 빅인티저 구조체 정의하는 헤더파일 ////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//################################################################################################# 
//                                  BIGINT에 사용되는 정의들
//#################################################################################################

/************** BIGINT 부호 **************/
#define NON_NEGATIVE    0
#define NEGATIVE        1

/************** BIGINT 단위 **************/
typedef unsigned int word;

/************** 함수들의 작동 여부 확인을 위한 definitions **************/
#define FUNC_SUCCESS               0 // 함수 정상 작동
#define FUNC_ERROR                 -1 // 함수 이상 작동
#define POINTER_ERROR              1 // 이미 사용중인 포인터 
#define NULL_POINTER_ERROR         2 // 널 포인터 에러.
#define SIZE_ERROR                 3 // 덧, 뺄셈 사이즈 에러.
#define INVALID_CHAR_ERROR         4 // 잘못된 문자열 들어갔을 때 생기는 에러.

/************** BIGINT 구조체 정의 **************/
typedef struct{
    int sign; // NEGATIVE or NON_NEGATIVE
    int wordlen; // 배열 A의 길이
    word* bData; // 배열 A의 주소
} BIGINT;


