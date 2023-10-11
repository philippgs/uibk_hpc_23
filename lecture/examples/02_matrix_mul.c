#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#ifndef SIZE
#define SIZE 8
#endif

int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];

void fill_matrix(int m[SIZE][SIZE]) {
	static int n = 0;
	for(int i = 0; i < SIZE; i++) {
		for(int j = 0; j < SIZE; j++) {
			m[i][j] = n++;
		}
	}
}

void print_matrix(int m[SIZE][SIZE]) {
	int i, j = 0;
	for(i = 0; i < SIZE; i++) {
		printf("\n\t| ");
		for(j = 0; j < SIZE; j++)
			printf("%'5d ", m[i][j]);
		printf("|");
	}
	printf("\n");
}

int main(int argc, char** argv) {

	int myRank, numProcs;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

	// only support integer-divisible sizes
	if(SIZE % numProcs != 0) {
		MPI_Finalize();
		return EXIT_FAILURE;
	}

	// root generates input data
	if(myRank == 0) {
		fill_matrix(A);
		fill_matrix(B);
	}

	// compute index range of local computation
	int start = myRank * SIZE / numProcs;
	int end = (myRank + 1) * SIZE / numProcs;

	// distribute rows of A to everyone
	MPI_Scatter(A, SIZE * SIZE / numProcs, MPI_INT, A[start], SIZE * SIZE / numProcs, MPI_INT, 0,
	            MPI_COMM_WORLD);

	// broadcast entire matrix B to everyone
	MPI_Bcast(B, SIZE * SIZE, MPI_INT, 0, MPI_COMM_WORLD);

	// local computation of every rank
	for(int i = start; i < end; i++) {
		for(int j = 0; j < SIZE; j++) {
			C[i][j] = 0;
			for(int k = 0; k < SIZE; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}

	// gather result rows back to root
	MPI_Gather(C[start], SIZE * SIZE / numProcs, MPI_INT, C, SIZE * SIZE / numProcs, MPI_INT, 0,
	           MPI_COMM_WORLD);

	if(myRank == 0) {
		print_matrix(C);
	}

	MPI_Finalize();

	return EXIT_SUCCESS;
}
