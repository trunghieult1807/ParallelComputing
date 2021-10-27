#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define N 100000

float dataArray[N];
float sumLocal(float A[], long chunk);

int main(int argc, char **argv)
{
    int rank, size;
    long i, offset, chunk;
    float localSum, globalSum;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    chunk = N / size + 1;

    float recvbuf[chunk];

    if (rank == 0)
    {
        for (i = 0; i < N; i++)
        {
            dataArray[i] = random() % 1000;
        }
    }

    MPI_Scatter(dataArray, chunk, MPI_FLOAT, recvbuf, chunk, MPI_FLOAT, 0, MPI_COMM_WORLD);

    localSum = sumLocal(recvbuf, chunk);

    MPI_Reduce(&localSum, &globalSum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        fprintf(stdout, "\n The result is : %f \n", globalSum);
    }
    MPI_Finalize();
    return 0;
}

float sumLocal(float A[], long chunk)
{
    long i;
    float s = 0;
    for (i = 0; i < chunk; i++)
    {
        s += A[i];
    }
    return s;
}