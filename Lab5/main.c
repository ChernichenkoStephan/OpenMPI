#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "Long_Word/long_word.h"
#include "Utils/utils.h"

/**
* Starting script for manual call in unix systems
* ./start.sh
*
* Starting commend for manual debug without MPI
* clear && gcc -o test main.c ./Long_Word/long_word.c && ./test
*/

/**
* Задание. Реализуйте на основе технологии MPI многопоточную программу
* с использованием групп процессов и коммуникаторов в соответствии с
* вариантом задания. Проверьте корректность работы программы.
* Результаты занесите в отчет.
*
* Реализуйте тип «длинное целое».
* Напишите программу, которая осуществляет умножение A целых чисел заданной
* длины методом Карацубы.
* (123)^8 = 52 389 094 428 262 881
*/

/*
https://docs.microsoft.com/en-us/message-passing-interface/mpi-datatype-enumeration

int MPI_Group_incl(MPI_Group oldgroup,int n, int *ranks,MPI_Group *newgroup);

int MPI_Group_free ( MPI_Group *group );
int MPI_Comm_free (MPI_Comm *comm);

int MPI_Group_size ( MPI_Group group, int *size );
int MPI_Group_rank ( MPI_Group group, int *rank );
*/


int main(int argc, char *argv[]) {
  MPI_Status status;
  /* Init phase */
  MPI_Datatype long_word_type;
  MPI_Comm comm0 = MPI_COMM_WORLD;
  int proc_rank = 0, proc_num = 1, op_status = 0;

  int elements_per_proc = 2;
  int amount = 8;
  Long_Word_DTO source_array[amount];
  Long_Word_DTO receive_array[elements_per_proc];

  /* ------------ Begin of communism area ------------ */
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(comm0, &proc_rank);
  MPI_Comm_size(comm0, &proc_num);

  int root = 0;
  char is_root = proc_rank == root;

  /* ------- Data preparation & send part ------- */
  if (is_root) {
    fill_words_array_test(source_array, amount);
    printf("Root sends:\n");
    for (size_t i = 0; i < amount; i++) {
      Long_Word_DTO_print(source_array + i);
    }
  }

  /* Make type & scatter the numbers to all processes */
  long_word_type = make_dto_type();
  int sctr_status = MPI_Scatter(source_array, elements_per_proc, long_word_type,
                                receive_array, elements_per_proc, long_word_type,
                                root, comm0);


  if (!is_root) {
    printf("Proc-%d receives:\n", proc_rank);
    for (size_t i = 0; i < elements_per_proc; i++) {
      Long_Word_DTO_print(receive_array + i);
    }
  }

  MPI_Group MPI_GROUP_WORLD;
  MPI_Group group1, group2;
  MPI_Comm comm1, comm2;
  char in_first  = proc_rank < 2;
  char in_second = proc_rank >= 2;

  int group1_process_ranks[2] = {0, 1};
  int group2_process_ranks[2] = {2, 3};

  MPI_Comm_group(comm0, &MPI_GROUP_WORLD);

  MPI_Group_incl(MPI_GROUP_WORLD, 2, group1_process_ranks, &group1);
  MPI_Group_incl(MPI_GROUP_WORLD, 2, group2_process_ranks, &group2);
  MPI_Comm_create(comm0, group1, &comm1);
  MPI_Comm_create(comm0, group2, &comm2);

  /* ------- Calculation part ------- */
  Long_Word* num_array = (Long_Word*) malloc(elements_per_proc * sizeof(Long_Word));
  for (size_t i = 0; i < elements_per_proc; i++)
    num_array[i] = Long_Word_from_DTO(receive_array);

  Long_Word res = num_array[0];

  for (size_t i = 1; i < elements_per_proc; i++) {
    Long_Word temp = Long_Word_mult_karatsuba(res, num_array[i]);
    res = temp;
  }

  /* ------- Group send part ------- */
  int size, rank, snd_status, rcv_status, is_group_root;
  Long_Word_DTO message;
  if (in_first) {
    MPI_Group_size(group1, &size);
    MPI_Group_rank(group1, &rank);
    is_group_root = rank == root;

    if (is_root) receive(&message, 1, &long_word_type, &comm1);
    else send(&res, 0, &long_word_type, &comm1);

    MPI_Group_free(&group1);
    MPI_Comm_free(&comm1);
  } else if (in_second) {
    MPI_Group_size(group2, &size);
    MPI_Group_rank(group2, &rank);
    is_group_root = rank == root;

    if (is_group_root) receive(&message, 1, &long_word_type, &comm2);
    else send(&res, 0, &long_word_type, &comm2);

    MPI_Group_free(&group2);
    MPI_Comm_free(&comm2);
  }

  /* ------- Final calculations & send part ------- */
  if (is_root) {
    Long_Word temp = Long_Word_mult_karatsuba(res, Long_Word_from_DTO(&message));
    receive(&message, 2, &long_word_type, &comm0);
    Long_Word final = Long_Word_mult_karatsuba(temp, Long_Word_from_DTO(&message));

    printf("Final res ((123)^8 = 52 389 094 428 262 881) :\n");
    Long_Word_print(&final);
  } else if (is_group_root) {
    Long_Word temp = Long_Word_mult_karatsuba(res, Long_Word_from_DTO(&message));
    send(&temp, root, &long_word_type, &comm0);
  }

  free(num_array);
  MPI_Type_free(&long_word_type);
  /* End of communism area */
  MPI_Finalize();
  return 0;
}
