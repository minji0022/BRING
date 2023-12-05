#include "bring_util.h"
//################################################################################################# 
//                                  BIGINT 생성 및 소멸관련 함수
//#################################################################################################
void bi_new(BIGINT** bi_dst, int wordlen) {
    if(*bi_dst != NULL) {
        bi_delete(bi_dst);
    }

    *bi_dst = (BIGINT*)malloc(sizeof(BIGINT));
    (*bi_dst)->sign = NON_NEGATIVE;
    (*bi_dst)->wordlen = wordlen;
    (*bi_dst)->p = (word*)calloc(wordlen, sizeof(word));
}

void bi_delete(BIGINT** bi_src) {
    if(*bi_src == NULL) {
        return ;
    }
    free((*bi_src)->p);
    free(*bi_src);
    *bi_src = NULL;
}


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

// @2023.09.30. hex fix
int bi_set_by_string(BIGINT** bi_dst, int sign, char* str, int base) {
    // calculate input str length and word length
    int str_len = strlen(str);
    int char_per_word = 2 * WORD_BYTE_SIZE; // 한워드에 몇글자가 들어가냐 : 변수 이륾 변경
    int wordlen = (str_len + char_per_word - 1) / char_per_word;
    
    // ----------- check -----------
    // printf("string length: %d, word length: %d\n", str_len, wordlen);

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

        // ----------- check -----------
        // printf("%s\n", chunk);
        
        // convert string -> hex(word)
        word hex_tmp = (word)strtoul(chunk, NULL, 16);

        // ————— check —————
        // printf("hex: %08x\n", hex_tmp);  

        // copy hex_tmp to big int word*
        (*bi_dst)->p[i] = hex_tmp;
    }
    return TRUE;
}

/*
int bi_set_by_string(BIGINT** bi_dst, int sign, char* str, int base) {
    int str_len = strlen(str); // 입력된 문자열의 길이 계산
    int char_per_word = 2 * WORD_BYTE_SIZE; // 한 워드에 입력받은 char의 몇글자가 들어가는지 계산.
    int wordlen = (str_len + char_per_word - 1) / char_per_word; // 문자열 길이에 따라서 필요한 워드 개수 계산
    // 현재 word(unsigned int)가 4바이트니 8자리씩 끊어서 워드 하나 할당으로 함. 
    // 8 대신 word와 관련있는 표현으로 바꿔보기 ex) 2*sizeof(word)

    // 빅인티저 생성
    bi_new(bi_dst, wordlen);
    (*bi_dst)->sign = sign;    // 빅인티저의 부호 설정

    // 문자열을 16진수에서 정수로 변환하여 빅인티저 배열에 저장하는 단계
    int index = str_len - 1; // index : 전체 문자열의 index. 끝부터 시작

    // 문자열 길이 및 워드길이 확인용
    printf("str_len : %d /// ", str_len);
    printf("wordlen : %d\n", wordlen);

    // 문자를 8개씩 처리하여 unsigned int 정수로 변환
    // 가장 오른쪽 워드(낮은 값)부터 작업
    // 즉, 받은 전체 문자열이 6606BC465BF5ABCD일 때 5BF5ABCD 먼저 처리, 이후 6606BC46 처리하는 방식.
    for (int i = 0; i < wordlen - 1; i++) { 
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
                bi_delete(bi_dst);
                return INVALID_CHAR_ERROR;
            }
            value |= (digit_value << (j * 4)); 
            //!!!! 16진수 기준--> 4비트씩 왼쪽으로 쉬프트하면서 값 계산...
            //!!!! 다른 진수 고려하려면 수정해야함.
            index--;
        }
        (*bi_dst)->p[i] = value;
        //데이터 잘 들어가는지 확인용 코드
        //printf("p[%d] : %08X /// ", i, (*bi_dst)->p[i]);
    }
    // 마지막 워드 부분 처리.
    word value = 0; 
    int digits_to_read = 8; 

    for (int j = 0; j < digits_to_read; j++) {

        if (index < 0) { // index 0 보다 작은 경우 : 문자열 끝난 경우
            value |= (0 << (j * 4));
        }
        else {
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
                bi_delete(bi_dst);
                return INVALID_CHAR_ERROR;
            }
            value |= (digit_value << (j * 4)); // 4비트씩 왼쪽으로 쉬프트하면서 값 계산
        }
        index--;
    }
    (*bi_dst)->p[wordlen-1] = value;
    //데이터 잘 들어가는지 확인용 코드
    //printf("p[last] : %08X ", (*bi_dst)->p[wordlen-1]);
    //printf("\n");
    return FUNC_SUCCESS;
}
*/


//################################################################################################# 
//                                  BIGINT 구조체를 활용하는 유틸 함수
//#################################################################################################
/* ///// 크기 비교 함수의 경우 Src1이 크거나 길면 1, 같으면 0, 작으면 -1 리턴 ///// */
void bi_print_bigint_hex(BIGINT* bi_src) {
    int word_size = WORD_BYTE_SIZE;

    if(bi_src->sign == NEGATIVE) {
        printf("-");
    }
    printf("0x");

#if WORD == 0
    for(int i = bi_src->wordlen - 1; i >= 0; i--) {
            printf("%02x", bi_src->p[i]);
        }
#elif WORD == 1
    for(int i = bi_src->wordlen - 1; i >= 0; i--) {
        printf("%08x", bi_src->p[i]);
    }
#elif WORD == 2
    for(int i = bi_src->wordlen - 1; i >= 0; i--) {
            printf("%016llx", bi_src->p[i]);
    }
#else

#endif
}


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

void bi_assign(BIGINT** bi_dst, BIGINT* bi_src) {
    if(*bi_dst != NULL) {
        bi_delete(bi_dst);
    }

    bi_new(bi_dst, bi_src->wordlen);
    (*bi_dst)->sign = bi_src->sign;
    array_copy((*bi_dst)->p, bi_src->p, bi_src->wordlen);
}

void bi_assign_flip_sign(BIGINT** bi_dst, BIGINT* bi_src) {
    if(*bi_dst != NULL) {
        bi_delete(bi_dst);
    }

    bi_new(bi_dst, bi_src->wordlen);
    if(bi_src->sign == NON_NEGATIVE) {
        (*bi_dst)->sign = NEGATIVE;    
    }
    else {
        (*bi_dst)->sign = NON_NEGATIVE;    
    }
    array_copy((*bi_dst)->p, bi_src->p, bi_src->wordlen);
}

void array_copy(word* p_dst, word* Src_p, int wordlen) {
    for(int i = 0; i < wordlen; i++) {
        p_dst[i] = Src_p[i];
    }
}

void bi_gen_rand(BIGINT** bi_src, int sign, int wordlen) {
    bi_new(bi_src, wordlen);
    (*bi_src)->sign = sign;
    array_rand((*bi_src)->p, wordlen);

    bi_refine(*bi_src);
}

void array_rand(word* Dst, int wordlen) {
    byte* ptr = (byte*)Dst;
    int cnt = wordlen * sizeof(word);
    while(cnt > 0) {
        *ptr = rand() & 0xff;
        ptr++;
        cnt--;
    }
}

void bi_set_zero(BIGINT** bi_src) {
    bi_new(bi_src, 1);
    (*bi_src)->sign = NON_NEGATIVE;
    (*bi_src)->p[0] = 0x0;
}

void bi_set_one(BIGINT** bi_src) {
    bi_new(bi_src, 1);
    (*bi_src)->sign = NON_NEGATIVE;
    (*bi_src)->p[0] = 0x1;
}

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

int bi_get_bit_length(BIGINT* bi_src) {
    int BitLen = (bi_src->wordlen) * WORD_BIT_SIZE; // word가 unsigned int인 경우 : (bi_src->wordlen) * 32 
    int i = WORD_BIT_SIZE - 1; // word가 unsigned int인 경우 : i = 31
    while ((bi_src->p[bi_src->wordlen - 1] >> i & 0x1) == 0) {
        BitLen--;
        i--;
    }
    return BitLen;
} 

int bi_get_word_length(BIGINT* bi_src) {
    return bi_src->wordlen;
}

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

int bi_get_max_length(BIGINT* bi_src1, BIGINT* bi_src2){
    int result = bi_compare_length(bi_src1, bi_src2);
    if(result == 1) {
        return bi_src1->wordlen;
    }
    else {
        return bi_src2->wordlen;
    }
}

int bi_get_min_length(BIGINT* bi_src1, BIGINT* bi_src2){
    int result = bi_compare_length(bi_src1, bi_src2);
    if(result == 1) {
        return bi_src2->wordlen;
    }
    else {
        return bi_src1->wordlen;
    }
}

int bi_abs_is_one(BIGINT* bi_src){ // 절댓값이 1이면 1, 아니면 0 리턴
    if(bi_src->wordlen == 1 && bi_src->p[0] == 1) {
        return 1;
    }
    return 0;
}

int bi_length_of_n(int n) {
    int length = 0;
    while(n > 0) {
        n >>= 1; // n을 오른쪽으로 1 비트 시프트
        length++;
    }
    return length;
}

void bi_fill_zero(BIGINT* bi_src, int len) {
    if(bi_src == NULL) {
        return ;
    }
    bi_src->wordlen = bi_src->wordlen + len;
    bi_src->p = (word*)realloc(bi_src->p, sizeof(word)*bi_src->wordlen);
    // 아래는 realloc시 메모리 할당 오류가 나는 경우를 처리하기 위한 부분임. 
    // 실제로는 bi_src->p = (word*)realloc(bi_src->p, sizeof(word)*new_wordlen); 
    // 대신 아래 부분이 있어야할 것 같음. bi_fill_zero의 리턴 값을 int로 하고, 메모리 할당 실패시 bi_Add랑 bi_Sub에서
    // 이를 확인하고 연산을 종료하든지, 다른 방식으로 연산을 진행하든지 해야할 수도?
    /*
    word* backupBuffer = bi_src->p;
    if(NULL == (bi_src->p = (word*)realloc(bi_src->p, sizeof(word)*new_wordlen)))
    {
        bi_src->p = backupBuffer;
        printf("Memory allocation is failed in bi_fill_zero(%p)", bi_src->p);
        return;
    }
    */
    for(int i = bi_src->wordlen-1; i >= bi_src->wordlen - len; i--) {
        bi_src->p[i] = 0x0;
    }
}

void reductionOf2(BIGINT** bi_dst, BIGINT* bi_src, int r){
    
    int wk = bi_src->wordlen * WORD_BIT_SIZE;
    //printf("k = %d >> %d = %d\n", r, SHIFT_SIZE, k);

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
        //bi_new(bi_dst, )
        printf("bi_dst is not allocated \n");
        return ; // 나머지는 사용 안함. 필요시 구현
    }
    
}