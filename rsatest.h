#ifndef RSATEST_H
#define RSATEST_H

#include "bring_arith.h"
#include <assert.h>

void encrypt(BIGINT** encrypted, BIGINT* message, BIGINT* e, BIGINT* n);
void decrypt(BIGINT** original, BIGINT* encrypted, BIGINT* d, BIGINT* n);
void display_rsa(BIGINT* message, BIGINT* n, BIGINT* e, BIGINT* d);

#endif  // RSATEST_H
