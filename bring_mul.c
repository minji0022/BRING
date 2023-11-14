#include "bring_arith.h"

//===============================================================================================//
//                                       사용자용 함수 구역
//                                     사용자가 직접 사용할 함수
//===============================================================================================//

int BI_Mul_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2) {
    // Error: 입력이 유효하지 않은 경우 - NULL_POINTER_ERROR
    if(bi_src1 == NULL || bi_src2 == NULL) { 
        return NULL_POINTER_ERROR;
    } 

    if(bi_is_zero(bi_src1) || bi_is_zero(bi_src2)) {
        bi_set_zero(bi_dst);
        return FUNC_SUCCESS;
    }
    if(bi_abs_is_one(bi_src1)) {
        bi_assign(bi_dst, bi_src2);
        return FUNC_SUCCESS;
    }
    if(bi_abs_is_one(bi_src2)) {
        bi_assign(bi_dst, bi_src1);
        return FUNC_SUCCESS;
    }

    //bi_dst 할당. 이게 필요 없나? schoolbook에서는 처음에 c=0 으로 초기화 해 줌.
    bi_new(bi_dst, (bi_src1->wordlen + bi_src2->wordlen + 1));

    // 추후 카라추바 곱셈으로 변경 예정
    bi_Mul_Schoolbook_zxy(bi_dst, bi_src1, bi_src2);

    if((bi_src1->sign == NEGATIVE && bi_src2->sign == NON_NEGATIVE) || (bi_src1->sign == NON_NEGATIVE && bi_src2->sign == NEGATIVE)) {
        (*bi_dst)->sign = NEGATIVE;
    }

    return FUNC_SUCCESS;
}

//===============================================================================================//
//                                       개발자용 함수 구역
//                                  사용자는 직접 사용하지 않는 함수
//===============================================================================================//

void bi_Mul_w(BIGINT** bi_dst, word p_src1, word p_src2){
    int w_div_2 = WORD_BIT_SIZE / 2;

    word src1_0 = p_src1 & ((1 << w_div_2) - 1);
    word src1_1 = p_src1 >> w_div_2;
    word src2_0 = p_src2 & ((1 << w_div_2) - 1);
    word src2_1 = p_src2 >> w_div_2;

    word t0 = src1_1 * src2_0;
    word t1 = src1_0 * src2_1;

    t0 = t0 + t1;
    t1 = (word)(t0 < t1);
    
    word c1 = src1_1 * src2_1;
    word c0 = src1_0 * src2_0;

    word t = c0;
    c0 = c0 + (t0 << w_div_2);
    c1 = c1 + (t1 << w_div_2) + (t0 >> w_div_2) + (word)(c0 < t);

    (*bi_dst)->p[0] = c0;
    (*bi_dst)->p[1] = c1;
}

void bi_Mul_Schoolbook_zxy(BIGINT** bi_dst, BIGINT* bi_src1, BIGINT* bi_src2) {
    BIGINT* tmp = NULL;
    BIGINT* lshfit_tmp = NULL;

    bi_set_zero(bi_dst);

    bi_new(&tmp, 2);
    bi_new(&lshfit_tmp, 2);

    for(int j = 0; j < bi_src1->wordlen; j++) {
        for(int i = 0; i < bi_src2->wordlen; i++) {
            bi_Mul_w(&tmp, bi_src1->p[j], bi_src2->p[i]);
            bi_left_shift(&lshfit_tmp, tmp, WORD_BIT_SIZE * (i + j));
            BI_Add_xy(bi_dst, lshfit_tmp);
        }
    }
}