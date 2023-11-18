#include "bring_arith.h"

//===============================================================================================//
//                                       사용자용 함수 구역
//                                     사용자가 직접 사용할 함수
//===============================================================================================//

/**
* @details [덧셈 함수] 두 큰 정수(BIGINT)를 더하는 연산
* @param[out] bi_dst 덧셈 결과 (= src1 + src2)
* @param[in] bi_src1 입력 src1
* @param[in] bi_src2 입력 src2
* @return Success or Error Code
*/
int BI_Add_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2) {
    // Error: 입력이 유효하지 않은 경우 - NULL_POINTER_ERROR
    if(bi_src1 == NULL || bi_src2 == NULL) {
        // *bi_dst == NULL 상태로 들어옴
        printf("[WARNING] : A 또는 B의 값이 존재하지 않음\n");
        return NULL_POINTER_ERROR;
    }
    
    // case 1: bi_src1이 0인 경우
    if(bi_is_zero(bi_src1)) {
        bi_assign(bi_dst, bi_src2);
        return FUNC_SUCCESS;
    }
    // case 2: bi_src2가 0인 경우
    if(bi_is_zero(bi_src2)) {
        bi_assign(bi_dst, bi_src1);
        return FUNC_SUCCESS;
    } 

    //bi_dst 할당.
    bi_new(bi_dst, (bi_src1->wordlen + bi_src2->wordlen + 1));

    // case 3: 부호 다른 경우
    if(bi_src1->sign == NON_NEGATIVE && bi_src2->sign == NEGATIVE) {
        // 음수의 절댓값이 더 큰 경우
        if(bi_compare_ABS(bi_src1, bi_src2) == -1) { 
            (*bi_dst)->sign = NEGATIVE;
            bi_Sub_zxy(bi_dst, bi_src2, bi_src1);
            return FUNC_SUCCESS;
        }
        else {
            bi_Sub_zxy(bi_dst, bi_src1, bi_src2);
            return FUNC_SUCCESS;
        }
    }
    if(bi_src1->sign == NEGATIVE && bi_src2->sign == NON_NEGATIVE) {
        if (bi_compare_ABS(bi_src1, bi_src2) == 1) {
            // 음수의 절댓값이 더 큰 경우
            (*bi_dst)->sign = NEGATIVE;
            bi_Sub_zxy(bi_dst, bi_src1, bi_src2);
            return FUNC_SUCCESS;     
        }
        else {
            bi_Sub_zxy(bi_dst, bi_src2, bi_src1);
            return FUNC_SUCCESS;
        }
        
    }
    else {// case 4: 부호 같은 경우
        (*bi_dst)->sign = bi_src1->sign;
        if(bi_src1->wordlen >= bi_src2->wordlen) {
            bi_Add_zxy(bi_dst, bi_src1, bi_src2);
            return FUNC_SUCCESS;
        }
        else {
            bi_Add_zxy(bi_dst, bi_src2, bi_src1);
            return FUNC_SUCCESS;
        }
    }
}

/**
* @details [덧셈 갱신 함수] 두 큰 정수(BIGINT)를 더하는 연산 (갱신: src1 += src2)
* @param[in] bi_src1 입력 src1, 출력 src1 = src1 + src2
* @param[in] bi_src2 입력 src2
* @return Success or Error Code
*/
int BI_Add_xy(BIGINT** bi_src1, BIGINT* bi_src2) {
    BIGINT* tmp = NULL;
    
    BI_Add_zxy(&tmp, *bi_src1, bi_src2);
    bi_assign(bi_src1, tmp);

    bi_delete(&tmp);
    return FUNC_SUCCESS;
}

//===============================================================================================//
//                                       개발자용 함수 구역
//                                  사용자는 직접 사용하지 않는 함수
//===============================================================================================//

/**
* @details 단일 워드 덧셈 Single Word Addition
* @param[out] p_dst 단일 워드 덧셈 결과 (= src1 + src2 + carry_in)
* @param[in] p_src1 입력 src1
* @param[in] p_src2 입력 src2
* @param[in] carry_in 입력 carry
* @return int carry_out
* @note bi_Add_w의 리턴 : carry
*/
int bi_Add_w(word* p_dst, word p_src1, word p_src2, int carry_in) {
    int carry_out = 0;

    *p_dst = (p_src1 + p_src2);

    if(*p_dst < p_src1) {
        carry_out = 1;
    }

    *p_dst += carry_in;
    
    if(*p_dst < carry_in) {
        carry_out += 1;
    }

    return carry_out;
}; 

/**
* @details 다중 워드 덧셈 Multi Word Addition
* @param[out] bi_dst 다중 워드 덧셈 결과 (= src1 + src2)
* @param[in] bi_src1 입력 src1
* @param[in] bi_src2 입력 src2
* @return 
* @note src1, src2 모두 양수로 보고 계산, src1의 길이가 src2의 길이보다 크거나 같도록 함.
*/
void bi_Add_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2) {
    int carry = 0;
    int flag = 0;

    // src1과 src2의 길이가 다름 = |src1| > |src2|
    if(bi_src1->wordlen != bi_src2->wordlen) {
        bi_fill_zero(bi_src2, bi_src1->wordlen - bi_src2->wordlen);
        flag = 1;
    }

    for(int i = 0; i < bi_src2->wordlen; i++) {
        carry = bi_Add_w(&((*bi_dst)->p[i]), bi_src1->p[i], bi_src2->p[i], carry);
    }

    // 마지막 워드 덧셈 연산 후 캐리 발생하는 경우를 위함
    ((*bi_dst)->p[bi_src1->wordlen]) = carry;

    if(flag) {
        bi_refine(bi_src2);
    }

    bi_refine(*bi_dst);
} 
