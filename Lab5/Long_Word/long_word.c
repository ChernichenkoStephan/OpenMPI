#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "long_word.h"

Long_Word Long_Word_sum(Long_Word a, Long_Word b) {
  Long_Word s;
  s.length = a.length + 1;
  s.nums = (int*) malloc(s.length * sizeof(int));

  s.nums[a.length - 1] = a.nums[a.length - 1];
  s.nums[a.length] = 0;
  for (long i = 0; i < b.length; ++i)
    s.nums[i] = a.nums[i] + b.nums[i];
  return s;
}

Long_Word* Long_Word_sub(Long_Word* a, Long_Word b) {
  for (long i = 0; i < b.length; ++i)
    a->nums[i] -= b.nums[i];
  return a;
}

void Long_Word_normalize(Long_Word l) {
  for (long i = 0; i < l.length - 1; ++i) {
    if (l.nums[i] >= 10) {
      int carryover = l.nums[i] / 10;
      l.nums[i + 1] += carryover;
      l.nums[i] -= carryover * 10;
    } else if (l.nums[i] < 0) {
      int carryover = (l.nums[i] + 1) / 10 - 1;
      l.nums[i + 1] += carryover;
      l.nums[i] -= carryover * 10;
    }
  }
}

Long_Word mult_karatsuba(Long_Word a, Long_Word b) {
  Long_Word res;
  res.length = a.length + b.length;
  res.nums = (int*) malloc(res.length * sizeof(int));

  if (a.length < 4) {
    memset(res.nums, 0, sizeof(int) * res.length);
    for (long i = 0; i < a.length; ++i)
      for (long j = 0; j < b.length; ++j) {
        res.nums[i + j] += a.nums[i] * b.nums[j];
      }
  } else {
    Long_Word a_part1;
    a_part1.nums = a.nums;
    a_part1.length = (a.length + 1) / 2;

    Long_Word a_part2;
    a_part2.nums = a.nums + a_part1.length;
    a_part2.length = a.length / 2;

    Long_Word b_part1;
    b_part1.nums = b.nums;
    b_part1.length = (b.length + 1) / 2;

    Long_Word b_part2;
    b_part2.nums = b.nums + b_part1.length;
    b_part2.length = b.length / 2;

    Long_Word sum_of_a_parts = Long_Word_sum(a_part1, a_part2);
    Long_Word_normalize(sum_of_a_parts);
    Long_Word sum_of_b_parts = Long_Word_sum(b_part1, b_part2);
    Long_Word_normalize(sum_of_b_parts);
    Long_Word res_of_sums_of_parts = mult_karatsuba(sum_of_a_parts, sum_of_b_parts);

    Long_Word res_of_first_parts = mult_karatsuba(a_part1, b_part1);
    Long_Word res_of_second_parts = mult_karatsuba(a_part2, b_part2);
    Long_Word sum_of_middle_terms = *Long_Word_sub(Long_Word_sub(&res_of_sums_of_parts, res_of_first_parts), res_of_second_parts);

    memcpy(res.nums, res_of_first_parts.nums,
      res_of_first_parts.length * sizeof(int));
    memcpy(res.nums + res_of_first_parts.length,
      res_of_second_parts.nums, res_of_second_parts.length
      * sizeof(int));
    for (long i = 0; i < sum_of_middle_terms.length; ++i)
      res.nums[a_part1.length + i] += sum_of_middle_terms.nums[i];

    free(sum_of_a_parts.nums);
    free(sum_of_b_parts.nums);
    free(res_of_sums_of_parts.nums);
    free(res_of_first_parts.nums);
    free(res_of_second_parts.nums);
  }

  Long_Word_normalize(res);

  return res;
}

void Long_Word_print(Long_Word* word) {
  int size, i = word->length;
  do {
    i--;
    size = i;
  } while (word->nums[i] == 0);
  for (i = size; i >= 0; i--) printf("%d", word->nums[i]);
}
