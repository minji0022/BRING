#include <stdio.h>
#include <time.h>
#include "bring_arith.h"

void add_test() { 
    double bench = 0.0;

    for(int i = 0; i < REPEAT_COUNT; i++) {
        BIGINT* num0 = NULL;
        BIGINT* num1 = NULL;
        BIGINT* result = NULL;

        bi_gen_rand(&num0, SRC1_SIGN, SRC1_WORD_SIZE);
        bi_gen_rand(&num1, SRC2_SIGN, SRC2_WORD_SIZE);

        clock_t start = clock();
        BI_Add_zxy(&result, num0, num1);
        clock_t end = clock();
        bench += (double)(end - start) / CLOCKS_PER_SEC;
        
        printf("if (");
        bi_print_bigint_hex(num0);
        printf(" + ");
        bi_print_bigint_hex(num1);
        printf(" != ");
        bi_print_bigint_hex(result);
        printf(") :  cnt -= 1 ");
        puts("");   

        bi_delete(&num0);
        bi_delete(&num1);
        bi_delete(&result);
    }

#if BENCHMARK_FLAG == 1
        printf("print(\" 소요 시간 : ");
        printf("%f", bench/REPEAT_COUNT);
        printf("초 ( %d 회 반복 평균)\")\n", REPEAT_COUNT);
#endif
}

void sub_test() { 
    double bench = 0.0;

    for(int i = 0; i < REPEAT_COUNT; i++) {
        BIGINT* num0 = NULL;
        BIGINT* num1 = NULL;
        BIGINT* result = NULL;

        bi_gen_rand(&num0, SRC1_SIGN, SRC1_WORD_SIZE);
        bi_gen_rand(&num1, SRC2_SIGN, SRC2_WORD_SIZE);

        clock_t start = clock();
        BI_Sub_zxy(&result, num0, num1);
        clock_t end = clock();
        bench += (double)(end - start) / CLOCKS_PER_SEC;
        
        printf("if (");
        bi_print_bigint_hex(num0);
        printf(" - ");
        bi_print_bigint_hex(num1);
        printf(" != ");
        bi_print_bigint_hex(result);
        printf(") :  cnt -= 1 ");
        puts("");   

        bi_delete(&num0);
        bi_delete(&num1);
        bi_delete(&result);
    }

#if BENCHMARK_FLAG == 1
        printf("print(\" 소요 시간 : ");
        printf("%f", bench/REPEAT_COUNT);
        printf("초 ( %d 회 반복 평균)\")\n", REPEAT_COUNT);
#endif
}

void mul_test() { 
    double bench = 0.0;

    for(int i = 0; i < REPEAT_COUNT; i++) {
        BIGINT* num0 = NULL;
        BIGINT* num1 = NULL;
        BIGINT* result = NULL;

        bi_gen_rand(&num0, SRC1_SIGN, SRC1_WORD_SIZE);
        bi_gen_rand(&num1, SRC2_SIGN, SRC2_WORD_SIZE);

        clock_t start = clock();
        BI_Mul_zxy(&result, num0, num1);
        clock_t end = clock();
        bench += (double)(end - start) / CLOCKS_PER_SEC;
        
        printf("if (");
        bi_print_bigint_hex(num0);
        printf(" * ");
        bi_print_bigint_hex(num1);
        printf(" != ");
        bi_print_bigint_hex(result);
        printf(") :  cnt -= 1 ");
        puts("");   

        bi_delete(&num0);
        bi_delete(&num1);
        bi_delete(&result);
    }

#if BENCHMARK_FLAG == 1
        printf("print(\" 소요 시간 : ");
        printf("%f", bench/REPEAT_COUNT);
        printf("초 ( %d 회 반복 평균)\")\n", REPEAT_COUNT);
#endif
}

void div_test() { 
    double bench = 0.0;

    for(int i = 0; i < REPEAT_COUNT; i++) {
        BIGINT* num0 = NULL;
        BIGINT* num1 = NULL;
        BIGINT* q = NULL;
        BIGINT* r = NULL;

        bi_gen_rand(&num0, SRC1_SIGN, SRC1_WORD_SIZE);
        bi_gen_rand(&num1, SRC2_SIGN, SRC2_WORD_SIZE);

        clock_t start = clock();
        BI_Div_zxy(&q, &r, num0, num1);
        clock_t end = clock();
        bench += (double)(end - start) / CLOCKS_PER_SEC;
        
        printf("if (");
        bi_print_bigint_hex(num0);
        printf(" // ");
        bi_print_bigint_hex(num1);
        printf(" != ");
        bi_print_bigint_hex(q);
        printf(") :  cnt -= 1 ");
        puts("");   

        printf("if (");
        bi_print_bigint_hex(num0);
        printf(" %% ");
        bi_print_bigint_hex(num1);
        printf(" != ");
        bi_print_bigint_hex(r);
        printf(") :  cnt -= 1 ");
        puts("");

        bi_delete(&q);
        bi_delete(&r);
        bi_delete(&num0);
        bi_delete(&num1);
    }

#if BENCHMARK_FLAG == 1
        printf("print(\" 소요 시간 : ");
        printf("%f", bench/REPEAT_COUNT);
        printf("초 ( %d 회 반복 평균)\")\n", REPEAT_COUNT);
#endif
}

void mod_test() { 
    double bench = 0.0;

    for(int i = 0; i < REPEAT_COUNT; i++) {
        BIGINT* num0 = NULL;
        BIGINT* num1 = NULL;
        BIGINT* result = NULL;

        bi_gen_rand(&num0, SRC1_SIGN, SRC1_WORD_SIZE);
        bi_gen_rand(&num1, SRC2_SIGN, SRC2_WORD_SIZE);

        clock_t start = clock();
        BI_Mod_zxy(&result, num0, num1);
        clock_t end = clock();
        bench += (double)(end - start) / CLOCKS_PER_SEC;
        
        printf("if (");
        bi_print_bigint_hex(num0);
        printf(" %% ");
        bi_print_bigint_hex(num1);
        printf(" != ");
        bi_print_bigint_hex(result);
        printf(") :  cnt -= 1 ");
        puts("");   

        bi_delete(&num0);
        bi_delete(&num1);
        bi_delete(&result);
    }

#if BENCHMARK_FLAG == 1
        printf("print(\" 소요 시간 : ");
        printf("%f", bench/REPEAT_COUNT);
        printf("초 ( %d 회 반복 평균)\")\n", REPEAT_COUNT);
#endif
}

void sqr_test() { 
    double bench = 0.0;

    for(int i = 0; i < REPEAT_COUNT; i++) {
        BIGINT* num0 = NULL;
        BIGINT* result = NULL;

        bi_gen_rand(&num0, SRC1_SIGN, SRC1_WORD_SIZE);
        
        clock_t start = clock();
        BI_Sqr_zx(&result, num0);
        clock_t end = clock();
        bench += (double)(end - start) / CLOCKS_PER_SEC;
        
        printf("if (");
        bi_print_bigint_hex(num0);
        printf(" ** 2 !=");
        bi_print_bigint_hex(result);
        printf(") :  cnt -= 1 ");
        puts("");   

        bi_delete(&num0);
        bi_delete(&result);
    }

#if BENCHMARK_FLAG == 1
        printf("print(\" 소요 시간 : ");
        printf("%f", bench/REPEAT_COUNT);
        printf("초 ( %d 회 반복 평균)\")\n", REPEAT_COUNT);
#endif
}

void exp_mod_test() { 
    double bench = 0.0;

    for(int i = 0; i < REPEAT_COUNT; i++) {
        BIGINT* num0 = NULL;
        BIGINT* num1 = NULL;
        BIGINT* result = NULL;
        BIGINT* M = NULL;

        bi_gen_rand(&num0, SRC1_SIGN, SRC1_WORD_SIZE);
        bi_gen_rand(&num1, SRC2_SIGN, SRC2_WORD_SIZE);
        bi_gen_rand(&M, NON_NEGATIVE, 7);

        clock_t start = clock();
        BI_ExpMod_zx(&result, num0, num1, M);
        clock_t end = clock();
        bench += (double)(end - start) / CLOCKS_PER_SEC;
        
        printf("if (");
        printf("pow(");
        bi_print_bigint_hex(num0);
        printf(", ");
        bi_print_bigint_hex(num1);
        printf(", ");
        bi_print_bigint_hex(M);
        printf(") != ");
        bi_print_bigint_hex(result);
        printf(") : cnt -=1");
        puts("");

        bi_delete(&num0);
        bi_delete(&num1);
        bi_delete(&result);
        bi_delete(&M);
    }

#if BENCHMARK_FLAG == 1
        printf("print(\" 소요 시간 : ");
        printf("%f", bench/REPEAT_COUNT);
        printf("초 ( %d 회 반복 평균)\")\n", REPEAT_COUNT);
#endif
}

void barr_test() { 
    double bench = 0.0;

    for(int i = 0; i < REPEAT_COUNT; i++) {
        BIGINT* A = NULL;
        BIGINT* N = NULL;
        BIGINT* T = NULL;
        BIGINT* R = NULL;

        bi_gen_rand(&A, SRC1_SIGN, SRC1_WORD_SIZE);
        bi_gen_rand(&N, SRC2_SIGN, SRC2_WORD_SIZE);

        // Pre-Computation
        bi_BR_pre_computed(&T, N);

        clock_t start = clock();
        BI_Barret_Reduction(&R, A, N, T);
        clock_t end = clock();
        bench += (double)(end - start) / CLOCKS_PER_SEC;

        printf("if (");
        bi_print_bigint_hex(A);
        printf(" %% ");
        bi_print_bigint_hex(N);
        printf(" != ");
        bi_print_bigint_hex(R);
        printf(") :  cnt -= 1 ");
        puts("");   

        bi_delete(&A);
        bi_delete(&N);
        bi_delete(&R);
        bi_delete(&T);
    }

#if BENCHMARK_FLAG == 1
        printf("print(\" 소요 시간 : ");
        printf("%f", bench/REPEAT_COUNT);
        printf("초 ( %d 회 반복 평균)\")\n", REPEAT_COUNT);
#endif
}

int main() {
    printf("print(\"\"\"  * @brief SET VERIFY_MODE\n * @test ${0: Verify ADD(+) Operation\n * @test ${1: Verify SUB(-) Operation\n * @test ${2: Verify MUL(*) Operation\n * @test ${3: Verify DIV(/) Operation\n * @test ${4: Verify MOD(%%) Operation\n * @test ${5: Verify SQR(**) Operation\n * @test ${6: Verify EXP_MOD Operation\n * @test ${7: Verify Fast REDUCTION Operation \"\"\" )\n");
    printf("print(\" ---------------------------------------- \")\n");
    printf("print(\"             [ Success Rate ]  \")\n");
    printf("print(\" ---------------------------------------- \")\n");
    printf("print(\"             VERIFY MODE : ");
    printf("%d", VERIFY_MODE);
    printf("\")\n");

    printf("cnt = ");
    printf("%d\n", REPEAT_COUNT);

// ----------------- ADD -----------------
#if VERIFY_MODE == 0
        add_test();        

// ----------------- SUB -----------------
#elif VERIFY_MODE == 1
        sub_test();
        
// ----------------- MUL -----------------
#elif VERIFY_MODE == 2
        mul_test();

// ----------------- DIV -----------------
#elif (VERIFY_MODE == 3)
        div_test();

// ----------------- MOD -----------------
#elif VERIFY_MODE == 4
        mod_test();
        
// ----------------- SQR -----------------
#elif (VERIFY_MODE == 5)
        sqr_test();

// ----------------- EXP_MOD -----------------
#elif (VERIFY_MODE == 6) 
        exp_mod_test();

// ------------- Barr. Reduction -------------
#elif (VERIFY_MODE == 7) 
        barr_test();

#endif        

    printf("print(\"                %%s/");
    printf("%d", REPEAT_COUNT);
    printf(" \"%%format(cnt))\n");
    printf("print(\" ---------------------------------------- \")\n");

    return 0;
}