#include "Bignum_util.h"
//################################################################################################# 
//                                  BIGINT 생성 및 소멸관련 함수
//#################################################################################################

/**** 길이 정보를 받아서 데이터를 초기화하는 빅인티저 생성 함수 ****/
void bi_create(BIGINT** bi_Dst, int wordlen)
{
    *bi_Dst = (BIGINT*)malloc(sizeof(BIGINT));

    (*bi_Dst)->sign = NON_NEGATIVE; // NON_NEGATIVE로 초기화
    (*bi_Dst)->wordlen = wordlen;
    (*bi_Dst)->bData = (word*)calloc(wordlen, sizeof(word));

}

/**** 빅인티저 소멸 함수 ****/
int bi_destroy_bigint(BIGINT** bi_Src)
{
    if((*bi_Src) == NULL) return NULL_POINTER_ERROR;

    free((*bi_Src)->bData); // 빅인티저 구조체의 Addr 주소에 있는 데이터 값 비우기. 즉, 실제 수들 비우기
    free(*bi_Src); // 빅인티저 구조체 메모리 해제
    *bi_Src = NULL;

    return FUNC_SUCCESS;
}

/**** 16진수 문자열 -> 빅인티저 구조체 함수 ****/
int bi_set_by_string(BIGINT** bi_Dst, int sign, char* str)
{
    int str_len = strlen(str); // 입력된 문자열의 길이 계산
    int word_len = (str_len + 7) / 8; // 문자열 길이에 따라서 필요한 워드 개수 계산
    // 현재 word(unsigned int)가 4바이트니 8자리씩 끊어서 워드 하나 할당으로 함. 
    // 8 대신 word와 관련있는 표현으로 바꿔보기 ex) 2*sizeof(word)

    // 빅인티저 생성
    bi_create(bi_Dst, word_len);
    (*bi_Dst)->sign = sign;    // 빅인티저의 부호 설정

    // 문자열을 16진수에서 정수로 변환하여 빅인티저 배열에 저장하는 단계
    int index = str_len - 1; // index : 전체 문자열의 index. 끝부터 시작

    // 문자열 길이 및 워드길이 확인용
    printf("str_len : %d /// ", str_len);
    printf("word_len : %d\n", word_len);

    // 문자를 8개씩 처리하여 unsigned int 정수로 변환
    // 가장 오른쪽 워드(낮은 값)부터 작업
    // 즉, 받은 전체 문자열이 6606BC465BF5ABCD일 때 5BF5ABCD 먼저 처리, 이후 6606BC46 처리하는 방식.
    for (int i = 0; i < word_len - 1; i++) { 
        word value = 0; // 현재 처리중인 16진수 숫자 누적할 변수. 
        // ex) 5BF5ABCD에서 CD처리 중이면 CD값을 정수로 변환하여 value에 넣을 것임.
        int digits_to_read = 8; // 16진수 문자를 8개(4바이트)씩 처리할 것임. 
        // 즉, 받은 전체 문자열이 6606BC465BF5ABCD일 때 6606BC46 5BF5ABCD 단위로 처리.

        for (int j = 0; j < digits_to_read; j++) {
            char c = str[index];
            int digit_value;

            if (c >= '0' && c <= '9') {
                digit_value = c - '0';
            } else if (c >= 'A' && c <= 'F') {
                digit_value = c - 'A' + 10;
            } else if (c >= 'a' && c <= 'f') {
                digit_value = c - 'a' + 10;
            } else {
                // 예외 처리: 유효하지 않은 문자
                bi_destroy_bigint(bi_Dst);
                return INVALID_CHAR_ERROR;
            }

            value |= (digit_value << (j * 4)); // 4비트씩 왼쪽으로 쉬프트하면서 값 계산
            index--;
        }

        (*bi_Dst)->bData[i] = value;
        //데이터 잘 들어가는지 확인용 코드
        printf("bData[%d] : %08X /// ", i, (*bi_Dst)->bData[i]);
    }

    // 마지막 워드 부분 처리.
    word value = 0; 
    int digits_to_read = 8; 

    for (int j = 0; j < digits_to_read; j++) {

        if (index < 0){ // index 0 보다 작은 경우 : 문자열 끝난 경우
            value |= (0 << (j * 4));
        }

        else{
            char c = str[index];
            int digit_value;

            if (c >= '0' && c <= '9') {
                digit_value = c - '0';
            } else if (c >= 'A' && c <= 'F') {
                digit_value = c - 'A' + 10;
            } else if (c >= 'a' && c <= 'f') {
                digit_value = c - 'a' + 10;
            } else {
                // 예외 처리: 유효하지 않은 문자
                bi_destroy_bigint(bi_Dst);
                return INVALID_CHAR_ERROR;
            }
            value |= (digit_value << (j * 4)); // 4비트씩 왼쪽으로 쉬프트하면서 값 계산
        }
        index--;
    }

    (*bi_Dst)->bData[word_len-1] = value;
    //데이터 잘 들어가는지 확인용 코드
    //printf("bData[last] : %08X ", (*bi_Dst)->bData[word_len-1]);
    //printf("\n");
    return FUNC_SUCCESS;
}

/**** 불필요한 0 지우는 함수****/
int bi_refine(BIGINT* bi_Src){
    if (bi_Src == NULL)
        return NULL_POINTER_ERROR; 

    int new_wordlen = bi_Src->wordlen;
    while (new_wordlen > 1){
        if (bi_Src->bData[new_wordlen-1] != 0) break; // 0이 아니면 지우지 않음
        new_wordlen--;
    }
    // 0인 word가 존재하여 지워진 경우
    if (new_wordlen != bi_Src->wordlen){
        bi_Src->wordlen = new_wordlen;
        bi_Src->bData = (word*)realloc(bi_Src->bData, sizeof(word)*new_wordlen);
    }
    // 0인 경우
    if ((bi_Src->wordlen == 1) && (bi_Src->bData[0] == 0x0))
        bi_Src->sign = NON_NEGATIVE;
    
    return FUNC_SUCCESS;

}
///////////////////////////////////////////////////////////////////////////////////////////////////

//################################################################################################# 
//                                  BIGINT 구조체를 활용하는 유틸 함수
//#################################################################################################
/**** 빅인티저 복사 함수 ****/ 
// BI_Src를 BI_Dst에 복사. BI_Dst <- BI_Src Copy Biginteger
// 부호 유지
int bi_copy_bigint(BIGINT* bi_Dst, BIGINT* bi_Src){
    if(bi_Dst == NULL) {
        printf("카피 오류 : 포인터 없음");
        return NULL_POINTER_ERROR; // 포인터 에러 
    }

    bi_Dst->wordlen = bi_Src->wordlen;
    bi_Dst->sign = bi_Src->sign;

    for (int i = 0; i < bi_Src->wordlen; i++){
        bi_Dst->bData[i] = bi_Src->bData[i];
    }

    printf("복사 결과 [bi_Dst] : ");
    bi_print_bigint(bi_Dst);
    
    return FUNC_SUCCESS;

}
// 부호 변경
int bi_copy2_bigint(BIGINT* bi_Dst, BIGINT* bi_Src){
    if(bi_Dst == NULL) {
        printf("카피 오류 : 포인터 없음");
        return NULL_POINTER_ERROR; // 포인터 에러 
    }
    bi_Dst->wordlen = bi_Src->wordlen;

    if (bi_Src->sign == NON_NEGATIVE) bi_Dst->sign = NEGATIVE;
    else bi_Dst->sign = NON_NEGATIVE;

    for (int i = 0; i < bi_Src->wordlen; i++){
        bi_Dst->bData[i] = bi_Src->bData[i];
    }

    printf("복사 결과 [bi_Dst] : ");
    bi_print_bigint(bi_Dst);
    
    return FUNC_SUCCESS;

}
/**** 빅인티저 출력 함수 ****/ 
void bi_print_bigint(BIGINT* bi_Src) {
    if (bi_Src->sign == NEGATIVE) {
        printf("-");
    }
    for (int i = bi_Src->wordlen - 1; i >= 0; i--) {
        printf("%08X ", bi_Src->bData[i]);
    }
    printf("\n");
}
/**** 빅인티저 길이 반환 함수 ****/ 
int bi_get_length(BIGINT* bi_Src){
    return bi_Src->wordlen;
}
/**** 두개의 빅인티저 길이 비교 함수 ****/ 
// bi_Src1의 길이가 더 길면 1, bi_Src2의 길이가 더 길면 -1, 길이가 같으면 0 출력
int bi_compare_length(BIGINT* bi_Src1, BIGINT* bi_Src2){
    if (bi_get_length(bi_Src1) > bi_get_length(bi_Src2)){
        return 1;
    }

    else if (bi_get_length(bi_Src1) < bi_get_length(bi_Src2)){
        return -1;
    }

    else return 0;
}
/**** 두개의 빅인티저 길이 중 긴 값 리턴 함수 ****/ 
int bi_get_max_length(BIGINT* bi_Src1, BIGINT* bi_Src2){
    int result = bi_compare_length(bi_Src1, bi_Src2);
    if (result == 1) return bi_Src1->wordlen;
    else return bi_Src2->wordlen;
}
/**** 두개의 빅인티저 절대값 비교 함수 ****/ 
// bi_Src1의 절댓값이 더 크면 1, bi_Src2의 절댓값이 더 크면 -1, 같으면 0 리턴
int bi_compare_ABS(BIGINT* bi_Src1, BIGINT* bi_Src2){
    int n = bi_get_length(bi_Src1);
    int m = bi_get_length(bi_Src2);

    // x의 워드 길이가 더 큰 경우
    if (n > m){
        return 1;
    }

    // y의 워드 길이가 더 큰 경우
    else if (n < m){
        return -1;
    }

    // 워드 길이가 같은 경우
    for (int j = n-1; j >= 0; j--){
        if (bi_Src1->bData[j] > bi_Src2->bData[j]){
            return 1;
        }
        else if (bi_Src1->bData[j] < bi_Src2->bData[j]){
            return -1;
        }
    }
    return 0;
}
/**** 두개의 빅인티저 값 비교 함수 ****/ 
// 부호 정보 고려. bi_Src1가 크면 1, bi_Src2가 크면 -1, 같으면 0
int bi_compare_bigint(BIGINT* bi_Src1, BIGINT* bi_Src2){
    // sign : NON_NEGATIVE = 0, NEGATIVE = 1
    int x_sign = bi_Src1->sign;
    int y_sign = bi_Src2->sign;

    // bi_Src1는 NON_NEGATIVE, bi_Src2는 NEGATIVE인 경우
    if (x_sign == 0 & y_sign == 1){
        return 1;
    }
    // bi_Src1는 NEGATIVE, bi_Src2는 NON_NEGATIVE인 경우 : bi_Src2가 더큼.
    if (x_sign == 1 & y_sign == 0){
        return -1;
    }

    // bi_Src1 * bi_Src2 >= 0 인 경우(부호 같은 경우)
    int ret = bi_compare_ABS(bi_Src1, bi_Src2); // 절댓값 비교함수

    // bi_Src1, bi_Src2가 NON_NEGATIVE인 경우, bi_compare_ABS 결과 그대로 출력하면 됨.
    if (x_sign == 0){
        return ret;
    }
    // bi_Src1, bi_Src2가 NEGATIVE인 경우, bi_compare_ABS 결과 반대로 출력하면 됨.
    else 
        return ret * (-1);
}
/**** 빅인티저가 0, 1인지 판별하는 함수 ****/
// true 1, false 0 리턴
int bi_is_zero(BIGINT* bi_Src){
    
    if (bi_Src->sign == NEGATIVE || bi_Src->bData[0] != 0) return 0;

    for (int i = bi_Src->wordlen - 1; i >= 1; i--) {
        if (bi_Src->bData[i] != 0) return 0;
    }
    return 1;
}
int bi_is_one(BIGINT* bi_Src){
    
    if (bi_Src->sign == NEGATIVE || bi_Src->bData[0] != 1) return 0;

    for (int i = bi_Src->wordlen - 1; i >= 1; i--) {
        if (bi_Src->bData[i] != 0) return 0;
    }
    return 1;
}

/**** 빅인티저 복사 함수 ****/
//void bi_assign(BIGINT* bi_Dst,  BIGINT* bi_Src)
//void bi_set_zero(BIGINT* bi_Src);
/**** 랜덤한 빅인티저 생성 함수 ****/
// wordlen 길이의 랜덤한 빅인티저 생성
//int bi_create_random_bigint(BIGINT* bi_Dst, int wordlen);
