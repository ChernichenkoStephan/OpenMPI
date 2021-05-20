#pragma once

typedef struct {
  int *nums;
  long length;
} Long_Word;

Long_Word Long_Word_sum(Long_Word a, Long_Word b);
Long_Word* Long_Word_sub(Long_Word* a, Long_Word b);
Long_Word mult_karatsuba(Long_Word a, Long_Word b);

void Long_Word_normalize(Long_Word l);

int* Long_Word_to_pack(Long_Word* word);
void Long_Word_print(Long_Word* word);
