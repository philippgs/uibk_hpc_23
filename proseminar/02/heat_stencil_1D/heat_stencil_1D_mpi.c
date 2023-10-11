#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

typedef double value_t;

#define RESOLUTION 120

// -- vector utilities --

typedef value_t* Vector;

Vector createVector(int N);

void releaseVector(Vector m);

void printTemperature(Vector m, int N);

// -- simulation code ---

int main(int argc, char** argv) {
	// 'parsing' optional input parameter = problem size
	int N = 2000;
	if(argc > 1) {
		N = atoi(argv[1]);
	}

	int myRank, numProcs;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

	// Ensure that work is evenly distributed
	// allows integer divisions
	// could be easily fixed
	if(N % numProcs != 0) {
		MPI_Finalize();
		return EXIT_FAILURE;
	}
	int T = N * 500;
	printf("Computing heat-distribution for room size N=%d for T=%d timesteps\n", N, T);

	double starttime, endtime;
	starttime = MPI_Wtime();
	// ---------- setup ----------
	int rankLength = N / numProcs;
	int bufferLength = rankLength + 2;

	// create a buffer for storing temperature fields
	Vector A = createVector(bufferLength);

	// set up initial conditions in A
	for(int i = 0; i < bufferLength; i++) {
		A[i] = 273; // temperature is 0° C everywhere (273 K)
	}

	// and there is a heat source in one corner
	int source_x = N / 4;
	if(source_x >= myRank * rankLength && source_x < (myRank + 1) * rankLength) {
		A[(source_x % rankLength) + 1] = 273 + 60;
	}
	if(0) {
		printf("Initial:\t");
		printTemperature(A, N);
		printf("\n");
	}

	// ---------- compute ----------

	// create a second buffer for the computation
	Vector B = createVector(bufferLength);

	// for each time step ..
	for(int t = 0; t < T; t++) {
		// .. we propagate the temperature

		// has left neighbor
		if(myRank != 0) {
			MPI_Sendrecv(&A[1], 1, MPI_DOUBLE, myRank - 1, 0, &A[0], 1, MPI_DOUBLE, myRank - 1, 0,
			             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		// has right neighbor
		if(myRank != numProcs - 1) {
			MPI_Sendrecv(&A[rankLength], 1, MPI_DOUBLE, myRank + 1, 0, &A[rankLength + 1], 1,
			             MPI_DOUBLE, myRank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		for(long long i = 0; i < bufferLength; i++) {
			// center stays constant (the heat is still on)
			if(i == source_x % rankLength + 1 && myRank == source_x / rankLength) {
				B[i] = A[i];
				continue;
			}

			// get temperature at current position
			value_t tc = A[i];

			// get temperatures of adjacent cells
			value_t tl = (i != 0) ? A[i - 1] : tc;
			value_t tr = (i != bufferLength - 1) ? A[i + 1] : tc;

			// compute new temperature at current position
			B[i] = tc + 0.2 * (tl + tr + (-2 * tc));
		}

		// swap matrices (just pointers, not content)
		Vector H = A;
		A = B;
		B = H;

		// show intermediate step
		if(!(t % 10000) && myRank == 0) {
			printf("Step t=%d:\t", t);
			printTemperature(A, N);
			printf("\n");
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	releaseVector(B);

	Vector res = createVector(N);
	MPI_Gather(&A[1], rankLength, MPI_DOUBLE, res, rankLength, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	int success = 1;
	if(myRank == 0) {

		endtime = MPI_Wtime();
		printf("Wall: %f seconds\n", endtime - starttime);

		printf("Final:\t\t");
		printTemperature(res, N);
		printf("\n");

		for(long long i = 0; i < N; i++) {
			value_t temp = res[i];
			if(273 <= temp && temp <= 273 + 60) continue;
			success = 0;
			break;
		}

		printf("Verification: %s\n", (success) ? "OK" : "FAILED");
	}

	MPI_Finalize();

	// ---------- cleanup ----------

	releaseVector(A);
	releaseVector(res);

	// done
	return (success) ? EXIT_SUCCESS : EXIT_FAILURE;
}

Vector createVector(int N) {
	// create data and index vector
	return malloc(sizeof(value_t) * N);
}

void releaseVector(Vector m) {
	free(m);
}

void printTemperature(Vector m, int N) {
	const char* colors = " .-:=+*^X#%@";
	const int numColors = 12;

	// boundaries for temperature (for simplicity hard-coded)
	const value_t max = 273 + 30;
	const value_t min = 273 + 0;

	// set the 'render' resolution
	int W = RESOLUTION;

	// step size in each dimension
	int sW = N / W;

	// room
	// left wall
	printf("X");
	// actual room
	for(int i = 0; i < W; i++) {
		// get max temperature in this tile
		value_t max_t = 0;
		for(int x = sW * i; x < sW * i + sW; x++) {
			max_t = (max_t < m[x]) ? m[x] : max_t;
		}
		value_t temp = max_t;

		// pick the 'color'
		int c = ((temp - min) / (max - min)) * numColors;
		c = (c >= numColors) ? numColors - 1 : ((c < 0) ? 0 : c);

		// print the average temperature
		printf("%c", colors[c]);
	}
	// right wall
	printf("X");
}
