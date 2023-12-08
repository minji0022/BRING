#include <stdio.h>
#include "rsa_pss.h"
#include "sha256.h"
#include <assert.h>
#include "rsa.h"

void display_rsa(BIGINT* message, BIGINT* n, BIGINT* e, BIGINT* d) {
    BIGINT* encrypted = NULL;
    BIGINT* decrypted = NULL;
    bi_set_zero(&encrypted);
    bi_set_zero(&decrypted);

    encrypt(&encrypted, message, e, n);
    decrypt(&decrypted, encrypted, d, n);

    // The decrypted message must be equal to the original
    assert(bi_compare_bigint(message, decrypted) == 0);

    printf("[Public key] \n(e: ");
    bi_print_bigint_hex(e);
    printf(", \nn: ");
    bi_print_bigint_hex(n);
    printf(")\n");
    printf("[Private key] \n(d: ");
    bi_print_bigint_hex(d);
    printf(", \nn: ");
    bi_print_bigint_hex(n);
    printf(")\n");
    printf("\nOriginal message: ");
    bi_print_bigint_hex(message);
    printf("\nEncrypted message: ");
    bi_print_bigint_hex(encrypted);
    printf("\nDecrypted message: ");
    bi_print_bigint_hex(decrypted);
    printf("\n");

    bi_delete(&encrypted);
    bi_delete(&decrypted);
}

void generate_random_salt(uint8_t* salt, size_t salt_length) {
    for (size_t i = 0; i < salt_length; ++i) {
        salt[i] = rand() % 256;
    }
}

int main() {
    BIGINT* msg = NULL;
    BIGINT* n = NULL;
    BIGINT* d = NULL;
    BIGINT* e = NULL;
    bi_set_by_string(&msg, NON_NEGATIVE, "d436e99569fd32a7c8a05bbc90d32c49", HEXDECIMAL);
    bi_set_by_string(&n, NON_NEGATIVE, "C30F078A843037A4835869B828BAFCDA24C80AE2942D2EB507476BE4D948D410AFD489A2B8ED75A7F88BED40C6D89604E3510228B33848EDABBF0E3757EE64ADBD91C55542344BAE68EB7E7970D63231118C7C4A7A36413600B93F9208C0C5EBF7E0BE473F351C2EE661C4BDB396962878EA236F28C677B6C6BA8B143B88E15827DE4D6E9C7B7382FB3BBB91E26BEA81485C6DF4636AC4E2AC2894E0AA319CE6FF3540AB34A7471FE0EF42AA69728E8D20579CF3668FB9D8644896D90E257E22F3FBFF471BDF1998E710E5E2217405EB1F2FBFCAAD2C5A8BD5E22B7FE8C5EC6CC8A5484C0EBD1BC5F8BEC1C71895A5FE39CD0F6BD85B55BD256467411050B6B5", HEXDECIMAL);
    bi_set_by_string(&d, NON_NEGATIVE, "947C3F70597A5F11340F3CC33CF642FF489AA376A1F7D723657C200D20BF08F7FEF19B28ACFFCEDDE41D98A903F5E215AC79815C3C39EF8245887628F91DF7B42EA7E6012619AB370495A83C8801F64956238C86218C658DD7752D885C736AF75B45D173691163F7EBD6A72F05A3EF80B5B2FEF062A8DA7EDFBF279A6A4BA3C5E237458C98AAABDBF68D3BB24968E1780F81ACE47292299D6BB371BC2A16FCED234319F622288AA1B44CE938573E067F19CA5E5D2AB2DBF699F1B348A5F885F4FEF1B38780B1E07BD55EF6CE2C0B0A8384E77E208640AE4631019DABDD2C0782914A8631C6E4F011618AB71D0A0FF76CAB405744B5DA5B9F6A3598A8E4F4656D", HEXDECIMAL);
    bi_set_by_string(&e, NON_NEGATIVE, "010001", HEXDECIMAL);

    printf("RSA in progress...\n");
    display_rsa(msg, n, e, d);

    printf("RSA-PSS signing in progress...\n");

    size_t salt_length = 32;  // 원하는 솔트 길이
    uint8_t* salt = (uint8_t*)malloc(salt_length);
    generate_random_salt(salt, salt_length);

    BIGINT* signature = NULL;
    
    size_t modulus_size = bi_sizeof_bytes(n);
    size_t em_length = (modulus_size * 8 + 7) / 8;
    uint8_t* em_s = (uint8_t*)malloc(em_length);

    rsa_pss_sign(&signature, em_s, msg, d, n, salt, salt_length);

    printf("RSA-PSS verification in progress...\n");

    printf("\nRSA Signature: ");
    bi_print_bigint_hex(signature);
    printf("\n");

    int verification_result = rsa_pss_verify(signature, em_s, msg, e, n, 32);

    if (verification_result) {
        printf("RSA-PSS verification succeeded!\n");
    } else {
        printf("RSA-PSS verification failed.\n");
    }

    bi_delete(&msg);
    bi_delete(&n);
    bi_delete(&d);
    bi_delete(&e);
    bi_delete(&signature);
    free(em_s);

    return 0;
}