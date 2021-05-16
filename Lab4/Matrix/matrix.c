#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

Complex_Number** alloc_complex_matrix(const int matrix_size) {
  Complex_Number** res = (Complex_Number**) malloc(matrix_size * sizeof(Complex_Number*));

  for(int i = 0; i < matrix_size; i++)
    res[i] = (Complex_Number*) malloc(matrix_size * sizeof(Complex_Number));

  return res;
}

void free_complex_matrix(Complex_Number** mtx, const int matrix_size) {
  for (size_t i = 0; i < matrix_size; i++) free(mtx[i]);
  free(mtx);
}

Complex_Matrix* Matrix_alloc(const int amount, const int matrix_size) {
  Complex_Matrix* res = (Complex_Matrix*) malloc(amount * sizeof(Complex_Matrix));

  for (size_t i = 0; i < amount; i++)
    res[i].values = alloc_complex_matrix(matrix_size);

  return res;
}

void Matrix_free(Complex_Matrix* mtx, const int amount) {
  for(int i = 0; i < amount; i++)
    free_complex_matrix(mtx[i].values, mtx->size);
  free(mtx);
}

Complex_Number* Matrix_to_array(const Complex_Matrix* mtx_arr, const int amount) {
  int matrix_size = mtx_arr->size;
  Complex_Number* res = (Complex_Number*) malloc(matrix_size * matrix_size * sizeof(Complex_Number));
  for (size_t i = 0; i < amount; i++)
    for (size_t j = 0; j < matrix_size; j++)
      for (size_t k = 0; k < matrix_size; k++) {
        int index = (i * matrix_size * matrix_size) + (j * matrix_size) + k;
        res[index] = mtx_arr[i].values[j][k];
      }
  return res;
}

void Matrix_from_array(const Complex_Number* arr, const Complex_Matrix* mtx_arr, const int amount) {
  int matrix_size = mtx_arr->size;
  for (size_t i = 0; i < amount; i++)
    for (size_t j = 0; j < matrix_size; j++)
      for (size_t k = 0; k < matrix_size; k++) {
        int index = (i * matrix_size * matrix_size) + (j * matrix_size) + k;
        mtx_arr[i].values[j][k] = arr[index];
      }
}

Complex_Matrix* Matrix_get_rand_matrixes(const int amount, const int matrix_size) {
  Complex_Matrix* res = Matrix_alloc(amount, matrix_size);

  for (size_t i = 0; i < amount; i++) {
    res[i].size = matrix_size;

    for (size_t j = 0; j < matrix_size; j++)
      for (size_t k = 0; k < matrix_size; k++) {
        res[i].values[j][k].real      = 1 + rand() % 10;
        res[i].values[j][k].imaginary = 1 + rand() % 10;
      }
  }

  return res;
}

Complex_Matrix* Matrix_get_empty_matrixes(const int amount, const int matrix_size) {
  Complex_Matrix* res = Matrix_alloc(amount, matrix_size);
  for (size_t i = 0; i < amount; i++) res[i].size = matrix_size;
  return res;
}

void Matrix_copy(Complex_Matrix* to, Complex_Matrix* from) {
  for (size_t i = 0; i < to->size; i++)
    for (size_t j = 0; j < to->size; j++) {
      to->values[i][j].real      = from->values[i][j].real;
      to->values[i][j].imaginary = from->values[i][j].imaginary;
    }
}

// z1⋅z2 = (x1 + y1i)⋅(x2 + y2i) = (x1⋅x2 − y1⋅y2) + (x1⋅y2 + x2⋅y1)i
Complex_Number complex_multiply(const Complex_Number l, const Complex_Number r) {
  Complex_Number res;
  // (x1⋅x2 − y1⋅y2)
  res.real = l.real * r.real - l.imaginary * r.imaginary;
  // (x1⋅y2 + x2⋅y1)i
  res.imaginary = l.real * r.imaginary + r.real * l.imaginary;

  return res;
}

// z1 + z2 = (x1 + y1i) + (x2 + y2i) = (x1 + x2) + (y1 + y2)i
Complex_Number complex_sum(const Complex_Number l, const Complex_Number r) {
  Complex_Number res;
  // (x1 + x2)
  res.real = l.real + r.real;
  // (y1 + y2)i
  res.imaginary = l.imaginary + r.imaginary;

  return res;
}

void Matrix_multiply(const Complex_Matrix* l_mtx, const Complex_Matrix* r_mtx, Complex_Matrix* res_mtx) {
  int size = l_mtx->size;

  for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
          res_mtx->values[i][j].real = 0;
          res_mtx->values[i][j].imaginary = 0;
          for (int k = 0; k < size; ++k)
              res_mtx->values[i][j] = complex_sum(
                                          res_mtx->values[i][j],
                                          complex_multiply(l_mtx->values[i][k], r_mtx->values[k][j])
                                        );
      }
  }
}

void Matrix_rec_mult(Complex_Matrix* mtx_arr,
                      int index,
                      const int amount,
                      Complex_Matrix* res_mtx,
                      Complex_Matrix* temp_mtx) {

  if (amount == index) return;

  Matrix_multiply(res_mtx, &(mtx_arr[index]), temp_mtx);
  Matrix_copy(res_mtx, temp_mtx);

  Matrix_rec_mult(mtx_arr, ++index, amount, res_mtx, temp_mtx);
}

void Matrix_acc_multiply(Complex_Matrix* mtx_arr, const int amount, Complex_Matrix* res_mtx) {
  Complex_Matrix* temp_mtx = Matrix_alloc(1, res_mtx->size);

  Matrix_multiply(&(mtx_arr[0]), &(mtx_arr[1]), res_mtx);
  Matrix_rec_mult(mtx_arr, 2, amount, res_mtx, temp_mtx);

  Matrix_free(temp_mtx, 1);
}

void Matrix_print(const Complex_Matrix* mtx) {
  printf("Size: %d\n", mtx->size);

  for (size_t i = 0; i < mtx->size; i++) {
    for (size_t j = 0; j < mtx->size; j++)
      printf("{r:%d|i:%d} ",
        mtx->values[i][j].real,
        mtx->values[i][j].imaginary
      );
    printf("\n");
  }

}
void Matrix_mult_print(const Complex_Matrix* mtx_arr, const int amount) {
  for (size_t i = 0; i < amount; i++) {
    printf("---- Matrix %zu ----\n", i);
    Matrix_print(&(mtx_arr[i]));
  }
}
