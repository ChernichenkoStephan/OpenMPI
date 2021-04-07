#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

float sum(float* array, int size) {
  float res = 0;
  for (size_t i = 0; i < size; i++)
    res += array[i];
  return res;
}

int get_ordered_num(int size) {
  int res = 10;
  for (size_t i = 0; i < size; i++) res*=10;
  return res;
}

/**
* example for 4 processes:
* [250, 250, 250, 250, 25, 25, 25, 25, 2.5, 2.5, 2.5, 2.5, 0.25, 0.25, 0.25, 0.25]
*/
float* get_source_array(int count) {
  float ordered_num = get_ordered_num(count - 2);
  float current_num;
  float* res = (float*) malloc(sizeof(float) * count * count);

  for (size_t i = 0; i < count; i++) {
    current_num = ordered_num / count;

    for (size_t j = 0; j < count; j++) {
      res[i * count + j] = current_num;
    }

    ordered_num /= 10;
  }

  return res;
}
