CC = gcc

all : bignum bring_test verify_code

# BRING의 제공 연산에 대한 예제 코드
bring_test : bignum.dylib examples/bring_arith_test.c
	$(CC) -Wall -g examples/bring_arith_test.c -o examples/bring_arith_test bignum.dylib

# BRING의 제공 연산에 대한 검증 파이썬 코드 출력
verify_code : bignum.dylib bring_verify.c
	$(CC) -Wall -g bring_verify.c -o bring_verify bignum.dylib
# SETTING 1: "MacOS, Linux"의 경우 다음 명령어로 실행	
	./bring_verify > bring_verify.py
# SETTING 2: "Window"의 경우 다음 명령어로 실행	
#	bring_verify > bring_verify.py

# BRING 동적 라이브러리 생성
bignum : bring_util.c bring_add.c bring_sub.c bring_mul.c bring_div.c bring_arith.c
	$(CC) -shared -fPIC -o bignum.dylib bring_util.c bring_add.c bring_sub.c bring_div.c bring_mul.c bring_arith.c

# 실행 파일 삭제
clean:
	rm examples/bring_arith_test bring_verify