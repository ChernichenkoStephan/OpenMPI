#include <stdio.h>
#include "mpi.h"
int main(int argc, char* argv[])
{
  int ProcNum, ProcRank, RecvRank;
  MPI_Status Status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0)
  {
   // Действия, выполняемые только процессом с рангом 0
   printf ("\n Hello from process %3d", ProcRank);
   for ( int i=1; i < ProcNum; i++ )
   {
    MPI_Recv(&RecvRank, 1, MPI_INT, MPI_ANY_SOURCE,
    MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
    printf("\n Hello from process %3d", RecvRank);
   }
  } else // Сообщение, отправляемое всеми процессами,
         // кроме процесса с рангом 0
  MPI_Send(&ProcRank,1,MPI_INT,0,0,MPI_COMM_WORLD);
  MPI_Finalize();
  return 0;
}


int MPI_Send(void *buf, int count, MPI_Datatype type, int dest, int tag, MPI_Comm comm);
int MPI_Recv(void *buf, int count, MPI_Datatype type, int source, int tag, MPI_Comm comm, MPI_Status *status);


double startTime = 0, finishTime = 0, time = 0;
startTime = MPI_Wtime();
finishTime = MPI_Wtime();
time = startTime - finishTime;
printf("Time elapsed: %f\n", time);
