#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* clear && gcc -o test tests.c && ./test
* clear && gcc -o test tests.c Long_Word/long_word.c && ./test
https://docs.microsoft.com/en-us/message-passing-interface/mpi-datatype-enumeration
*/

#include "Long_Word/long_word.h"

int main(int argc, char const *argv[]) {
  Long_Word a, b;
  int size = 5;
  int* an = (int*) malloc(size * sizeof(int));
  int* bn = (int*) malloc(size * sizeof(int));
  for (size_t i = 0; i < 5; i++) {
    an[i] = 2;
    bn[i] = 2;
  }

  a.nums = an;
  b.nums = bn;
  a.length = size;
  b.length = size;
  Long_Word res = mult_karatsuba(a, b);

  Long_Word_print(&res);

  return 0;
}
