#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define min(a, b) ((a) < (b) ? (a) : (b))

int n_elems, numtasks;

int main(int argc, char *argv[])
{
    n_elems = atoi(argv[1]);
    int numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // alocare vectori
    int* v1 = malloc(n_elems * sizeof(int));
    int* v2 = malloc(n_elems * sizeof(int));
    int *rez = calloc(n_elems, sizeof(int));
   
    // proces parinte - initializeaza vectorii
    if (rank == 0)
    {
        MPI_Status status;
        for (int i = 0; i < n_elems; i++) {
            v1[i] = i;
        }
        for (int i = 0; i < n_elems; i++) {
            v2[i] = 3 * i;
        }

        printf("v1: ");
        for (int i = 0; i < n_elems; i++) {
            printf("%d ", v1[i]);
        }
        printf("\n");
        
        printf("v2: ");
        for (int i = 0; i < n_elems; i++) {
            printf("%d ", v2[i]);
        }
        printf("\n");
    }

    // trimit vectorii
    MPI_Bcast(v1, n_elems, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(v2, n_elems, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0)
    {
        MPI_Status status;
        int start = (rank - 1) * (double)n_elems / (numtasks - 1);
        int end = min(rank * (double)n_elems / (numtasks - 1), n_elems);

        for(int i = start; i < end; ++i) {
            rez[i] = v1[i] + v2[i];
        }

        MPI_Send(rez + start, end - start, MPI_INT, 0, 1, MPI_COMM_WORLD);
        
    } else {
        MPI_Status status;
        for (int i = 1; i < numtasks; i++) {
            int start = (i - 1) * (double)n_elems / (numtasks - 1);
            int end = min(i * (double)n_elems / (numtasks - 1), n_elems);

            MPI_Recv(rez + start, end - start, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
        }

        printf("rez: ");
        for (int i = 0; i < n_elems; i++) {
            printf("%d ", rez[i]);
        }
        printf("\n");
    }

    free(v1);
    free(v2);
    free(v);

    MPI_Finalize();
}