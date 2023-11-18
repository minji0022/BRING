CC = gcc

all : bring_test verify_code bignum

bring_test : bignum.dylib bring_test.c
	gcc -Wall -g bring_test.c -o bring_test bignum.dylib

# 검증 파이썬 코드 출력 버전
verify_code : bignum.dylib bring_verify_python_code.c
	gcc -Wall -g bring_verify_python_code.c -o bring_verify bignum.dylib

bignum : bring_util.c bring_add.c bring_sub.c bring_mul.c
	gcc -shared -fPIC -o bignum.dylib bring_util.c bring_add.c bring_sub.c bring_mul.c

clean:
	rm bring_test