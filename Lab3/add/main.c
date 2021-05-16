#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "../Utils/utils.h"

/**
*
* Модифицируйте код следующим образом:
*
* Входные данные - количество процессов, количество рассылаемых в каждый поток чисел.
* Выходные (вывести в консоль) - начальные числа (рандомим, не превышают 10
* и больше 0), промежуточные результаты, результаты префиксной редукции.
*
* Нужно:
* 1) Нулевой рассылает начальные данные. (MPI_Scatter)
* 2) Каждый процесс далее по кольцу
*    2.1) модифицирует свой кусок данных;
*    2.2) к нему через редукцию (MPI_Reduce) прибавляет данные из других процессов;
*    2.3) передает (MPI_Bcast) далее по кольцу.
* 3) В конце кольца нулевой выполняет префиксную редукцию и все процессы после
*     этого выдают результат.
*
* Фактически, тут нет повторов кольца.
* Просто перебираем от нулевого в конец: 0->1->2->...->n. Затем в нулевом MPI_Scan
* Естественно пробуем широковещательную, причем именно из 3, т.к в каждый процесс
* идут свои собственные порции данных
*
*/

// int MPI_Scatter(void *sbuf,int scount,MPI_Datatype stype,void *rbuf,int rcount,MPI_Datatype rtype, int root, MPI_Comm comm)
// int MPI_Reduce(void *sendbuf, void *recvbuf,int count,MPI_Datatype type, MPI_Op op,int root,MPI_Comm comm);
// int MPI_Scan(void *sendbuf, void *recvbuf,int count,MPI_Datatype type, MPI_Op op,MPI_Comm comm).
// int MPI_Bcast(void *buf,int count,MPI_Datatype type,int root,MPI_Comm comm);


int main(int argc, char** argv) {
  // Init phase
  int proc_rank, proc_num, recvRank;
  MPI_Status status;

  // Begin of communism area
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

  int* source_array;  // Send buff
  int* sub_array;     // Receive buff
  int op_status, full_sum = 0, proc_sum = 0, final_sum = 0;
  int elements_per_proc = 10;

  if (proc_rank == 0) {
    printf("First proc inits array %d-%d-%d \n", proc_num, elements_per_proc, proc_num * elements_per_proc);
    source_array = get_rand_source_array(proc_num * elements_per_proc);
    for (size_t i = 0; i < proc_num * elements_per_proc; i++) printf("%d ", source_array[i]);
  }

  sub_array = malloc(sizeof(int) * elements_per_proc);

  // 1) Нулевой рассылает начальные данные. (MPI_Scatter)
  op_status = MPI_Scatter(source_array, elements_per_proc, MPI_INT, sub_array,
              elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

  printf("\nProc-%d recived:\n", proc_rank);
  for (size_t i = 0; i < elements_per_proc; i++) printf("%d ", sub_array[i]);
  printf("\n");

  for (int i = 0; i < proc_num; i++) {

    int root = i, is_root = (proc_rank == root);

    if (is_root) {
      // 2.1) модифицирует свой кусок данных;
      for (size_t i = 0; i < elements_per_proc; i++) proc_sum += sub_array[i];
      printf("Proc-%d модифицирует свой кусок данных: %d\n", proc_rank, proc_sum);
    }

    // 2.2) к нему через редукцию (MPI_Reduce) прибавляет данные из других процессов
    op_status = MPI_Reduce(&proc_sum, &full_sum, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
    if (is_root)  printf("Proc-%d через редукцию прибавляет данные из других процессов: %d\n", proc_rank, full_sum);

    // 2.3) передает (MPI_Bcast) далее по кольцу.
    op_status = MPI_Bcast(&full_sum , 1, MPI_INT,root, MPI_COMM_WORLD);
    if (is_root) printf("Proc-%d передает (MPI_Bcast) далее по кольцу: %d\n", proc_rank, full_sum);

  }

  MPI_Barrier(MPI_COMM_WORLD);

  free(sub_array);
  if (proc_rank == 0) free(source_array);

  op_status = MPI_Scan(&full_sum, &final_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  printf("Proc-%d выдает результат: %d\n", proc_rank, final_sum);

  // End of communism area
  MPI_Finalize();

  return 0;
}
