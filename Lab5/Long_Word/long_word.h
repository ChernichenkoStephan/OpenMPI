#pragma once

#define MAX_WORD_LENGTH 20

typedef struct {
  int *numbers;
  long word_length;
} Long_Word;

typedef struct {
  int  numbers[MAX_WORD_LENGTH];
  long word_length;
} Long_Word_DTO;

Long_Word  Long_Word_sum(Long_Word a, Long_Word b);
Long_Word* Long_Word_sub(Long_Word* a, Long_Word b);
Long_Word  Long_Word_mult_karatsuba(Long_Word a, Long_Word b);

void Long_Word_normalize(Long_Word l);

Long_Word_DTO Long_Word_to_DTO(Long_Word* word);
Long_Word Long_Word_from_DTO(Long_Word_DTO* dto);

void Long_Word_print(Long_Word* word);
void Long_Word_DTO_print(Long_Word_DTO* dto);
