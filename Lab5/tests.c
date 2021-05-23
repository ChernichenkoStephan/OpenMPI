#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* clear && gcc -o test tests.c && ./test
* clear && gcc -o test tests.c Long_Word/long_word.c && ./test
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

  printf("Long_Word_mult_karatsuba: 493_817_284\n");
  a.numbers = an;
  b.numbers = bn;
  a.word_length = size;
  b.word_length = size;
  Long_Word res0 = Long_Word_mult_karatsuba(a, b);
  Long_Word_print(&res0);

  printf("\nLong_Word_sum:\n");
  Long_Word res1 = Long_Word_sum(a, b);
  Long_Word_print(&res1);

  for (size_t i = 0; i < a.word_length; i++) a.numbers[i] = 3;
  printf("\nLong_Word_sub:\n");
  Long_Word_sub(&a, b);
  Long_Word_print(&a);

  for (size_t i = 0; i < a.word_length; i++) a.numbers[i] = 10;
  printf("\nLong_Word_normalize:\n");
  Long_Word_normalize(a);
  Long_Word_print(&a);

  printf("\nLong_Word_to_DTO:\n");
  Long_Word_DTO baz = Long_Word_to_DTO(&a);
  Long_Word_DTO_print(&baz);

  printf("\nLong_Word_from_DTO:\n");
  Long_Word bar = Long_Word_from_DTO(&baz);
  Long_Word_print(&bar);

  return 0;
}
