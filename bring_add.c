#include "bring_arith.h"
//===============================================================================================//
//                                       사용자용 함수 구역
//                                     사용자가 직접 사용할 함수

int BI_Add_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2) {
    if(bi_src1 == NULL || bi_src2 == NULL) {
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
        // 음수가 더 절댓값이 큰 경우
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
//===============================================================================================//

//===============================================================================================//
//                                       개발자용 함수 구역
//                                  사용자는 직접 사용하지 않는 함수
//

//===============================================================================================//
//                                       개발자용 함수 구역
//                                  사용자는 직접 사용하지 않는 함수
//

/**** 단일 워드 덧셈 ****/
// bi_Add_w의 리턴 : 캐리
// !!!! 캐리를 리턴 해주는 형식으로 할지 아니면 변수에 carry_out을 추가하고 리턴타입은 void로 할지 정해도 됨. 
int bi_Add_w(word* p_dst, word p_src1, word p_src2, int carry_in) {
    int carry_out = 0;

    // 0xffffffff 수정 필요
    // *p_dst = (p_src1 + p_src2) & 0xffffffff;
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

/**** 다중 워드 덧셈 ****/
// 리턴 값 : void, 둘다 양수로 보고 계산, src1의 길이가 src2의 길이보다 크거나 같도록 함.
void bi_Add_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2) {
    // // bi_src1이 0일 때
    // if(bi_is_zero(bi_src1))
    //     return bi_assign(bi_dst, bi_src2);
    // // bi_src2이 0일 때
    // if(bi_is_zero(bi_src2))
    //     return bi_assign(bi_dst, bi_src1);

    // // bi_src1이 양수이고 bi_src2가 음수일 때
    // if((bi_src1->sign == NON_NEGATIVE) && (bi_src2->sign == NEGATIVE))
    //     return bi_Sub_zxy(bi_dst, bi_src1, bi_src2);
    // // bi_src1이 음수이고 bi_src2가 양수일 때 
    // if((bi_src1->sign == NEGATIVE) && (bi_src2->sign == NON_NEGATIVE))
    //     return bi_Sub_zxy(bi_dst, bi_src2, bi_src1);

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

    ((*bi_dst)->p[bi_src1->wordlen]) = carry;

    bi_refine(*bi_dst);

    // // bi_src1의 길이와 bi_src2의 길이가 같을 때
    // if((bi_src1->wordlen) == (bi_src2->wordlen)){
    //     for(int i = 0; i < bi_src2->wordlen; i++) {
    //         carry_in = bi_Add_w(&((*bi_dst)->p[i]), bi_src1->p[i], bi_src2->p[i], carry_in);
    //     }
    //     if(carry_in == 1) {
    //         bi_dst[bi_src1->wordlen] = carry_in;
    //     }
    // } // bi_src1의 길이가 bi_src2의 길이보다 길 때
    // else if((bi_src1->wordlen) > (bi_src2->wordlen)){
    //     for(int i = 0; i < bi_src2->wordlen; i++)
    //         carry_in = bi_Add_w(&((*bi_dst)->p[i]), bi_src1->p[i], bi_src2->p[i], carry_in);
    //     if(carry_in == 1){
    //         for(int i = bi_src2->wordlen; i < bi_src1->wordlen; i++){
    //             bi_dst[i] = (bi_src1->p[i] + carry_in) & 0xffffffff;
    //             if(carry_in == 1)
    //                 bi_dst[bi_src1->wordlen] = carry_in;
    //         }
    //     }
    //     else{
    //         for(int i = bi_src2->wordlen; i < bi_src1->wordlen; i++)
    //             bi_dst[i] = bi_src1->p[i];
    //     }
    // } // bi_src1의 길이가 bi_src2의 길이보다 짧을 때
    // else{
    //     for(int i = 0; i < bi_src1->wordlen; i++)
    //         carry_in = bi_Add_w(bi_dst, bi_src2->p[i], bi_src1->p[i], carry_in);
    //     if(carry_in == 1){
    //         for(int i = bi_src1->wordlen; i < bi_src2->wordlen; i++){
    //             carry_in = (bi_src2->p[i] + carry_in) & 0xffffffff;
    //             if(carry_in == 1)
    //                 bi_dst[bi_src2->wordlen] = carry_in;
    //         }
    //     }
    //     else{
    //         for(int i = bi_src1->wordlen; i < bi_src2->wordlen; i++)
    //             bi_dst[i] = bi_src2->p[i];
    //     }
    // }
    if(flag) {
        bi_refine(bi_src2);
    }

} 
