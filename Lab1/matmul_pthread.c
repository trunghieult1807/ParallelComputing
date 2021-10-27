#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#define NUM_THREADS 3

struct Info
{
    int offset;
    int len;
    int size;
};

double *A;
double *B;
double *C;

void *thread_func(void *arg)
{
    struct Info *info;
    int offset, len, size, start, end, i, j;
    info = (struct Info *)arg;
    offset = info->offset;
    len = info->len;
    size = info->size;
    start = offset * len;
    end = start + len;
    for (i = start; i < end; ++i)
    {
        if (i >= size * size) {
            break;
        }
        int row, col;
        row = i / size;
        col = i % size;
        for (j = 0; j < size; ++j)
        {
            C[row * size + col] += A[row * size + j] * B[j * size + col];
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: <file> <matrix_size>\n");
        exit(-1);
    }
    int n = atoi(argv[1]);

    A = (double *)malloc(sizeof(double) * n * n);
    B = (double *)malloc(sizeof(double) * n * n);
    C = (double *)malloc(sizeof(double) * n * n);

    int i, j;
    for (i = 0; i < n; ++i)
    {
        for (j = 0; j < n; ++j)
        {
            A[i * n + j] = (double)i / (double)n;
            B[i * n + j] = (double)j / (double)n;
            C[i * n + j] = 0.0;
        }
    }
    void *status;
    int len;
    len = ceil((double)(n * n) / NUM_THREADS);
    pthread_t threads[NUM_THREADS];
    struct Info thread_params[NUM_THREADS];
    int create_flag;
    for (i = 0; i < NUM_THREADS; ++i)
    {
        
        thread_params[i].offset = i;
        thread_params[i].len = len;
        thread_params[i].size = n;

        create_flag = pthread_create(&threads[i], NULL, thread_func, (void *)&thread_params[i]);
        if (create_flag)
        {
            printf("ERROR: return code from pthread_create() is %d\n", create_flag);
            exit(-1);
        }
    }
    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], &status);
    }

    double norm = 0.0;
    for (i = 0; i < n; ++i)
    {
        for (j = 0; j < n; ++j)
        {
            double term = C[i * n + j] - (double)(i * j) / (double)n;
            norm += term * term;
        }
    }
    if (norm > 1e-10)
        printf("Something wrong... Norm is equal to %f\n", norm);
    else
        printf("Yup, we're good\n");

    free(A);
    free(B);
    free(C);
    pthread_exit(NULL);
    return 0;
}
