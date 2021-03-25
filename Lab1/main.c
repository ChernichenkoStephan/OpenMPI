#include <mpi.h>
#include <stdio.h>

// Starting script for manual call in unix systems
// ./start.sh

/*
Реализуйте функцию ring, которая создаёт N процессов и посылает сообщение
первому процессу, который посылает сообщение второму, второй - третьему,
и так далее. Наконец, процесс N посылает сообщение обратно процессу 1.
После того, как сообщение обежало вокруг кольца M раз, все процессы заканчивают работу.
*/

int main(int argc, char** argv) {
  // Init phase

  int proc_rank, proc_num, recvRank;
  MPI_Status status;

  // Start of MPI block
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

  const int PREW_PROC = proc_rank - 1;
  const int NEXT_PROC = proc_rank + 1;
  const int LAST_PROC = proc_num  - 1;

  if (proc_rank == 0) {
    // Start of proc with 0 rank block
    printf("Processes created: %d\n", proc_num);
    int message = 777 - proc_num + 1;
    int rsv_message;

    printf("Rank 0 process sends message to %d...\n", 1);
    int snd_status = MPI_Send(&message, 1, MPI_INT, NEXT_PROC, 0, MPI_COMM_WORLD);

    printf("Rank 0 process w8s message from %d...\n", LAST_PROC);
    int rcv_status = MPI_Recv(&rsv_message, 1, MPI_INT, LAST_PROC,
              MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    printf("Master process got the massage: %d\n", rsv_message);
  } else {
    // Start of other procs ranks blocks
    int message;

    printf("Rank %d process w8...\n", proc_rank);
    int rcv_status = MPI_Recv(&message, 1, MPI_INT, PREW_PROC,
              MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    // To check of correctness
    int msg_to_sent = ++message;

    printf("Rank %d process passes %d to %d...\n", proc_rank, msg_to_sent, NEXT_PROC);
    int snd_status;
    if (proc_rank != proc_num - 1) {
        snd_status = MPI_Send(&msg_to_sent, 1, MPI_INT,
                    NEXT_PROC, 0, MPI_COMM_WORLD);
    }
    else {
      snd_status = MPI_Send(&msg_to_sent, 1, MPI_INT,
                 0, 0, MPI_COMM_WORLD);
    }
  }

  // End of MPI block
  MPI_Finalize();

  return 0;
}
