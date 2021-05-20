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
*/

MPI_Datatype make_long_word_type(int word_length) {
  MPI_Datatype res;
  int fields_amount         = 2;
  MPI_Datatype old_types[2] = {MPI_INT, MPI_INT};
  int          blocklens[2] = {word_length, 1};
  MPI_Aint displacements[2] = {
    offsetof(Long_Word, nums),
    offsetof(Long_Word, length)
  };

  MPI_Type_create_struct(fields_amount, blocklens, displacements, old_types, &res);
  MPI_Type_commit(&res);
  return res;
}


int main(int argc, char *argv[]) {
  /* Init phase */
  MPI_Status status;
  MPI_Datatype long_word_type;
  int proc_rank = 0, proc_num = 1, op_status = 0;
  int amount = 4;
  int word_length = 5;

  Long_Word* source_array;
  int elements_per_proc = 1;

  /* ------------ Begin of communism area ------------ */
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

  int root = 0;
  int is_root = proc_rank == root;

  long_word_type = make_long_word_type(word_length);

  if (is_root) {
    printf("nums %lu | %lu\n", sizeof(Long_Word), offsetof(Long_Word, nums));
    printf("length %lu | %lu\n", sizeof(int), offsetof(Long_Word, length));
  }

  if (is_root) {
      source_array = (Long_Word*) malloc(amount * sizeof(Long_Word));
      for (size_t i = 0; i < amount; i++) {
        source_array[i].nums = (int*) malloc(word_length * sizeof(int));
        for (size_t j = 0; j < word_length; j++)
          source_array[i].nums[j] = 7;
      }

      MPI_Send(source_array, amount, long_word_type, 1, 0, MPI_COMM_WORLD);
  }
  else if (proc_rank == 1) {
    source_array = (Long_Word*) malloc(amount * sizeof(Long_Word));
    for (size_t i = 0; i < amount; i++) source_array[i].nums = (int*) malloc(word_length * sizeof(int));
    MPI_Recv(source_array, amount, long_word_type, root, 0, MPI_COMM_WORLD, &status);
    
    for (size_t i = 0; i < amount; i++)
      printf("%d\n", source_array[i].nums[0]);

  }

  /* Clear part */
  MPI_Type_free(&long_word_type);

  /* End of communism area */
  MPI_Finalize();
  return 0;
}
