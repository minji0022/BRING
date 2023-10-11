# BRING
BRING: BIG NumbeR c library !

Advanced Application Programming Course Project. This project aims to implement a library for large number arithmetic using the C programming language.

## Contributors

Team **PINOKIO's** with Kookmin Univ. in Korea.
- Minji Kim @minji0022
- Donghyun Park
- Hyebin Noh
- Yeji Noh(?)

## Building
```
gcc -shared -fPIC -o bignum.dylib bring_util.c 
gcc -Wall -g bring_test.c -o bring_test bignum.dylib
./bring_test
```