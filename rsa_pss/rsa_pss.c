#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "sha256.h"
#include "rsa.h"

// Export a BigInt to a uint8_t array
void bi_export_to_uint8(BIGINT* num, uint8_t* output, size_t output_size) {
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

    // Copy the data from the input buffer to the BigInt
    for (size_t i = num_words; i > 0; --i) {
        for (size_t j = 0; j < sizeof(word); ++j) {
            (*num)->p[i - 1] |= ((word)input[(num_words - i) * sizeof(word) + (sizeof(word) - 1 - j)] << (8 * j));
        }
    }
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

// RSA-PSS Encoding
void rsa_pss_encode(uint8_t* em, uint8_t* hash, size_t hash_length, size_t em_length, uint8_t* salt, size_t salt_length) {
    // EMSA-PSS 인코딩
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
}

// RSA-PSS Verify Encoding
int rsa_pss_verify_encoding(uint8_t* em, size_t em_length, uint8_t* hash, size_t hash_length, size_t salt_length) {
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

void rsa_pss_sign(BIGINT** signature, uint8_t* em_s, BIGINT* message, BIGINT* d, BIGINT* n, uint8_t* salt, size_t salt_length) {
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

    size_t modulus_size = bi_sizeof_bytes(n);
    size_t em_length = (modulus_size * 8 + 7) / 8;

    // @2023.12.08 Encoding 문제 있음
    // rsa_pss_encode(em_s, hash, SHA256_DIGEST_SIZE, em_length, salt, salt_length);

    // Convert the PSS encoding to a BigInt
    BIGINT* pm = NULL;
    
    bi_import_from_uint8(&pm, em_s, em_length, NON_NEGATIVE);
    
    encrypt(signature, pm, d, n);

    free(message_buffer);
}

int rsa_pss_verify(BIGINT* signature, uint8_t* em_s, BIGINT* message, BIGINT* e, BIGINT* n, size_t salt_length) {
    BIGINT* pm = NULL;
    decrypt(&pm, signature, e, n);

    // Convert the signature to a PSS encoding
    size_t em_length = bi_get_word_length(n) * sizeof(word);
    uint8_t* em_v = (uint8_t*)malloc(em_length);
    bi_export_to_uint8(pm, em_v, em_length);

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

    // Verify the PSS encoding
    // int result = rsa_pss_verify_encoding(em, SHA256_DIGEST_SIZE, hash, em_length, salt_length);

    int are_equal = 1;

    for (int i = 0; i < sizeof(em_length); i++) {
        if (*(em_s + i) != *(em_v + i)) {
            are_equal = 0;  // 하나라도 다르면 0으로 설정하고 루프 종료
            break;
        }
    }

    free(em_v);
    free(message_buffer);

    return are_equal;
}