#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "long_word.h"

void Long_Word_normalize(Long_Word l) {
  for (long i = 0; i < l.word_length - 1; ++i) {
    if (l.numbers[i] >= 10) {
      int carryover = l.numbers[i] / 10;
      l.numbers[i + 1] += carryover;
      l.numbers[i] -= carryover * 10;
    } else if (l.numbers[i] < 0) {
      int carryover = (l.numbers[i] + 1) / 10 - 1;
      l.numbers[i + 1] += carryover;
      l.numbers[i] -= carryover * 10;
    }
  }
}

Long_Word_DTO Long_Word_to_DTO(Long_Word* word) {
  Long_Word_DTO res;
  res.word_length = word->word_length;
  for (size_t i = 0; i < word->word_length; i++)
    res.numbers[i] = word->numbers[i];
  return res;
}

Long_Word Long_Word_from_DTO(Long_Word_DTO* dto) {
  Long_Word res;
  res.numbers = (int*) malloc(dto->word_length * sizeof(int));
  res.word_length = dto->word_length;
  for (size_t i = 0; i < dto->word_length; i++)
    res.numbers[i] = dto->numbers[i];
  return res;
}

Long_Word Long_Word_sum(Long_Word a, Long_Word b) {
  Long_Word s;
  s.word_length = a.word_length + 1;
  s.numbers = (int*) malloc(s.word_length * sizeof(int));

  s.numbers[a.word_length - 1] = a.numbers[a.word_length - 1];
  s.numbers[a.word_length] = 0;
  for (long i = 0; i < b.word_length; ++i)
    s.numbers[i] = a.numbers[i] + b.numbers[i];
  return s;
}

Long_Word* Long_Word_sub(Long_Word* a, Long_Word b) {
  for (long i = 0; i < b.word_length; ++i)
    a->numbers[i] -= b.numbers[i];
  return a;
}

Long_Word Long_Word_mult_karatsuba(Long_Word a, Long_Word b) {
  Long_Word res;
  res.word_length = a.word_length + b.word_length;
  res.numbers = (int*) malloc(res.word_length * sizeof(int));

  if (a.word_length < 4) {
    memset(res.numbers, 0, sizeof(int) * res.word_length);
    for (long i = 0; i < a.word_length; ++i)
      for (long j = 0; j < b.word_length; ++j) {
        res.numbers[i + j] += a.numbers[i] * b.numbers[j];
      }
  } else {
    Long_Word a_part1;
    a_part1.numbers = a.numbers;
    a_part1.word_length = (a.word_length + 1) / 2;

    Long_Word a_part2;
    a_part2.numbers = a.numbers + a_part1.word_length;
    a_part2.word_length = a.word_length / 2;

    Long_Word b_part1;
    b_part1.numbers = b.numbers;
    b_part1.word_length = (b.word_length + 1) / 2;

    Long_Word b_part2;
    b_part2.numbers = b.numbers + b_part1.word_length;
    b_part2.word_length = b.word_length / 2;

    Long_Word sum_of_a_parts = Long_Word_sum(a_part1, a_part2);
    Long_Word sum_of_b_parts = Long_Word_sum(b_part1, b_part2);
    Long_Word_normalize(sum_of_a_parts);
    Long_Word_normalize(sum_of_b_parts);
    Long_Word res_of_sums_of_parts = Long_Word_mult_karatsuba(sum_of_a_parts, sum_of_b_parts);

    Long_Word res_of_first_parts  = Long_Word_mult_karatsuba(a_part1, b_part1);
    Long_Word res_of_second_parts = Long_Word_mult_karatsuba(a_part2, b_part2);
    Long_Word sum_of_middle_terms = *Long_Word_sub(Long_Word_sub(&res_of_sums_of_parts, res_of_first_parts), res_of_second_parts);

    memcpy(res.numbers, res_of_first_parts.numbers,
      res_of_first_parts.word_length * sizeof(int));
    memcpy(res.numbers + res_of_first_parts.word_length,
      res_of_second_parts.numbers, res_of_second_parts.word_length
      * sizeof(int));
    for (long i = 0; i < sum_of_middle_terms.word_length; ++i)
      res.numbers[a_part1.word_length + i] += sum_of_middle_terms.numbers[i];

    free(sum_of_a_parts.numbers);
    free(sum_of_b_parts.numbers);
    free(res_of_sums_of_parts.numbers);
    free(res_of_first_parts.numbers);
    free(res_of_second_parts.numbers);
  }

  Long_Word_normalize(res);

  return res;
}

void Long_Word_print(Long_Word* word) {
  int size, i = word->word_length;
  do {
    i--;
    size = i;
  } while (word->numbers[i] == 0);
  for (i = size; i >= 0; i--) {
    printf("%d", word->numbers[i]);
    if ((i % 3) == 0) printf(" ");
  }
  printf("\n");
}

void Long_Word_DTO_print(Long_Word_DTO* dto) {
  int size, i = dto->word_length;
  do {
    i--;
    size = i;
  } while (dto->numbers[i] == 0);
  for (i = size; i >= 0; i--) {
    printf("%d", dto->numbers[i]);
    if ((i % 3) == 0) printf(" ");
  }
  printf("\n");
}
