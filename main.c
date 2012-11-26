#include <stdio.h> 
#include "mpi.h"
#define DEFAULT_TAG 0


double f(double x) {
	return x;
}

int main(int argc, char **argv) {  
    double left = 0;
    double right = 1;
    double n = 5000000;
    
    int rank, size, i, start, finish;
    double n_per_process, step, local_sum = 0, sum = 0;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    n_per_process = ceil(n/size);
    step = (right - left)/n; 
    start = rank*n_per_process;
    if (rank == size - 1) {
    	for(i = start; i < n - 1; i++) {
    		local_sum += step*f(i*step);
    	}
    } else {
    	finish = start + n_per_process;
    	for(i = start; i < finish; i++) {
    		local_sum += step*f(i*step);
    	}
    }
    MPI_Allreduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
	
	printf("Sum = %f on the process %d\n", sum, rank);
	
	MPI_Finalize();
    return 0;
}
