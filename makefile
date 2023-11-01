bring_test : bignum.dylib bring_test.c
	gcc -Wall -g bring_test.c -o bring_test bignum.dylib

bignum.dylib : bring_util.c bring_add.c bring_sub.c
	gcc -shared -fPIC -o bignum.dylib bring_util.c bring_add.c bring_sub.c

clean:
	rm bring_test