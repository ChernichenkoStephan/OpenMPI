#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "Utils/utils.h"

/**
* Starting script for manual call in unix systems
* ./start.sh
*
* Starting commend for manual debug without MPI
* gcc -o test main.c ./Utils/utils.c && ./test
*/

/**
* Задание. Модифицировать программу, написанную на Л.Р. No1 так чтобы она работала на
* основе обобщенной передачи сообщений.
*/

int main(int argc, char** argv) {
  // Init phase
  int proc_rank, proc_num, recvRank;
  MPI_Status status;

  // Begin of communism area
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

  int elements_per_proc = proc_num;

  float* source_array;  // Send buff
  float* sub_sums;      // Temp buff
  float* sub_array;     // Receive buff

  float send_sum_message = 0;
  float rcv_sum_message = 0;

  int last_in_chain = 7;

  for (int i = 0; i < last_in_chain; i++) {

    int root = i % proc_num;
    int is_root = proc_rank == root;
    int is_next = proc_rank == (root + 1) % proc_num;

    if (is_root) {
      printf("%d processes turn in loop %d\n", root, i);
      source_array = get_source_array(proc_num);
      sub_sums = malloc(sizeof(float) * proc_num);
    }

    sub_array = malloc(sizeof(float) * elements_per_proc);

    // Scatter the numbers to all processes
    int sctr_status = MPI_Scatter(source_array, elements_per_proc, MPI_FLOAT, sub_array,
                elements_per_proc, MPI_FLOAT, root, MPI_COMM_WORLD);

    // Compute the sum of own subset all processes
    float sub_sum = sum(sub_array, proc_num);

    // Gather all partial sums down to the root process
    int gthr_status = MPI_Gather(&sub_sum, 1, MPI_FLOAT, sub_sums, 1, MPI_FLOAT, root,
               MPI_COMM_WORLD);

    // Accumulating loop sum
    MPI_Barrier(MPI_COMM_WORLD);
    if (is_root) {
      send_sum_message = sum(sub_sums, proc_num) + rcv_sum_message;

      int reciver_rank = (root + 1) % proc_num;
      int snd_status = MPI_Send(&send_sum_message, 1, MPI_FLOAT, reciver_rank, 0, MPI_COMM_WORLD);

      printf("%d->%d [%4.1f] \n", proc_rank, reciver_rank, send_sum_message);

      free(source_array);
      free(sub_sums);
    }
    if (is_next) {

      int sender_rank = (proc_rank + proc_num - 1) % proc_num;
      int rcv_status = MPI_Recv(&rcv_sum_message, 1, MPI_FLOAT, sender_rank,
        MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      printf("%d<-%d [%4.1f] \n", proc_rank, sender_rank, rcv_sum_message);
    }
    free(sub_array);
  }

  if (proc_rank == last_in_chain % proc_num) {
    sleep(1);
    printf("Result in last proc (proc #%d) = %f\n", proc_rank, rcv_sum_message);
  }

  // End of communism area
  MPI_Finalize();

  return 0;
}
