#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv)
{
    int rank, size;
    MPI_Status Stat;
    unsigned long long total_pts = 1000000000;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int circle_pts = 0;
    long len = total_pts / (long)size;
    if (rank == 0)
    {
        float x, y, z;
        unsigned int j = rand();
        long int i;
        for (i = 0; i < len; i++)
        {
            x = (float)rand_r(&j) / RAND_MAX;
            y = (float)rand_r(&j) / RAND_MAX;
            x = x * 2 - 1;
            y = y * 2 - 1;
            z = x * x + y * y;
            if (z <= 1)
            {
                circle_pts += 1;
            }
        }
        for (i = 1; i < size; i++)
        {
            int recv = 0;
            MPI_Recv(&recv, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &Stat);
            circle_pts += recv;
        }
        double pi = 4 * ((double)circle_pts / (double)total_pts);
        fprintf(stdout, "The value of Pi is: %.10f\n", pi);
    }
    else
    {
        float x, y, z;
        unsigned int j = rand();
        long int i;
        for (i = 0; i < len; i++)
        {
            x = (float)rand_r(&j) / RAND_MAX;
            y = (float)rand_r(&j) / RAND_MAX;
            x = x * 2 - 1;
            y = y * 2 - 1;
            z = x * x + y * y;
            if (z <= 1)
            {
                circle_pts += 1;
            }
        }
        MPI_Send(&circle_pts, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}