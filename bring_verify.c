#include "bring_arith.h"

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

    for(int i = 0; i < REPEAT_COUNT; i++) {
        BIGINT* num0 = NULL;
        BIGINT* num1 = NULL;
        BIGINT* result = NULL;
        
        bi_gen_rand(&num0, NON_NEGATIVE, 15);
        bi_gen_rand(&num1, NON_NEGATIVE, 11);

        printf("if (");


#if (VERIFY_MODE == 0) || (VERIFY_MODE == 1) || (VERIFY_MODE == 2) || (VERIFY_MODE == 4)
// ----------------- ADD -----------------
#if VERIFY_MODE == 0
        bi_print_bigint_hex(num0);
        printf(" + ");
        BI_Add_zxy(&result, num0, num1);

// ----------------- SUB -----------------
#elif VERIFY_MODE == 1
        bi_print_bigint_hex(num0);
        printf(" - ");
        BI_Sub_zxy(&result, num0, num1);
        
// ----------------- MUL -----------------
#elif VERIFY_MODE == 2
        bi_print_bigint_hex(num0);
        printf(" * ");
        BI_Mul_zxy(&result, num0, num1);

// ----------------- MOD -----------------
#elif VERIFY_MODE == 4
        bi_print_bigint_hex(num0);
        printf(" %% ");
        BI_Mod_zxy(&result, num0, num1);
#endif
        bi_print_bigint_hex(num1);
        printf(" != ");
        bi_print_bigint_hex(result);
        printf(") :  cnt -= 1 ");
        puts("");
        
// ----------------- DIV -----------------
#elif (VERIFY_MODE == 3)
        BIGINT* q = NULL;
        BIGINT* r = NULL;

        bi_print_bigint_hex(num0);
        printf(" // ");
        BI_Div_zxy(&q, &r, num0, num1);
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

// ----------------- SQR -----------------
#elif (VERIFY_MODE == 5)
        bi_print_bigint_hex(num0);
        printf(" ** 2 !=");
        BI_Sqr_zx(&result, num0);

        bi_print_bigint_hex(result);
        printf(") :  cnt -= 1 ");
        puts("");

#elif (VERIFY_MODE == 6) // EXP_MOD Operation
        BIGINT* M = NULL;
        bi_gen_rand(&M, NON_NEGATIVE, 7);

        printf("pow(");
        BI_ExpMod_zx(&result, num0, num1, M);
        bi_print_bigint_hex(num0);
        printf(", ");
        bi_print_bigint_hex(num1);
        printf(", ");
        bi_print_bigint_hex(M);
        printf(") != ");
        bi_print_bigint_hex(result);
        printf(") : cnt -=1");
        puts("");
        bi_delete(&M);

#endif        
        bi_delete(&num0);
        bi_delete(&num1);
        bi_delete(&result);
    }
    printf("print(\"                %%s/");
    printf("%d", REPEAT_COUNT);
    printf(" \"%%format(cnt))\n");
    printf("print(\" ---------------------------------------- \")\n");

    return 0;
}