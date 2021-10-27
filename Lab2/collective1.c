#include <mpi.h> 
#include <stdio.h> 
int main(int argc, char **argv)   
{ 
    int i, rank, size; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    for(i = 0; i < rank; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
    }
    
    printf("Hello world, I have rank %d out of %d processes \n", rank, size); 

    for(i = 0; i < size-rank; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize(); 
    return 0; 
}