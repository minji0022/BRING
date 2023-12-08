#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "sha256.h"
#include "rsa.h"

// Export a BigInt to a uint8_t array
void bi_export_to_uint8(BIGINT* num, uint8_t* output, size_t output_size) {
    printf("export test\nnum: ");
    bi_print_bigint_hex(num);
    printf("\noutput: ");
    
    // Check for NULL pointers and size compatibility
    if (num == NULL || output == NULL) {
        // Handle error: NULL pointer
        return;
    }

    // Compute the word length required for the BigInt
    size_t num_words = bi_get_word_length(num);

    if (output_size < num_words * sizeof(word)) {
        // Handle error: Output buffer is too small
        return;
    }

    // Copy BigInt data to the output buffer
    for (size_t i = num_words; i > 0; --i) {
        word current_word = num->p[i - 1];
        for (size_t j = 0; j < sizeof(word); ++j) {
            output[(num_words - i) * sizeof(word) + j] = (uint8_t)((current_word >> (8 * (sizeof(word) - 1 - j))) & 0xFF);
        }
    }

    for (size_t i = 0; i < output_size; ++i) {
        printf("%02x", output[i]);
    }
    printf("\n");
}


// Import a BigInt from a uint8_t array
void bi_import_from_uint8(BIGINT** num, uint8_t* input, size_t input_size, int sign) {
    // Check for NULL pointers
    if (input == NULL) {
        // Handle error: NULL pointer
        return;
    }

    // Compute the word length required for the BigInt
    size_t num_words = (input_size + sizeof(word) - 1) / sizeof(word);

    // Create a new BigInt
    bi_new(num, num_words);
    
    printf("import test\ninput: ");
    for (size_t i = 0; i < input_size; ++i) {
        printf("%02x", input[i]);
    }
    printf("\nnum: ");

    // Copy the data from the input buffer to the BigInt
    for (size_t i = num_words; i > 0; --i) {
        for (size_t j = 0; j < sizeof(word); ++j) {
            (*num)->p[i - 1] |= ((word)input[(num_words - i) * sizeof(word) + (sizeof(word) - 1 - j)] << (8 * j));
        }
    }

    bi_print_bigint_hex(*num);
    printf("\n");
}

// MGF1 (Mask Generation Function)
static void mgf1(uint8_t* seed, size_t seed_length, uint8_t* mask, size_t mask_length) {
    size_t hlen = SHA256_DIGEST_SIZE;  // SHA-256의 출력 길이

    // 계산에 필요한 블록 수
    size_t block_count = (mask_length + hlen - 1) / hlen;

    // 마스크 초기화
    memset(mask, 0, mask_length);

    for (size_t counter = 0; counter < block_count; ++counter) {
        // 현재 카운터 값을 빅엔디안 바이트 배열로 변환
        uint8_t counter_bytes[4];
        counter_bytes[0] = (uint8_t)((counter >> 24) & 0xFF);
        counter_bytes[1] = (uint8_t)((counter >> 16) & 0xFF);
        counter_bytes[2] = (uint8_t)((counter >> 8) & 0xFF);
        counter_bytes[3] = (uint8_t)(counter & 0xFF);

        // MGF1 마스킹
        uint8_t hash_input[seed_length + 4];
        memcpy(hash_input, seed, seed_length);
        memcpy(hash_input + seed_length, counter_bytes, sizeof(counter_bytes));

        // SHA-256 해시 계산
        SHA256_CTX sha256_ctx;
        sha256_init(&sha256_ctx);
        sha256_update(&sha256_ctx, hash_input, seed_length + sizeof(counter_bytes));
        sha256_final(&sha256_ctx, mask + counter * hlen);

        // 마스크 길이가 부족한 경우 나머지만큼 잘라냄
        if (counter == block_count - 1) {
            size_t remaining = mask_length % hlen;
            if (remaining > 0) {
                memcpy(mask + counter * hlen, mask + counter * hlen, remaining);
            }
        }
    }
}

// XOR Masking
static void xor_mask(uint8_t* mgf_mask, size_t mask_length, uint8_t* data, size_t data_length) {
    // XOR 연산 수행
    for (size_t i = 0; i < mask_length && i < data_length; ++i) {
        data[i] ^= mgf_mask[i];
    }
}


// RSA-PSS Encoding
void rsa_pss_encode(uint8_t* em, uint8_t* hash, size_t hash_length, size_t em_length, uint8_t* salt, size_t salt_length) {
    // EMSA-PSS 인코딩

    /*
    // Length of padding (8 for trailer)
    size_t ps_length = em_length - m_hash_length - s_length - 2;

    // Generate mask
    uint8_t mgf_mask[m_hash_length];
    mgf1(m_hash, m_hash_length, mgf_mask, sizeof(mgf_mask));

    // XOR mask
    for (size_t i = 0; i < m_hash_length; ++i) {
        em[i] ^= mgf_mask[i];
    }

     // Print the intermediate result after XOR masking
    printf("After XOR Masking: ");
    for (int i = 0; i < m_hash_length; i++) {
        printf("%02x", em[i]);
    }
    printf("\n");

    // Apply DB
    memset(em + m_hash_length, 0, ps_length);  // Pad with zeros
    em[m_hash_length + ps_length] = 0x01;      // Set leftmost byte to 0x01
    memcpy(em + m_hash_length + ps_length + 1, m_hash, m_hash_length);  // Copy hash

    // Generate H
    SHA256_CTX sha256_ctx;
    sha256_init(&sha256_ctx);
    sha256_update(&sha256_ctx, em, em_length);
    sha256_final(&sha256_ctx, em);
    */
    printf("rsa pss encoding\n");
    
    // DB 생성 (Data Block)
    uint8_t* DB = (uint8_t*)malloc(hash_length + salt_length);
    
    // 해시 값 복사
    memcpy(DB, hash, hash_length);

    // 솔트 값 복사
    memcpy(DB + hash_length, salt, salt_length);

    // 생성된 DB를 em에 복사
    memcpy(em + salt_length + 1, DB, hash_length + salt_length);

    // DB 마스크 생성
    uint8_t db_mask[salt_length];
    mgf1(db_mask, salt_length, DB, hash_length);

    // DB 값에 DB 마스크 XOR 연산
    for (size_t i = 0; i < salt_length; ++i) {
        DB[hash_length + i] ^= db_mask[i];
    }

    // DB 값이 em 전체를 차지하지 않는 경우 나머지 부분을 0으로 초기화
    if (hash_length + salt_length < em_length) {
        memset(DB + hash_length + salt_length, 0, em_length - hash_length - salt_length);
    }

    // DB 값 연결
    memcpy(em, DB, hash_length + salt_length);

    // PSS 패딩 앞부분에 0x01 바이트 추가
    em[hash_length + salt_length] = 0x01;

    // DB' 추가: 0xBC 바이트 추가
    memset(em + hash_length + salt_length + 1, 0xBC, em_length - hash_length - salt_length - 1);

    // Salt 값 추가
    memcpy(em + em_length - salt_length, DB + hash_length, salt_length);

    // DB' 마스크 생성
    uint8_t db_prime_mask[hash_length];
    mgf1(db_prime_mask, hash_length, DB, hash_length + salt_length);

    // DB' 마스크 XOR 연산
    for (size_t i = 0; i < hash_length; ++i) {
        DB[hash_length + i] ^= db_prime_mask[i];
    }

    memcpy(em, DB, hash_length + salt_length + 1);

    // DB 메모리 해제
    free(DB);

    // Print the SHA-256 hash
    printf("SHA-256 Hash: ");
    for (int i = 0; i < em_length; i++) {
        printf("%02x", em[i]);
    }
    printf("\n");
}

// 8비트 비트 반전 (Bit Reverse)
static void bit_reverse(uint8_t* array, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        array[i] = ~array[i];
    }
}

// RSA-PSS Verify Encoding
int rsa_pss_verify_encoding(uint8_t* em, size_t em_length, uint8_t* hash, size_t hash_length, size_t salt_length) {
    /*
    // MGF1 마스킹
    uint8_t* mgf_mask = (uint8_t*)malloc(em_length);
    mgf1(hash, hash_length, mgf_mask, em_length);

    // XOR 마스킹
    uint8_t* masked_message = (uint8_t*)malloc(em_length);
    memcpy(masked_message, encoded_message, em_length);
    xor_mask(mgf_mask, em_length, masked_message, em_length);

    // 8비트 비트 반전 (Bit Reverse)
    bit_reverse(masked_message, em_length);

    // 비트 검증
    size_t zero_bits = 8 * sizeof(size_t) - hash_length - salt_length - 2;
    printf("0x%02X\n", ((masked_message[0] >> (8- zero_bits)) & 0x01));
    if (((masked_message[0] >> (8- zero_bits)) & 0x01) == 1) {
        // 검증 실패: 첫 번째 바이트의 앞부분이 0x01이 아님
        printf("f\n");
        free(mgf_mask);
        free(masked_message);
        return 0;
    }

    for (size_t i = 1; i < zero_bits; ++i) {
        if ((masked_message[i >> 3] & (1 << (7 - (i & 7)))) != 0) {
            // 검증 실패: 0 비트가 아닌 비트 발견
            free(mgf_mask);
            free(masked_message);
            return 0;
        }
    }

    if (masked_message[zero_bits >> 3] != 0x01) {
        // 검증 실패: 0 비트 다음의 바이트가 0x01이 아님
        free(mgf_mask);
        free(masked_message);
        return 0;
    }

    free(mgf_mask);
    free(masked_message);
    return 1;  // 검증 성공
    */


    // Check if the length of em is sufficient
    if (em_length < hash_length + salt_length + 2) {
        // Handle error: Invalid PSS padding length
        return 0;  // PSS decoding failure
    }

    // Extract salt from PSS padding
    uint8_t* salt = em + em_length - salt_length - 1;

    // Calculate the length of MGF mask
    size_t mgf_mask_length = em_length - hash_length - 1;

    // MGF1 mask generation
    uint8_t* mgf_mask = (uint8_t*)malloc(mgf_mask_length);
    mgf1(mgf_mask, mgf_mask_length, salt, salt_length);

    // XOR operation to obtain m'
    uint8_t* m_prime = (uint8_t*)malloc(mgf_mask_length);
    for (size_t i = 0; i < mgf_mask_length; ++i) {
        m_prime[i] = em[i] ^ mgf_mask[i];
    }

    // Set the leading bits of m' to zero
    m_prime[0] &= 0xFF >> (8 * em_length - 8 * hash_length - 1);

    // Extract hash from m'
    uint8_t* extracted_hash = m_prime + mgf_mask_length - hash_length;

    // Compare extracted hash with the provided hash
    int hash_compare = memcmp(extracted_hash, hash, hash_length);

    // Clean up allocated memory
    free(mgf_mask);
    free(m_prime);

    // Return 1 if hashes match, 0 otherwise
    return hash_compare == 0;
}

size_t bi_sizeof_bytes(BIGINT* num) {
    // Check for NULL pointer
    if (num == NULL) {
        // Handle error: NULL pointer
        return 0;
    }

    // Compute the number of bytes required to represent the BigInt
    size_t num_bytes = num->wordlen * sizeof(word);

    // Trim leading zero bytes
    while (num_bytes > sizeof(word) && num->p[num->wordlen - 1] == 0) {
        num_bytes -= sizeof(word);
    }

    return num_bytes;
}

void rsa_pss_sign(BIGINT** signature, BIGINT* message, BIGINT* d, BIGINT* n, uint8_t* salt, size_t salt_length) {
    // Compute SHA-256 hash of the message
    SHA256_CTX sha256_ctx;
    uint8_t hash[SHA256_DIGEST_SIZE];
    sha256_init(&sha256_ctx);
    
    // Export BigInt to uint8 buffer
    size_t message_size = bi_get_word_length(message) * sizeof(word);
    uint8_t* message_buffer = (uint8_t*)malloc(message_size);
    bi_export_to_uint8(message, message_buffer, message_size);
    
    sha256_update(&sha256_ctx, message_buffer, message_size);
    sha256_final(&sha256_ctx, hash);

    // 얻은 해시 값 출력
    printf("Hash: ");
    for (int i = 0; i < SHA256_DIGEST_SIZE; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");

    // Calculate em_length based on the RSA modulus size
    size_t modulus_size = bi_sizeof_bytes(n);
    size_t em_length = (modulus_size * 8 + 7) / 8;

    // Compute the PSS encoding of the hash
    uint8_t* em = (uint8_t*)malloc(em_length);
    rsa_pss_encode(em, hash, SHA256_DIGEST_SIZE, em_length, salt, salt_length);

    // Convert the PSS encoding to a BigInt
    BIGINT* pm = NULL;
    printf("em_length: %d\n", em_length);
    bi_import_from_uint8(&pm, em, em_length, NON_NEGATIVE);
    
    printf("encoded sign: ");
    bi_print_bigint_hex(pm);
    printf("\nRSA signature: ");
    encrypt(signature, pm, d, n);
    bi_print_bigint_hex(*signature);
    printf("\n");

    free(em);
    free(message_buffer);
}

int rsa_pss_verify(BIGINT* signature, BIGINT* message, BIGINT* e, BIGINT* n, size_t salt_length) {
    BIGINT* pm = NULL;
    printf("\ndecoded sign: ");
    decrypt(&pm, signature, e, n);
    bi_print_bigint_hex(pm);
    printf("\n");

    // Convert the signature to a PSS encoding
    size_t em_length = bi_get_word_length(n) * sizeof(word);
    uint8_t* em = (uint8_t*)malloc(em_length);
    printf("Signature before decoding: ");
    bi_export_to_uint8(pm, em, em_length);

    // Compute SHA-256 hash of the message
    SHA256_CTX sha256_ctx;
    uint8_t hash[SHA256_DIGEST_SIZE];
    sha256_init(&sha256_ctx);

    // Export BigInt to uint8 buffer
    size_t message_size = bi_get_word_length(message) * sizeof(word);
    uint8_t* message_buffer = (uint8_t*)malloc(message_size);
    bi_export_to_uint8(message, message_buffer, message_size);

    sha256_update(&sha256_ctx, message_buffer, message_size);
    sha256_final(&sha256_ctx, hash);

    // 얻은 해시 값 출력
    printf("Hash: ");
    for (int i = 0; i < SHA256_DIGEST_SIZE; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");

    // Verify the PSS encoding
    int result = rsa_pss_verify_encoding(em, SHA256_DIGEST_SIZE, hash, em_length, salt_length);

    free(em);
    free(message_buffer);

    return result;
}