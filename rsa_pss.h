#ifndef RSA_PSS_H
#define RSA_PSS_H

#include <stdint.h>
#include "bring_arith.h"

// Export a BigInt to a uint8_t array
void bi_export_to_uint8(BIGINT* num, uint8_t* output, size_t output_size);

// Import a BigInt from a uint8_t array
void bi_import_from_uint8(BIGINT** num, uint8_t* input, size_t input_size, int sign);

// Compute the size of bytes required to represent a BigInt
size_t bi_sizeof_bytes(BIGINT* num);

// RSA-PSS Sign
void rsa_pss_sign(BIGINT** signature, uint8_t* em_s, BIGINT* message, BIGINT* d, BIGINT* n, uint8_t* salt, size_t salt_length);

// RSA-PSS Verify
int rsa_pss_verify(BIGINT* signature, uint8_t* em_s, BIGINT* message, BIGINT* e, BIGINT* n, size_t salt_length);

#endif  // RSA_PSS_H