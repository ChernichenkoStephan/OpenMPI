#include <mpi.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

// Starting script for manual call in unix systems
// ./start.sh

/*
Задание. Модифицировать программу, написанную на Л.Р. No1, так чтобы она работала на
основе коллективной передачи сообщений. Результаты работы сравнить и занести в отчет.
*/

int main(int argc, char** argv) {
  // Init phase
  int proc_rank, proc_num, recvRank;
  MPI_Status status;

  // Begin of communism area
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

  int message_to_send = 0;    // Send buff
  int message_to_receive = 0; // Receive buff
  int brr_status;             // For system use (to check MPI_Barrier correctness)

  for (int current_proc = 0; current_proc < proc_num; current_proc++) {

    brr_status = MPI_Barrier(MPI_COMM_WORLD);

    int bct_status; // For system use (to check MPI_Bcast correctness)
    if (current_proc == proc_rank){
      // Residence of individualism
      // Sender MPI_Bcast call
      printf("\nComrade %d process sends message %d to all...\n\n", proc_rank, message_to_send);
      bct_status = MPI_Bcast(&message_to_send, 1, MPI_INT, current_proc, MPI_COMM_WORLD);

    } else {

      // This sleep is for cool print
      // As i understood, send is harder operation,
      // therefore receiver come to this block earlier and
      // "w8s" message prints before "send" one.
      // For performance just comment out sleep function
      // !!!!!!!!!!!! AND NEXT ONE ON LINE 69 !!!!!!!!!!!!
      sleep(1);

      // Receiver MPI_Bcast call
      bct_status = MPI_Bcast(&message_to_receive, 1, MPI_INT, current_proc, MPI_COMM_WORLD);
      printf("Comrade %d process got the massage: %d\n", proc_rank, message_to_receive);
    }

    if (proc_rank == current_proc + 1) {
      // Residence of individualism
      // Increasing received message to send it in next loop
      message_to_send = message_to_receive + 1;
      printf("Comrade %d process, I'm next. New message: %d\n", proc_rank, message_to_send);
    }

    brr_status = MPI_Barrier(MPI_COMM_WORLD);

    // This sleep is for cool print, comment it for performance
    sleep(1);

  }

  if (proc_rank == 0)
    printf("\nResult messsage: %d\n", message_to_receive);

  // End of communism area
  MPI_Finalize();

  return 0;
}
