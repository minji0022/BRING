#include <stdio.h>
#include "rsa_pss.h"
#include "sha256.h"
#include <assert.h>

void encrypt(BIGINT** encrypted, BIGINT* message, BIGINT* e, BIGINT* n);
void decrypt(BIGINT** original, BIGINT* encrypted, BIGINT* d, BIGINT* n);