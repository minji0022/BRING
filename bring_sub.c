#include "bring_arith.h"

//===============================================================================================//
//                                       사용자용 함수 구역
//                                     사용자가 직접 사용할 함수
//===============================================================================================//

/**
* @details 두 큰 정수(BIGINT)를 빼는 연산
* @param[out] bi_dst 뺄셈 결과 (= src1 - src2)
* @param[in] bi_src1 입력 src1
* @param[in] bi_src2 입력 src2
* @return Success or Error Code
*/
int BI_Sub_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2){
    // Error: 입력이 유효하지 않은 경우 - NULL_POINTER_ERROR
    if(bi_src1 == NULL || bi_src2 == NULL) { 
        // *bi_dst == NULL 상태로 들어옴
        printf("[WARNING] : A 또는 B의 값이 존재하지 않음\n");
        return NULL_POINTER_ERROR;
    } 

    // case 1: bi_src1이 0인 경우
    if(bi_is_zero(bi_src1)) { /* 부호 변경하여 복사 */ // 0 - bi_Src2
        bi_assign_flip_sign(bi_dst, bi_src2); // 이 안에 bi_new 있음
        return FUNC_SUCCESS;
    } 
    // case 2: bi_src2가 0인 경우
    if(bi_is_zero(bi_src2)) { /* 부호 유지하며 복사 */ // bi_src1 - 0
        bi_assign(bi_dst, bi_src1);  // 이 안에 bi_new 있음.
        return FUNC_SUCCESS;
    }
    // case 3: bi_src1, bi_src2가 같은 경우
    if(bi_compare_bigint(bi_src1, bi_src2) == 0) { 
        bi_set_zero(bi_dst); // 이 안에 bi_new 있음.
        return FUNC_SUCCESS;
    }

    //bi_dst 할당.
    bi_new(bi_dst, bi_src1->wordlen + bi_src2->wordlen);

    // case 4: 부호 다른 경우
    if(bi_src1->sign == NON_NEGATIVE && bi_src2->sign == NEGATIVE) { /* case 4: 부호 다른 경우 : 양수 - 음수 */
        if(bi_src1->wordlen >= bi_src2->wordlen) {
            bi_Add_zxy(bi_dst, bi_src1, bi_src2);
            return FUNC_SUCCESS;
        }
        else {
            bi_Add_zxy(bi_dst, bi_src2, bi_src1);
            return FUNC_SUCCESS;
        }
    }
    if(bi_src1->sign == NEGATIVE && bi_src2->sign == NON_NEGATIVE) { /* case 4: 부호 다른 경우 : 음수 - 양수 */
        (*bi_dst)->sign = NEGATIVE;

        if(bi_src1->wordlen >= bi_src2->wordlen) {
            bi_Add_zxy(bi_dst, bi_src1, bi_src2);
            return FUNC_SUCCESS;
        }
        else {
            bi_Add_zxy(bi_dst, bi_src2, bi_src1);
            return FUNC_SUCCESS;
        }
    }    
    // case 5: 부호 같은 경우 : 양수 - 양수
    if(bi_src1->sign == NON_NEGATIVE && bi_src2->sign == NON_NEGATIVE) { 
        if(bi_compare_ABS(bi_src1, bi_src2) == -1){ // 뒤에가 절대값이 더 큰 값인 경우
            (*bi_dst)->sign = NEGATIVE;
            bi_Sub_zxy(bi_dst, bi_src2, bi_src1);
            return FUNC_SUCCESS;
        }
        else {
            bi_Sub_zxy(bi_dst, bi_src1, bi_src2);
            return FUNC_SUCCESS;
        }
    } 
    // case 5: 부호 같은 경우 : 음수 - 음수
    if(bi_src1->sign == NEGATIVE && bi_src2->sign == NEGATIVE) { 
        if(bi_compare_ABS(bi_src1, bi_src2) == 1) { // 앞에가 절대값이 더 큰 값인 경우
            (*bi_dst)->sign = NEGATIVE;
            bi_Sub_zxy(bi_dst, bi_src1, bi_src2);
            return FUNC_SUCCESS;     
        }
        else {
            bi_Sub_zxy(bi_dst, bi_src2, bi_src1);
            return FUNC_SUCCESS;
        }
    }
    return FUNC_ERROR;
}

int BI_Sub_xy(BIGINT** bi_src1, BIGINT* bi_src2) {
    BIGINT* tmp = NULL;
    
    BI_Sub_zxy(&tmp, *bi_src1, bi_src2);
    bi_assign(bi_src1, tmp);

    bi_delete(&tmp);
    return FUNC_SUCCESS;
}


//===============================================================================================//
//                                       개발자용 함수 구역
//                                  사용자는 직접 사용하지 않는 함수
//===============================================================================================//

/**
* @details 단일 워드 뺄셈 Single Word Subtraction
* @param[out] p_dst 단일 워드 뻴셈 결과 (= src1 - src2 - borrow_in)
* @param[in] p_src1 입력 src1
* @param[in] p_src2 입력 src2
* @param[in] borrow_in 입력 borrow
* @return int borrow_out
* @note bi_Sub_w 리턴 : borrow. src1의 길이가 src2의 길이보다 크거나 같도록 함.
*/
int bi_Sub_w(word* p_dst, word p_src1, word p_src2, int borrow_in) {
    int borrow_out = 0;

    *p_dst = (p_src1 - borrow_in);
    if(p_src1 < borrow_in) {
        borrow_out = 1;
    }
    if(*p_dst < p_src2) {
        borrow_out += 1;
    }
    *p_dst = (*p_dst - p_src2);

    return borrow_out; // 0 또는 1
}

/**
* @details 다중 워드 뺄셈 Multi Word Subtraction
* @param[out] bi_dst 다중 워드 뺄셈 결과 (= src1 - src2)
* @param[in] bi_src1 입력 src1
* @param[in] bi_src2 입력 src2
* @return 
* @note src1, src2 모두 양수로 보고 계산, src1의 길이가 src2의 길이보다 크거나 같도록 함.
*/
void bi_Sub_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2) {
    int borrow = 0;
    int flag = 0;

    if(bi_src1->wordlen != bi_src2->wordlen) {
        bi_fill_zero(bi_src2, bi_src1->wordlen - bi_src2->wordlen);
        flag = 1;
    }
    
    for(int i = 0; i < bi_src1->wordlen; i++) {
        borrow = bi_Sub_w(&((*bi_dst)->p[i]), bi_src1->p[i], bi_src2->p[i], borrow);
    }

    if(flag) {
        bi_refine(bi_src2);
    }

    bi_refine(*bi_dst);
}
