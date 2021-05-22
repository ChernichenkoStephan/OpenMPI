#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "Long_Word/long_word.h"

/**
* Starting script for manual call in unix systems
* ./start.sh
*
* Starting commend for manual debug without MPI
* clear && gcc -o test main.c ./Long_Word/long_word.c && ./test
*/

/**
* Задание. Реализуйте на основе технологии MPI многопоточную программу с использованием
* групп процессов и коммуникаторов в соответствии с вариантом задания.
* Проверьте корректность работы программы. Результаты занесите в отчет.
*
* Реализуйте тип «длинное целое».
* Напишите программу, которая осуществляет умножение A целых чисел заданной длины
* методом Карацубы.
* (123)^8 = 52 389 094 428 262 881
*/

MPI_Datatype make_dto_type() {
  MPI_Datatype res;
  int fields_amount         = 2;
  MPI_Datatype old_types[2] = {MPI_INT, MPI_LONG};
  int          blocklens[2] = {MAX_WORD_LENGTH, 1};
  MPI_Aint displacements[2] = {
    offsetof(Long_Word_DTO, numbers),
    offsetof(Long_Word_DTO, word_length)
  };

  MPI_Type_create_struct(fields_amount, blocklens, displacements, old_types, &res);
  MPI_Type_commit(&res);
  return res;
}

// Long_Word* get_words_array(int amount) {
//   Long_Word* res = (Long_Word*) malloc(amount * sizeof(Long_Word));
//
//   for (size_t i = 0; i < amount; i++) {
//     res[i].word_length = 3;
//     for (size_t j = 0; j < 3; j++)
//       res[i].numbers[j] = j;
//   }
//   return res;
// }

void fill_words_array_test(Long_Word_DTO* array, int amount) {
  for (size_t i = 0; i < amount; i++) {
    array[i].word_length = 3;
    for (size_t j = 0; j < 3; j++)
      array[i].numbers[j] = 3 - j;
  }
}

void print_array(Long_Word* array, int length) {
  for (size_t i = 0; i < length; i++) Long_Word_print(array + i);
}


int main(int argc, char *argv[]) {
  /* Init phase */
  MPI_Status status;
  MPI_Datatype long_word_type;
  int proc_rank = 0, proc_num = 1, op_status = 0;

  int elements_per_proc = 2;
  int amount = 8;
  Long_Word_DTO source_array[amount];

  fill_words_array_test(source_array, amount);

  /* ------------ Begin of communism area ------------ */
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

  int root = 0;
  int is_root = proc_rank == root;
  long_word_type = make_dto_type();

  if (is_root) {

    printf("Root sends\n");
    for (size_t i = 0; i < amount; i++) {
      Long_Word_DTO_print(source_array + i);
      printf("\n");
    }

    MPI_Send(source_array, amount, long_word_type, 1, 0, MPI_COMM_WORLD);

  }
  else if (proc_rank == 1) {

    printf("1 gets\n");
    MPI_Recv(source_array, amount, long_word_type, root, 0, MPI_COMM_WORLD, &status);
    for (size_t i = 0; i < amount; i++) {
      Long_Word_DTO_print(source_array + i);
      printf("\n");
    }

  }

  MPI_Type_free(&long_word_type);
  /* End of communism area */
  MPI_Finalize();
  return 0;
}
