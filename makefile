CC = gcc

all : bignum bring_test verify_code

bring_test : bignum.dylib bring_test.c
	$(CC) -Wall -g bring_test.c -o bring_test bignum.dylib

# 검증 파이썬 코드 출력 버전
verify_code : bignum.dylib bring_verify.c
	$(CC) -Wall -g bring_verify.c -o bring_verify bignum.dylib
	./bring_verify > bring_verify.py

bignum : bring_util.c bring_add.c bring_sub.c bring_mul.c bring_div.c bring_arith.c
	$(CC) -shared -fPIC -o bignum.dylib bring_util.c bring_add.c bring_sub.c bring_div.c bring_mul.c bring_arith.c

clean:
	rm bring_test bring_verify