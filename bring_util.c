#include "bring_util.h"
//################################################################################################# 
//                                  BIGINT 생성 및 소멸 관련 함수
//#################################################################################################

/**
 * @brief BIGINT 생성
 * @param[out] bi_dst wordlen의 길이인 큰 정수 메모리 공간 생성
 * @param[in] wordlen 워드 길이 설정
 * @note 부호는 NON_NEGATIVE로 설정
*/
void bi_new(BIGINT** bi_dst, int wordlen) {
    if(*bi_dst != NULL) {
        bi_delete(bi_dst);
    }

    *bi_dst = (BIGINT*)malloc(sizeof(BIGINT));
    (*bi_dst)->sign = NON_NEGATIVE;
    (*bi_dst)->wordlen = wordlen;
    (*bi_dst)->p = (word*)calloc(wordlen, sizeof(word));
}

/**
 * @brief BIGINT 소멸
 * @param[in] bi_src 입력 BIGINT 삭제
*/
void bi_delete(BIGINT** bi_src) {
    if(*bi_src == NULL) {
        return ;
    }
    free((*bi_src)->p);
    free(*bi_src);
    *bi_src = NULL;
}

/**
 * @brief 유효한 16진수 값인지 확인
 * @return TRUE or ERR_INVALID_INPUT
*/
int is_valid_hex(char* str) {
    // 입력 문자열이 NULL이면 유효하지 않음
    if (str == NULL) {
        return ERR_INVALID_INPUT;
    }
    // 문자열이 비어있으면 유효하지 않음
    if (*str == '\0') {
        return ERR_INVALID_INPUT;
    }
    // 문자열의 각 문자가 유효한 16진수 문자인지 확인
    while (*str != '\0') {
        if(!isxdigit(*str)) {
            return ERR_INVALID_INPUT;
        }
        str++;
    }
    return TRUE;
}

/**
 * @brief 16진수 문자열을 입력 받아 BIGINT set
 * @param[out] bi_dst 출력
 * @param[in] sign 입력 부호
 * @param[in] str 입력 문자열
 * @param[in] base 입력 진수 - 16진수 HEX만 지원
*/
int bi_set_by_string(BIGINT** bi_dst, int sign, char* str, int base) {
    // calculate input str length and word length
    int str_len = strlen(str);
    int char_per_word = 2 * WORD_BYTE_SIZE; // 한워드에 몇글자가 들어가냐 : 변수 이륾 변경
    int wordlen = (str_len + char_per_word - 1) / char_per_word;
    
    // declare new big int and initialize sign
    bi_new(bi_dst, wordlen);
    (*bi_dst)->sign = sign;

    // check invalid input
    if(is_valid_hex(str) != TRUE) {
        (*bi_dst) = NULL;
        puts("[WARNING] : Wrong String INPUT...");
        return INVALID_CHAR_ERROR;
    }
    // convert str -> word array 
    for(int i = 0; i < wordlen; i++) {
        // copy 8*index to new string
        char chunk[char_per_word + 1]; // +1 : to contain NUL("\0")
        int start_index = str_len - (i + 1) * char_per_word;
        int end_index = start_index + char_per_word;
        if (start_index < 0) { 
            start_index = 0; 
        }

        strncpy(chunk, str + start_index, end_index - start_index);
        chunk[end_index - start_index] = '\0'; // Add NUL
        
        // convert string -> hex(word)
        word hex_tmp = (word)strtoul(chunk, NULL, 16);

        // copy hex_tmp to big int word*
        (*bi_dst)->p[i] = hex_tmp;
    }
    return TRUE;
}

//################################################################################################# 
//                                  BIGINT 구조체를 활용하는 유틸 함수
//#################################################################################################
/**
 * @brief BIGINT를 16진수로 출력
 * @param[in] bi_src 입력
*/
void bi_print_bigint_hex(BIGINT* bi_src) {
    int word_size = WORD_BYTE_SIZE;

    if(bi_src->sign == NEGATIVE) {
        printf("-");
    }
    printf("0x");

// case 1: word -> unsigned char
#if WORD == 0
    for(int i = bi_src->wordlen - 1; i >= 0; i--) {
            printf("%02x", bi_src->p[i]);
        }

// case 2: word -> unsigned int
#elif WORD == 1
    for(int i = bi_src->wordlen - 1; i >= 0; i--) {
        printf("%08x", bi_src->p[i]);
    }

// case 3: word -> unsigned long long
#elif WORD == 2
    for(int i = bi_src->wordlen - 1; i >= 0; i--) {
            printf("%016llx", bi_src->p[i]);
    }
#else

#endif
}

/**
 * @brief BIGINT 상위 워드에 0이 채워져 있는 경우, 이를 제거하는 함수
 * @param[in] bi_src 입력
*/
void bi_refine(BIGINT* bi_src) {
    if(bi_src == NULL) {
        return ;
    }

    int new_wordlen = bi_src->wordlen;
    while(new_wordlen > 1) {
        if(bi_src->p[new_wordlen - 1] != 0) {
            break;
        }
        new_wordlen--;
    }
    if(bi_src->wordlen != new_wordlen) {
        bi_src->wordlen = new_wordlen;
        bi_src->p = (word*)realloc(bi_src->p, sizeof(word)*new_wordlen);
    }

    if((bi_src->wordlen == 1) && (bi_src->p[0] == 0x0)) { // 0인 빅인티저가 된 경우
        bi_src->sign = NON_NEGATIVE;
    }
}

/**
 * @brief BIGINT 할당 함수 (bi_dst = bi_src)
 * @param[out] bi_dst 출력
 * @param[in] bi_src 입력
*/
void bi_assign(BIGINT** bi_dst, BIGINT* bi_src) {
    if(*bi_dst != NULL) {
        bi_delete(bi_dst);
    }

    bi_new(bi_dst, bi_src->wordlen);
    (*bi_dst)->sign = bi_src->sign;
    array_copy((*bi_dst)->p, bi_src->p, bi_src->wordlen);
}

/**
 * @brief BIGINT 부호만 반대로 할당하는 함수 (bi_dst = -bi_src)
 * @param[out] bi_dst 출력
 * @param[in] bi_src 입력
*/
void bi_assign_flip_sign(BIGINT** bi_dst, BIGINT* bi_src) {
    if(*bi_dst != NULL) {
        bi_delete(bi_dst);
    }

    bi_new(bi_dst, bi_src->wordlen);

    // flip sign
    if(bi_src->sign == NON_NEGATIVE) {
        (*bi_dst)->sign = NEGATIVE;    
    }
    else {
        (*bi_dst)->sign = NON_NEGATIVE;    
    }
    array_copy((*bi_dst)->p, bi_src->p, bi_src->wordlen);
}

/**
 * @brief 배열 복사 함수
*/
void array_copy(word* p_dst, word* Src_p, int wordlen) {
    for(int i = 0; i < wordlen; i++) {
        p_dst[i] = Src_p[i];
    }
}

/**
 * @brief 설정된 길이의 BIGINT를 랜덤하게 설정하는 함수
 * @param[out] bi_dst 출력
 * @param[in] sign 입력 부호
 * @param[in] wordlen 입력 길이
*/
void bi_gen_rand(BIGINT** bi_dst, int sign, int wordlen) {
    bi_new(bi_dst, wordlen);
    (*bi_dst)->sign = sign;
    array_rand((*bi_dst)->p, wordlen);

    bi_refine(*bi_dst);
}

/**
 * @brief 배열에 랜덤한 값을 할당하는 함수
*/
void array_rand(word* Dst, int wordlen) {
    byte* ptr = (byte*)Dst;
    int cnt = wordlen * sizeof(word);
    while(cnt > 0) {
        *ptr = rand() & 0xff;
        ptr++;
        cnt--;
    }
}

/**
 * @brief 0으로 초기화
 * @param[in] bi_src 입력
*/
void bi_set_zero(BIGINT** bi_src) {
    bi_new(bi_src, 1);
    (*bi_src)->sign = NON_NEGATIVE;
    (*bi_src)->p[0] = 0x0;
}

/**
 * @brief 1로 초기화
 * @param[in] bi_src 입력
*/
void bi_set_one(BIGINT** bi_src) {
    bi_new(bi_src, 1);
    (*bi_src)->sign = NON_NEGATIVE;
    (*bi_src)->p[0] = 0x1;
}

/**
 * @brief 0인지 확인 
 * @param[in] bi_src 입력
 * @return 0이라면 1을, 아니라면 0을 반환
*/
int bi_is_zero(BIGINT* bi_src) { // TRUE 1, false 0 리턴
    if(bi_src->sign == NEGATIVE || bi_src->p[0] != 0) {
        return 0;
    }

    for(int i = bi_src->wordlen - 1; i >= 1; i--) {
        if(bi_src->p[i] != 0) {
            return 0;
        } 
    }
    return 1;
}

/**
 * @brief 1인지 확인 
 * @param[in] bi_src 입력
 * @return 1이라면 1을, 아니라면 0을 반환
*/
int bi_is_one(BIGINT* bi_src) { // TRUE 1, false 0 리턴
    if(bi_src->sign == NEGATIVE || bi_src->p[0] != 1) {
        return 0;
    }
    for(int i = bi_src->wordlen - 1; i >= 1; i--) {
        if (bi_src->p[i] != 0) {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief 절댓값 비교 함수
 * @param[in] bi_src1 입력
 * @param[in] bi_src2 입력
 * @return |bi_src1| > |bi_src2|이라면 1을, |bi_src1| < |bi_src2|라면 -1을, |bi_src1| = |bi_src2|라면 0을 반환
*/
int bi_compare_ABS(BIGINT* bi_src1, BIGINT* bi_src2){ // bi_src1가 크면 1, bi_src2가 크면 -1, 같으면 0
    // x의 워드 길이가 더 큰 경우
    if(bi_src1->wordlen > bi_src2->wordlen) {
        return 1;
    }
    // y의 워드 길이가 더 큰 경우
    else if(bi_src1->wordlen < bi_src2->wordlen) {
        return -1;
    }
    // 워드 길이가 같은 경우
    for(int j = bi_src1->wordlen-1; j >= 0; j--) {
        if(bi_src1->p[j] > bi_src2->p[j]) {
            return 1;
        }
        else if(bi_src1->p[j] < bi_src2->p[j]) {
            return -1;
        }
    }
    return 0;
} 

/**
 * @brief 크기 비교 함수
 * @param[in] bi_src1 입력
 * @param[in] bi_src2 입력
 * @return bi_src1 > bi_src2이라면 1을, bi_src1 < bi_src2라면 -1을, bi_src1 = bi_src2라면 0을 반환
*/
int bi_compare_bigint(BIGINT* bi_src1, BIGINT* bi_src2){ // bi_src1가 크면 1, bi_src2가 크면 -1, 같으면 0
    // sign : NON_NEGATIVE = 0, NEGATIVE = 1
    int x_sign = bi_src1->sign;
    int y_sign = bi_src2->sign;
    // bi_src1는 NON_NEGATIVE, bi_src2는 NEGATIVE인 경우
    if(x_sign == NON_NEGATIVE & y_sign == NEGATIVE) {
        return 1;
    }
    // bi_src1는 NEGATIVE, bi_src2는 NON_NEGATIVE인 경우 : bi_src2가 더큼.
    if(x_sign == NEGATIVE & y_sign == NON_NEGATIVE) {
        return -1;
    }
    // bi_src1 * bi_src2 >= 0 인 경우(부호 같은 경우)
    int ret = bi_compare_ABS(bi_src1, bi_src2); // 절댓값 비교함수
    // bi_src1, bi_src2가 NON_NEGATIVE인 경우, bi_compare_ABS 결과 그대로 출력하면 됨.
    if(x_sign == 0) {
        return ret;
    }
    // bi_src1, bi_src2가 NEGATIVE인 경우, bi_compare_ABS 결과 반대로 출력하면 됨.
    else {
        return ret * (-1);
    }        
}

/**
 * @brief 비트 길이를 반환하는 함수
 * @param[in] bi_src 입력
 * @return 비트 길이
*/
int bi_get_bit_length(BIGINT* bi_src) {
    int BitLen = (bi_src->wordlen) * WORD_BIT_SIZE; // word가 unsigned int인 경우 : (bi_src->wordlen) * 32 
    int i = WORD_BIT_SIZE - 1; // word가 unsigned int인 경우 : i = 31
    while ((bi_src->p[bi_src->wordlen - 1] >> i & 0x1) == 0) {
        BitLen--;
        i--;
    }
    return BitLen;
} 

/**
 * @brief 워드 길이를 반환하는 함수
 * @param[in] bi_src 입력
 * @return 워드 길이
*/
int bi_get_word_length(BIGINT* bi_src) {
    return bi_src->wordlen;
}

/**
 * @brief 길이 비교 함수
 * @param[in] bi_src1 입력
 * @param[in] bi_src2 입력
 * @return bi_src1->wordlen > bi_src2->wordlen이라면 1을, bi_src1->wordlen < bi_src2->wordlen라면 -1을, bi_src1->wordlen = bi_src2->wordlen라면 0을 반환
*/
int bi_compare_length(BIGINT* bi_src1, BIGINT* bi_src2){ // bi_src1가 크면 1, bi_src2가 크면 -1, 같으면 0
    if(bi_get_word_length(bi_src1) > bi_get_word_length(bi_src2)) {
        return 1;
    }
    else if(bi_get_word_length(bi_src1) < bi_get_word_length(bi_src2)) {
        return -1;
    }
    else {
        return 0;
    }
}

/**
 * @brief 더 큰 BIGINT의 워드 길이를 반환하는 함수
 * @param[in] bi_src1 입력
 * @param[in] bi_src2 입력
 * @return 워드 길이
*/
int bi_get_max_length(BIGINT* bi_src1, BIGINT* bi_src2){
    int result = bi_compare_length(bi_src1, bi_src2);
    if(result == 1) {
        return bi_src1->wordlen;
    }
    else {
        return bi_src2->wordlen;
    }
}

/**
 * @brief 더 작은 BIGINT의 워드 길이를 반환하는 함수
 * @param[in] bi_src1 입력
 * @param[in] bi_src2 입력
 * @return 워드 길이
*/
int bi_get_min_length(BIGINT* bi_src1, BIGINT* bi_src2){
    int result = bi_compare_length(bi_src1, bi_src2);
    if(result == 1) {
        return bi_src2->wordlen;
    }
    else {
        return bi_src1->wordlen;
    }
}

/**
 * @brief 절댓값이 1인지 확인하는 함수
 * @param[in] bi_src 입력
 * @return 절댓값이 1이면 1, 아니면 0 리턴
*/
int bi_abs_is_one(BIGINT* bi_src){ 
    if(bi_src->wordlen == 1 && bi_src->p[0] == 1) {
        return 1;
    }
    return 0;
}

/**
 * @brief int 의 비트 길이를 반환하는 함수
 * @param[in] n 입력
 * @return n의 비트 길이
*/
int bi_length_of_n(int n) {
    int length = 0;
    while(n > 0) {
        n >>= 1; // n을 오른쪽으로 1 비트 시프트
        length++;
    }
    return length;
}

/**
 * @brief 원하는 워드 길이가 될 때까지 상위 워드에 0을 채우는 함수
 * @param[in] bi_src 입력
 * @param[in] len 입력 워드 길이
*/
void bi_fill_zero(BIGINT* bi_src, int len) {
    if(bi_src == NULL) {
        return ;
    }
    bi_src->wordlen = bi_src->wordlen + len;
    bi_src->p = (word*)realloc(bi_src->p, sizeof(word)*bi_src->wordlen);
    
    for(int i = bi_src->wordlen-1; i >= bi_src->wordlen - len; i--) {
        bi_src->p[i] = 0x0;
    }
}

/**
 * @brief dst = src mod 2^r
 * @param[out] bi_dst 출력
 * @param[in] bi_src 입력
 * @param[in] r 입력
*/
void reductionOf2(BIGINT** bi_dst, BIGINT* bi_src, int r){
    int wk = bi_src->wordlen * WORD_BIT_SIZE;

    if (r >= WORD_BIT_SIZE * bi_src->wordlen) {
        bi_assign(bi_dst, bi_src);
        return ;
    }
    else if (r % WORD_BIT_SIZE == 0 && ( r/WORD_BIT_SIZE < bi_src->wordlen)) {
        int k = r >> SHIFT_SIZE;
        bi_new(bi_dst, k);
        array_copy((*bi_dst)->p, bi_src->p, k);
        return ;
    }
    else {
        printf("bi_dst is not allocated \n");
        return ; // 나머지는 사용 안함. 필요시 구현
    }
    
}