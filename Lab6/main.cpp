#define MSMPI_NO_DEPRECATE_20
#include <complex>
#include <vector>
#include <cmath>
#include <stdio.h>
#include "mpi.h"

#include "Utils/utils.h"
#include "Long_Word/long_word.h"
using namespace std;

/**
* Starting script for manual call in unix systems
* ./start.sh
*
* Starting commend for manual debug without MPI
* clear && gcc -o test main.temp3 ./Long_Word/long_word.temp3 && ./test
*/

/**
*
* Задание.
* Реализуйте на основе технологии MPI многопоточную программу умножения длинных
* целых чисел методом Шёнхаге — Штрассена, используя виртуальные топологии.
* Сравните результаты работы с программами умножения длинных целых, реализованными
* в Л.Р. NoNo 4 и 5.
*/

int main(int argc, char* argv[]) {
    const int NUM_LENGTH = 4;
    const int NUM_AMOUT = 9;
    const int HALF_AMOUNT = NUM_AMOUT / 2;
    bool is_even = 1;
    if (NUM_AMOUT % 2 == 1) is_even = 0;

    int* source_num = new int[NUM_LENGTH];
    vector<int> temp1, temp2, temp3;

    /* for result from 1, 2 procs */
    int* result1 = new int[NUM_LENGTH];
    int* result2 = new int[NUM_LENGTH];
    int part1_size = 0, part2_size = 0;

    int ProcNum, ProcRank;
    MPI_Status Status;

    /* ------------ Begin of communism area ------------ */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    /* 2D net creation topology */
    MPI_Comm GridComm;
    const int dimensions_number = 2;
    int dimensions[dimensions_number], periodic[dimensions_number];
    int reorder = 1, max_dimensions = 2;

    dimensions[0] = dimensions[1] = 2;
    periodic[0] = periodic[1] = 1;
    MPI_Cart_create(MPI_COMM_WORLD, dimensions_number, dimensions, periodic, reorder, &GridComm);

    int place[dimensions_number * dimensions_number][2];
    /* procs from (0,0) (Main proc) part */
    if (ProcRank == 0) {
        printf("Topology: \n");
        for (int rank = 0; rank < dimensions_number*dimensions_number; rank++) {
            MPI_Cart_coords(GridComm, rank, dimensions_number, place[rank]);
            printf("#%d - (%d, %d)\n", rank, place[rank][0], place[rank][1]);
        }

        /* data initialization */
        for (int i = 0; i < NUM_LENGTH; i++)
            source_num[i] = 5;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(source_num, NUM_LENGTH, MPI_INT, 0, MPI_COMM_WORLD);

    /* procs from (0,1) & (1,0) part */
    if (ProcRank == 1 || ProcRank == 2) {
        part1_size = NUM_LENGTH;

        for (int i = 0; i < part1_size; i++)
            temp1.push_back(source_num[i]);
        for (int i = 0; i < part1_size; i++)
            temp2.push_back(source_num[i]);

        printf("#%d - multy\n", ProcRank);
        for (int i = 0; i < HALF_AMOUNT - 1; i++) {
            temp3 = Long_Word_mul(temp1, temp2);
            for (int i = temp3.size() - 1; i >= 0; i--)
                printf("[%d]", temp3[i]);
            printf("\n");
            part1_size = temp3.size();
            temp1 = temp3;
        }

        /* proc from (0,1) part */
        if (ProcRank == 1) {
            result1 = new int[part1_size];
            for (int i = 0; i < part1_size; i++)
                result1[i] = temp3.at(i);

            MPI_Send(&part1_size, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
            MPI_Send(result1, part1_size, MPI_INT, 3, 0, MPI_COMM_WORLD);
        }

        /* proc from (1,0) part */
        if (ProcRank == 2) {
            if (!is_even) {
                temp3 = Long_Word_mul(temp1, temp2);
                for (int i = temp3.size() - 1; i >= 0; i--)
                    printf("[%d]", temp3[i]);
                printf("\n");
                part2_size = temp3.size();
            }
            result2 = new int[part2_size];
            for (int i = 0; i < part2_size; i++)
                result2[i] = temp3.at(i);

            MPI_Send(&part2_size, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
            MPI_Send(result2, part2_size, MPI_INT, 3, 0, MPI_COMM_WORLD);
        }
    }

    /* ------- Final calculations & send part from (1,1) procs part ------- */
    if (ProcRank == 3) {
        printf("#%d - result multy", ProcRank);

        /* proc from (0,1) */
        MPI_Recv(&part1_size, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &Status);
        result1 = new int[part1_size];
        MPI_Recv(result1, part1_size, MPI_INT, 1, 0, MPI_COMM_WORLD, &Status);
        printf("\nA: ");
        for (int i = part1_size - 1; i >= 0; i--)
            printf("[%d]", result1[i]);

        /* proc from (1,0) part */
        MPI_Recv(&part2_size, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, &Status);
        result2 = new int[part2_size];
        MPI_Recv(result2, part2_size, MPI_INT, 2, 0, MPI_COMM_WORLD, &Status);
        printf("\nB: ");
        for (int i = part2_size - 1; i >= 0; i--)
            printf("[%d]", result2[i]);

        for (int i = 0; i < part1_size; i++)
            temp1.push_back(result1[i]);
        for (int i = 0; i < part2_size; i++)
            temp2.push_back(result2[i]);

        temp3 = Long_Word_mul(temp1, temp2);
        printf("\nR: ");
        for (int i = temp3.size() - 1; i >= 0; i--)
            printf("[%d]", temp3[i]);
        printf("\n");

        /* MPI clearing */
        MPI_Comm_free(&GridComm);
    }

    delete[] source_num;
    delete[] result1;
    delete[] result2;

    /* ------------ End of communism area ------------ */
    MPI_Finalize();
    return 1;
}
