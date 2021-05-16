#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "Matrix/matrix.h"


/**
* Starting script for manual call in unix systems
* ./start.sh
*
* Starting commend for manual debug without MPI
* clear && gcc -o test main.c ./Matrix/matrix.c && ./test
*/

/**
* Задание. Реализуйте на основе технологии MPI многопоточную программу в
* соответствии с вариантом задания. Найдите возможные способы проверки
* корректности работы программы. Оцените их возможное достоинства и недостатки.
* Проверьте корректность работы программы при помощи одного из них.
*
* Вариант 0
*   Задание
*     Реализуйте тип «комплексная матрица».
*     Напишите программу, которая осуществляет умножение A матриц размером NxN.
*/

int main(int argc, char** argv) {
  // Init phase
  int proc_rank = 0, proc_num = 1, recvRank = 0, op_status = 0;
  MPI_Status status;

  // ---- Amount calculations ----
  int matrix_size        = 4;
  int matrix_amount      = 8;
  int mtx_elements_amount = matrix_size * matrix_size;
  int elements_amount     = matrix_size * matrix_size * matrix_amount;

  // ---- Calculation vars ----
  Complex_Matrix* mtx_arr, *mtx_sub_arr;
  Complex_Matrix* res_mtx, *temp_res_mtx, *proc_res_mtx;

  // ---- Communication buffs ----
  Complex_Number* snd_array;  // Send buff
  Complex_Number* rcv_array;  // Receive buff

  // ---- MPI typedef preparation ----
  MPI_Datatype complex_number_type;   // datatype for complex num
  MPI_Datatype complex_matrix_type;   // datatype for complex matrices


  // ------------ Begin of communism area ------------
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

  int root = 0;
  int is_root = proc_rank == root;

  int mtxs_per_proc       = matrix_amount / proc_num;
  int elements_per_proc   = elements_amount / proc_num;

  // ------------ MPI typedef ------------
  // ---- Complex_Number DTO type creation ----
  MPI_Aint array_of_displacements[] = { offsetof( Complex_Number, real ),
                                        offsetof( Complex_Number, imaginary ) };
  int      block_lengths[2] = {1, 1};             // number of element in block
  MPI_Datatype old_types[2] = {MPI_INT, MPI_INT}; // source datatypes in block

  op_status = MPI_Type_create_struct(2, block_lengths, array_of_displacements, old_types, &complex_number_type);
  op_status = MPI_Type_commit (&complex_number_type);


  // ---- Complex_Matrix DTO type creation ----
  op_status = MPI_Type_vector(1, mtx_elements_amount, mtx_elements_amount, complex_number_type, &complex_matrix_type);
  op_status = MPI_Type_commit (&complex_matrix_type);


  // ------------ Objects preparations & root calc ------------
  proc_res_mtx = Matrix_get_empty_matrixes(1, matrix_size);

  if (is_root) {
    mtx_arr = Matrix_get_rand_matrixes(matrix_amount, matrix_size);
    temp_res_mtx = Matrix_get_empty_matrixes(1, matrix_size);
    res_mtx = Matrix_get_empty_matrixes(1, matrix_size);

    Matrix_acc_multiply(mtx_arr, mtxs_per_proc, proc_res_mtx);
  } else
    mtx_sub_arr = Matrix_get_empty_matrixes(mtxs_per_proc, matrix_size);


  MPI_Barrier(MPI_COMM_WORLD);
  // ------------ First part ------------
  for (int reciver = 1; reciver < proc_num; reciver++) {
    if (is_root) {

      int send_index = reciver * mtxs_per_proc;
      snd_array = Matrix_to_array(&(mtx_arr[send_index]), mtxs_per_proc);

      int snd_status = MPI_Send(&(snd_array[0]), mtxs_per_proc, complex_matrix_type,
                                    reciver, 0, MPI_COMM_WORLD);
      free(snd_array);
    } else if(reciver == proc_rank) {

      rcv_array = (Complex_Number*) malloc(elements_per_proc * sizeof(Complex_Number));
      int rcv_status = MPI_Recv(&(rcv_array[0]), elements_per_proc, complex_number_type,
                                    root, 0, MPI_COMM_WORLD, &status);

      Matrix_from_array(rcv_array, mtx_sub_arr, mtxs_per_proc);
      Matrix_acc_multiply(mtx_sub_arr, mtxs_per_proc, proc_res_mtx);

      free(rcv_array);
    }
  }

  // ------------ Fin part ------------
  for (int sender = 1; sender < proc_num; sender++) {
    if (!is_root) {
      snd_array = Matrix_to_array(proc_res_mtx, 1);
      int snd_status = MPI_Send(&(snd_array[0]), 1, complex_matrix_type,
                                    root, 0, MPI_COMM_WORLD);
      free(snd_array);
    } else {

      rcv_array = (Complex_Number*) malloc(mtx_elements_amount * sizeof(Complex_Number));
      int rcv_status = MPI_Recv(&(rcv_array[0]), mtx_elements_amount, complex_number_type,
                                    sender, 0, MPI_COMM_WORLD, &status);

      Matrix_from_array(rcv_array, temp_res_mtx, 1);
      Matrix_multiply(proc_res_mtx, temp_res_mtx, res_mtx);
      Matrix_copy(proc_res_mtx, res_mtx);

      free(rcv_array);
    }
  }


  if (is_root) {
    Matrix_copy(res_mtx, proc_res_mtx);
    printf("\nRes: \n");
    Matrix_print(res_mtx);
  }

  // ---- Memory clean work ----
  MPI_Type_free(&complex_number_type);
  MPI_Type_free(&complex_matrix_type);

  if (is_root) {
    Matrix_free(mtx_arr, matrix_amount);
    Matrix_free(temp_res_mtx, 1);
    Matrix_free(res_mtx, 1);
  }
  else Matrix_free(mtx_sub_arr, mtxs_per_proc);
  Matrix_free(proc_res_mtx, 1);

  // End of communism area
  MPI_Finalize();

  return 0;
}
