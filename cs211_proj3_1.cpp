#include <mpi.h>
#include <math.h>
#include <iostream>
#include "MyMPI.h"
#define MIN(a,b) ((a) < (b)) ? (a) : (b))

//Modify the parallel Sieve of Aratosthenes program in class so that the program does NOT set aside memory for even integers.

int main(int argc, char*argv[])
{

    MPI_Init (&argc, &argv); // initiaslizes anything if necessary
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI-Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &id); // determines process rank
    MPI_Comm_size(MPI_COMM_WORLD, &p); //determins number of processes
   
    if (argc != 2)
    {
        if(!id) printf ("Command line: %s <m>\n", argv[0]);
        MPI_Finalize(); 
        exit(1);
    }
    
    n = atoi(argv[1]);
    low_value = 2 + BLOCK_LOW(id, p, n-1);
    high_value = 2 + BLOCK_HIGH(id,p,n-1);
    size = BLOCK_SIZE(id,p,n-1);
    proc0_size = (n-1)/p;
    if((2 + proc0_size) < (int) sqrt((double) n))
    {
        if (!id) printf ("Too many processes\n");
        MPI_Finalize();
        exit(1);
    }

    marked = (char *) malloc (size);
    if (marked == NULL)
    {
        cout << "Cannot allocate enough memory" << endl;
        MPI_Finalize();
        exit(1);
    }

    for (i = 0; i < size; i++)
    {
        marked[i] = 0;
    }
    if(!id)
    {
        index = 0;
    }
    prime = 2;
    do 
    {
        if (prime*prime > low_value)
        {
             first = prime * prime - low_value;
        }
        else 
        {
            if (!(low_value % prime)) 
            {
                first = 0;
            }
            else
            {
                first = prime - (low_value % prime);
            }
        }
        for (i = first; i < size; i += prime)
        {
            marked[i] = 1;
        }
        if (!id)
        {
            while (marked[++index]);
            prime = index + 2;
        }
        MPI_Bcast (&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }   while (prime * prime <= n);

    count = 0;
    for (i = 0; i < size; i++)
    {
        if (!marked[i])
        {
            count++;
        }
    }

    MPI_Reduce (&count, &global_count, 1, MPI_INT, MPI_SUM,0,MPI_COMM_WORLD);
    elapsed_time += MPI_Wtime();
    if (!id)
    {
        printf("%d primes are less than or equal to %d\n", global_count, n);
        printf("Total elapsed time: %10.6f\n", elapsed_time);
    }
    MPI_Finalize();
    return 0;

}

//compile by doing mpiCC -o prog prog.cpp
//execution by mpiexec -v -np no_procs prog
//mpiexec -np 100 --hostfile hosts prog // runs 100 processes on 4 computers
