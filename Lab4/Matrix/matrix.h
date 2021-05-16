#pragma once

typedef struct {
	int real;
	int imaginary;
} Complex_Number;

typedef struct {
  int size;
	Complex_Number** values;
} Complex_Matrix;

Complex_Matrix* Matrix_alloc(const int amount, const int matrix_size);
void Matrix_free(Complex_Matrix* mtx, const int amount);
void Matrix_copy(Complex_Matrix* to, Complex_Matrix* from);

Complex_Number* Matrix_to_array(const Complex_Matrix* mtx_arr, const int amount);
void Matrix_from_array(const Complex_Number* arr, const Complex_Matrix* mtx_arr, const int amount);

Complex_Matrix* Matrix_get_rand_matrixes(const int amount, const int matrix_size);
Complex_Matrix* Matrix_get_empty_matrixes(const int amount, const int matrix_size);

void Matrix_acc_multiply(Complex_Matrix* mtx_arr, const int amount, Complex_Matrix* res_mtx);
void Matrix_multiply(const Complex_Matrix* l_mtx, const Complex_Matrix* r_mtx, Complex_Matrix* res_mtx);

void Matrix_print(const Complex_Matrix* mtx);
void Matrix_mult_print(const Complex_Matrix* mtx_arr, const int amount);
