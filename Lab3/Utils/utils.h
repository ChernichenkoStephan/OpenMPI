#pragma once

float sum(float* array, int size);

/**
* examples:
* size = 4 -> 1000
* size = 5 -> 10000 ...
*/
int get_ordered_num(int size);

/**
* example for 4 processes:
* [250, 250, 250, 250, 25, 25, 25, 25, 2.5, 2.5, 2.5, 2.5, 0.25, 0.25, 0.25, 0.25]
*/
float* get_source_array(int count);
