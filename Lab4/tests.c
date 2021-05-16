#include <stdio.h>
#include <stdlib.h>

#include "Matrix/matrix.c"

/**
* clear && gcc -o test tests.c && ./test
*/
void complex_num_test();
void complex_mtx_test();
void complex_mult_mtx_test();
void step_by_step_mult();


int main(int argc, char** argv) {
  /*
  1) Res by hand: z1+z2=2−4i
  2) Res by hand: z1*z2=9−7i
  */
  printf("--------/complex_num_test/--------\n");
  complex_num_test();

  /*
  c11 = a11 · b11 + a12 · b21
  c11 = {r:8|i:10}*{r:4|i:10} + {r:4|i:9}*{r:3|i:3}
  {r:8|i:10}*{r:4|i:10} = {r:-68|i:120}
  {r:4|i:9}*{r:3|i:3}   = {r:-15|i:39}
  c11 = {r:-83|i:159}

  c12 = a11 · b12 + a12 · b22
  c12 = {r:8|i:10}*{r:1|i:6} + {r:4|i:9}*{r:8|i:4}
  {r:8|i:10}*{r:1|i:6} = {r:-52|i:58}
  {r:4|i:9}*{r:8|i:4}  = {r:-4|i:88}
  c12 = {r:-56|i:146}

  c21 = a21 · b11 + a22 · b21
  c21 = {r:1|i:3}*{r:4|i:10} + {r:5|i:9}*{r:3|i:3}
  {r:1|i:3}*{r:4|i:10} = {r:-26|i:22}
  {r:5|i:9}*{r:3|i:3}  = {r:-12|i:42}
  c21 = {r:-38|i:64}

  c22 = a21 · b12 + a22 · b22
  c22 = {r:1|i:3}*{r:1|i:6} + {r:5|i:9}*{r:8|i:4}
  {r:1|i:3}*{r:1|i:6} = {r:-17|i:9}
  {r:5|i:9}*{r:8|i:4} = {r:4|i:92}
  c22 = {r:-13|i:101}

  Res by hand:
  {r:-83|i:159} {r:-56|i:146}
  {r:-38|i:64} {r:-13|i:101}
  */
  printf("\n--------/complex_mtx_test/--------\n");
  complex_mtx_test();

  /*
  Checked result:
  {r:-27350|i:-22560} {r:-68332|i:-39422}
  {r:-16942|i:-8634} {r:-39050|i:-15322}
  */
  printf("\n--------/complex_mult_mtx_test/--------\n");
  complex_mult_mtx_test();
  step_by_step_mult();

  return 0;
}

Complex_Matrix* get_test_matrixes(const int matrix_size) {

  Complex_Matrix* mtx_arr = Matrix_get_empty_matrixes(2, matrix_size);
  /*
  ---- Matrix 0 ----
  Size: 2
  {r:8|i:10} {r:4|i:9}
  {r:1|i:3} {r:5|i:9}
  */
  mtx_arr[0].values[0][0].real = 8;
  mtx_arr[0].values[0][0].imaginary = 10;
  mtx_arr[0].values[0][1].real = 4;
  mtx_arr[0].values[0][1].imaginary = 9;
  mtx_arr[0].values[1][0].real = 1;
  mtx_arr[0].values[1][0].imaginary = 3;
  mtx_arr[0].values[1][1].real = 5;
  mtx_arr[0].values[1][1].imaginary = 9;

  /*
  ---- Matrix 1 ----
  Size: 2
  {r:4|i:10} {r:1|i:6}
  {r:3|i:3} {r:8|i:4}
  */
  mtx_arr[1].values[0][0].real = 4;
  mtx_arr[1].values[0][0].imaginary = 10;
  mtx_arr[1].values[0][1].real = 1;
  mtx_arr[1].values[0][1].imaginary = 6;
  mtx_arr[1].values[1][0].real = 3;
  mtx_arr[1].values[1][0].imaginary = 3;
  mtx_arr[1].values[1][1].real = 8;
  mtx_arr[1].values[1][1].imaginary = 4;

  return mtx_arr;
}


Complex_Matrix* get_mult_test_matrixes(const int matrix_amount, const int matrix_size) {

  Complex_Matrix* mtx_arr = Matrix_get_empty_matrixes(matrix_amount, matrix_size);

  /*
  ---- Matrix 0 ----
  {r:8|i:10} {r:4|i:9}
  {r:1|i:3} {r:5|i:9}
  */
  mtx_arr[0].values[0][0].real = 8;
  mtx_arr[0].values[0][0].imaginary = 10;
  mtx_arr[0].values[0][1].real = 4;
  mtx_arr[0].values[0][1].imaginary = 9;
  mtx_arr[0].values[1][0].real = 1;
  mtx_arr[0].values[1][0].imaginary = 3;
  mtx_arr[0].values[1][1].real = 5;
  mtx_arr[0].values[1][1].imaginary = 9;

  /*
  ---- Matrix 1 ----
  {r:4|i:10} {r:1|i:6}
  {r:3|i:3} {r:8|i:4}
  */
  mtx_arr[1].values[0][0].real = 4;
  mtx_arr[1].values[0][0].imaginary = 10;
  mtx_arr[1].values[0][1].real = 1;
  mtx_arr[1].values[0][1].imaginary = 6;
  mtx_arr[1].values[1][0].real = 3;
  mtx_arr[1].values[1][0].imaginary = 3;
  mtx_arr[1].values[1][1].real = 8;
  mtx_arr[1].values[1][1].imaginary = 4;

  /*
  ---- Matrix 2 ----
  {r:8|i:10} {r:1|i:3}
  {r:4|i:10} {r:10|i:8}
  */
  mtx_arr[2].values[0][0].real = 8;
  mtx_arr[2].values[0][0].imaginary = 10;
  mtx_arr[2].values[0][1].real = 1;
  mtx_arr[2].values[0][1].imaginary = 3;
  mtx_arr[2].values[1][0].real = 4;
  mtx_arr[2].values[1][0].imaginary = 10;
  mtx_arr[2].values[1][1].real = 10;
  mtx_arr[2].values[1][1].imaginary = 8;

  /*
  ---- Matrix 3 ----
  {r:1|i:4} {r:10|i:9}
  {r:7|i:6} {r:8|i:7}
  */
  mtx_arr[3].values[0][0].real = 1;
  mtx_arr[3].values[0][0].imaginary = 4;
  mtx_arr[3].values[0][1].real = 10;
  mtx_arr[3].values[0][1].imaginary = 9;
  mtx_arr[3].values[1][0].real = 7;
  mtx_arr[3].values[1][0].imaginary = 6;
  mtx_arr[3].values[1][1].real = 8;
  mtx_arr[3].values[1][1].imaginary = 7;

  return mtx_arr;
}

void fulfill(Complex_Matrix* mtx, const int size, const int amount) {
  for (size_t i = 0; i < amount; i++)
    for (size_t j = 0; j < size; j++)
      for (size_t k = 0; k < size; k++) {
        mtx[i].values[j][k].real = 1;
        mtx[i].values[j][k].imaginary = 1;
      }
}


void complex_num_test() {
  Complex_Number z1, z2;

  /*
  Найти сумму z1+z2, если z1=5−6i, z2= −3+2i .
  z1+z2 = 5−6i+(−3+2i)=(5+(−3))+(−6+2)i=2−4i
  Ответ. z1+z2=2−4i
  */
  z1.real = 5;
  z1.imaginary = -6;

  z2.real = -3;
  z2.imaginary = 2;

  Complex_Number z_sum = complex_sum(z1, z2);
  printf("Res: %d+(%d)i\n", z_sum.real , z_sum.imaginary);

  /*
  Выполнить умножение комплексных чисел: z1=3+i и z2=2−3i
  Ответ. z1*z2=9−7i
  */

  z1.real = 3;
  z1.imaginary = 1;

  z2.real = 2;
  z2.imaginary = -3;

  Complex_Number z_mult = complex_multiply(z1, z2);
  printf("Res: %d+(%d)i\n", z_mult.real , z_mult.imaginary);
}

void complex_mtx_test() {
  int matrix_size        = 2;
  int matrix_amount      = 2;

  Complex_Matrix* mtx_arr, *res_mtx;

  mtx_arr = get_test_matrixes(matrix_size);
  res_mtx = Matrix_get_empty_matrixes(1, matrix_size);

  printf("Main inits: \n");
  Matrix_mult_print(mtx_arr, matrix_amount);

  fulfill(res_mtx, matrix_size, 1);
  Matrix_multiply(&(mtx_arr[0]), &(mtx_arr[1]), res_mtx);

  printf("\nRes: \n");
  Matrix_print(res_mtx);

  Matrix_free(mtx_arr, matrix_amount);
  Matrix_free(res_mtx, 1);
}

void complex_mult_mtx_test() {
  int matrix_size        = 2;
  int matrix_amount      = 4;

  Complex_Matrix* mtx_arr, *res_mtx;

  mtx_arr = get_mult_test_matrixes(matrix_amount, matrix_size);
  res_mtx = Matrix_get_empty_matrixes(1, matrix_size);

  printf("Main inits: \n");
  Matrix_mult_print(mtx_arr, matrix_amount);

  fulfill(res_mtx, matrix_size, 1);
  Matrix_acc_multiply(mtx_arr, matrix_amount, res_mtx);

  printf("Res: \n");
  Matrix_print(res_mtx);

  Matrix_free(mtx_arr, matrix_amount);
  Matrix_free(res_mtx, 1);
}


void step_by_step_mult() {

  int matrix_size        = 2;
  int matrix_amount      = 4;

  Complex_Matrix* mtx_arr, *res_mtx, *temp_res_mtx;
  mtx_arr = get_mult_test_matrixes(matrix_amount, matrix_size);
  temp_res_mtx = Matrix_get_empty_matrixes(1, matrix_size);
  res_mtx = Matrix_get_empty_matrixes(1, matrix_size);
  fulfill(temp_res_mtx, matrix_size, 1);
  fulfill(res_mtx, matrix_size, 1);

  printf("\n\n----\"Step by step multiply\"----\n");
  Matrix_multiply(&(mtx_arr[0]), &(mtx_arr[1]), temp_res_mtx);
  Matrix_copy(res_mtx, temp_res_mtx);
  printf("1 Temp Res: \n");
  Matrix_print(temp_res_mtx);

  printf("\n\n");
  Matrix_multiply(res_mtx, &(mtx_arr[2]), temp_res_mtx);
  Matrix_copy(res_mtx, temp_res_mtx);
  printf("2 Temp Res: \n");
  Matrix_print(temp_res_mtx);

  printf("\n\n");
  Matrix_multiply(res_mtx, &(mtx_arr[3]), temp_res_mtx);
  Matrix_copy(res_mtx, temp_res_mtx);
  printf("3 Temp Res: \n");
  Matrix_print(temp_res_mtx);

  printf("\n\n");
  printf("Final Res: \n");
  Matrix_print(res_mtx);

  Matrix_free(mtx_arr, matrix_amount);
  Matrix_free(res_mtx, 1);

}
