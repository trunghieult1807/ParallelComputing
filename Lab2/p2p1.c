#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
    int rank, size;
    int in;
    int out;
    int prev, next;
    int sum = 0;
    int i;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    prev = (rank - 1 + size) % size;
    next = (rank + 1) % size;

    in = rank;
    for (i = 0; i < size; i++)
    {
        MPI_Send(&in, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        MPI_Recv(&out, 1, MPI_INT, prev, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        in = out;
        sum += out;
    }

    printf("rank %d, sum = %d\n", rank, sum);

    MPI_Finalize();
    return 0;
}