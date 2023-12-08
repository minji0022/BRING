#include <stdio.h>
#include "rsa_pss.h"
#include "sha256.h"
#include <assert.h>

// RSA encryption
void encrypt(BIGINT** encrypted,
    BIGINT* message,
    BIGINT* e,
    BIGINT* n) {
    BI_ExpMod_zx(encrypted, message, e, n);
}

// RSA decryption
void decrypt(BIGINT** original,
    BIGINT* encrypted,
    BIGINT* d,
    BIGINT* n) {
    BI_ExpMod_zx(original, encrypted, d, n);
}