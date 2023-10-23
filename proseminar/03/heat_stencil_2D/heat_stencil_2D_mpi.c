#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef double value_t;

#define RESOLUTION 120

#define INDEX(i, j) ((i) + ((j)*bufferLengthX))

// -- vector utilities --

typedef value_t* Vector;

Vector createVector(int N);

void releaseVector(Vector X);
void factor(int n, int* fac1, int* fac2);

void printTemperature(Vector X, int n, int m);

// -- simulation code ---

int main(int argc, char** argv) {
	// 'parsing' optional input parameter = problem size
	int N = 5000;
	if(argc > 1) {
		N = atoi(argv[1]);
	}

	int myRank, numProcs;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

	if(myRank == 0) {
		printf("Setup: number of Processes = %d\n", numProcs);
	}

	int T = N * 100;
	printf("Computing heat-distribution for room size N=%d for T=%d timesteps\n", N, T);

	// ---------- setup ----------
	// assumed square problem space
	int ranksX, ranksY;
	factor(numProcs, &ranksX, &ranksY);
	int rankLengthX = N / ranksX;
	int rankLengthY = N / ranksY;
	int bufferLengthX = rankLengthX + 2;
	int bufferLengthY = rankLengthY + 2;

	int idX = myRank % ranksX;
	int idY = myRank / ranksX;

	int top = (idY - 1) * ranksX + idX;
	if(idY == 0) top = MPI_PROC_NULL;
	int bottom = (idY + 1) * ranksX + idX;
	if(idY + 1 == ranksY) bottom = MPI_PROC_NULL;
	int left = idY * ranksX + idX - 1;
	if(idX == 0) left = MPI_PROC_NULL;
	int right = idY * ranksX + idX + 1;
	if(idX + 1 == ranksX) right = MPI_PROC_NULL;

	MPI_Datatype topBottom, leftRight;
	MPI_Type_contiguous(rankLengthX, MPI_DOUBLE, &topBottom);
	MPI_Type_commit(&topBottom);

	MPI_Type_vector(rankLengthY, 1, bufferLengthX, MPI_DOUBLE, &leftRight);
	MPI_Type_commit(&leftRight);

	MPI_Request reqs[8];

	// create a buffer for storing temperature fields
	Vector A = createVector(bufferLengthX * bufferLengthY);
	// create a second buffer for the computation
	Vector B = createVector(bufferLengthX * bufferLengthY);

	// set up initial conditions in A
	for(int j = 0; j < bufferLengthY; j++) {
		for(int i = 0; i < bufferLengthX; i++) {
			A[INDEX(i, j)] = 273; // temperature is 0° C everywhere (273 K)
			B[INDEX(i, j)] = 273; // temperature is 0° C everywhere (273 K)
		}
	}

	// and there is a heat source in one corner
	int source_x = N / 4;
	int source_y = N / 4;
	if(source_x >= idX * rankLengthX - 1 && source_x < (idX + 1) * rankLengthX + 1 &&
	   source_y >= idY * rankLengthY - 1 && source_y < (idY + 1) * rankLengthY + 1) {
		source_x = source_x - idX * rankLengthX + 1;
		source_y = source_y - idY * rankLengthY + 1;
		A[INDEX(source_x, source_y)] = 273 + 60;
	} else {
		source_x = -1;
		source_y = -1;
	}

	if(source_x != -1) {
		printf("Initial from rank %d (x:%d, y:%d)(source: %d %d):\n", myRank, idX, idY, source_x,
		       source_y);
		printTemperature(A, bufferLengthX, bufferLengthY);
		printf("\n");
	}

	// ---------- compute ----------

	double starttime, endtime;
	starttime = MPI_Wtime();

	// for each time step ..
	for(int t = 0; t < T; t++) {

		MPI_Isend(&A[INDEX(1, 1)] /* north */, 1, topBottom, top, 0, MPI_COMM_WORLD, &(reqs[0]));
		MPI_Isend(&A[INDEX(1, rankLengthY)] /* south */, 1, topBottom, bottom, 0, MPI_COMM_WORLD,
		          &(reqs[1]));
		MPI_Isend(&A[INDEX(rankLengthX, 1)] /* east */, 1, leftRight, right, 0, MPI_COMM_WORLD,
		          &(reqs[2]));
		MPI_Isend(&A[INDEX(1, 1)] /* west */, 1, leftRight, left, 0, MPI_COMM_WORLD, &(reqs[3]));
		MPI_Irecv(&A[INDEX(1, 0)] /* north */, 1, topBottom, top, 0, MPI_COMM_WORLD, &(reqs[4]));
		MPI_Irecv(&A[INDEX(1, rankLengthY + 1)] /* south */, 1, topBottom, bottom, 0,
		          MPI_COMM_WORLD, &(reqs[5]));
		MPI_Irecv(&A[INDEX(rankLengthX + 1, 1)] /* west */, 1, leftRight, right, 0, MPI_COMM_WORLD,
		          &(reqs[6]));
		MPI_Irecv(&A[INDEX(0, 1)] /* east */, 1, leftRight, left, 0, MPI_COMM_WORLD, &(reqs[7]));
		MPI_Waitall(8, &(reqs[0]), MPI_STATUSES_IGNORE);

		for(long long j = 1; j < bufferLengthY - 1; j++) {
			// .. we propagate the temperature
			for(long long i = 1; i < bufferLengthX - 1; i++) {
				// center stays constant (the heat is still on)
				if(j == source_y && i == source_x) {
					B[INDEX(i, j)] = A[INDEX(i, j)];
					continue;
				}

				// get temperature at current position
				value_t tc = A[INDEX(i, j)];
				// get temperatures of adjacent cells
				value_t tl = idX == 0 && i == 1 ? tc : A[INDEX(i - 1, j)];
				value_t tr = idX == ranksX - 1 && i == rankLengthX ? tc : A[INDEX(i + 1, j)];
				value_t ta = idY == 0 && j == 1 ? tc : A[INDEX(i, j - 1)];
				value_t tb = idY == ranksY - 1 && j == rankLengthY ? tc : A[INDEX(i, j + 1)];

				// compute new temperature at current position
				B[INDEX(i, j)] = tc + 0.2 * (tl + tr + tb + ta + (-4 * tc));
			}
		}
		// swap matrices (just pointers, not content)
		Vector H = A;
		A = B;
		B = H;
		// show intermediate step
	}

	Vector res = createVector(N * N);

	if(myRank == 0) {
		// Prepare a temporary array to hold data from each process
		Vector temp = createVector(rankLengthX * rankLengthY);

		// Copy the local result from A to the appropriate place in res
		for(int j = 1; j <= rankLengthY; j++) {
			for(int i = 1; i <= rankLengthX; i++) {
				res[i - 1 + (j - 1) * N] = A[INDEX(i, j)];
			}
		}
		// Gather results from other ranks
		for(int rank = 1; rank < numProcs; ++rank) {
			MPI_Recv(temp, rankLengthX * rankLengthY, MPI_DOUBLE, rank, 0, MPI_COMM_WORLD,
			         MPI_STATUS_IGNORE);

			// Determine the offset based on the rank
			int offsetX = (rank % ranksX) * rankLengthX;
			int offsetY = (rank / ranksX) * N * rankLengthY;

			// Copy the received data to the appropriate place in res
			for(int j = 0; j < rankLengthY; ++j) {
				for(int i = 0; i < rankLengthX; ++i) {
					res[offsetX + i + offsetY + (j * N)] = temp[i + j * rankLengthX];
				}
			}
		}

		releaseVector(temp);
	} else {
		// Prepare a temporary array to hold the relevant data from A
		Vector temp = createVector(rankLengthX * rankLengthY);

		// Copy the data from A (skipping the halo)
		for(int j = 1; j <= rankLengthY; j++) {
			for(int i = 1; i <= rankLengthX; i++) {
				temp[i - 1 + (j - 1) * rankLengthX] = A[INDEX(i, j)];
			}
		}
		// Send the data to rank 0
		MPI_Send(temp, rankLengthX * rankLengthY, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

		releaseVector(temp);
	}

	int success = 1;
	if(myRank == 0) {

		endtime = MPI_Wtime();
		printf("Wall: %f seconds\n", endtime - starttime);
		// ---------- check ----------

		printf("Final:\t\n");
		printTemperature(res, N, N);
		printf("\n");

		int success = 1;
		for(long long i = 0; i < N; i++) {
			for(long long j = 0; j < N; j++) {
				value_t temp = res[INDEX(i, j)];
				if(273 <= temp && temp <= 273 + 60) continue;
				success = 0;
				break;
			}
		}

		printf("Verification: %s\n", (success) ? "OK" : "FAILED");
	}

	// ---------- cleanup ----------
	MPI_Type_free(&topBottom);
	MPI_Type_free(&leftRight);
	MPI_Finalize();
	releaseVector(A);
	releaseVector(B);
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

void printTemperature(Vector X, int bufferLengthX, int bufferLengthY) {
	const char* colors = " .-:=+*^X#%@";
	const int numColors = 12;

	// boundaries for temperature (for simplicity hard-coded)
	const value_t max = 273 + 30;
	const value_t min = 273 + 0;
	int W = RESOLUTION;
	int sH = bufferLengthY / W;
	int sW = bufferLengthX / W;
	for(int j = 0; j < W; j++) {
		printf("X");
		// actual room
		for(int i = 0; i < W; i++) {
			// get max temperature in this tile
			value_t max_t = 0;
			for(int y = sH * j; y < sH * j + sH; y++) {
				for(int x = sW * i; x < sW * i + sW; x++) {
					max_t = (max_t < X[INDEX(x, y)]) ? X[INDEX(x, y)] : max_t;
				}
			}
			value_t temp = max_t;

			// pick the 'color'
			int c = ((temp - min) / (max - min)) * numColors;
			c = (c >= numColors) ? numColors - 1 : ((c < 0) ? 0 : c);

			// print the average temperature
			printf("%c", colors[c]);
		}
		// right wall
		printf("X\n");
	}
}

// generated by Copilot
void factor(int r, int* fac1, int* fac2) {
	for(*fac1 = (int)(sqrt(r + 1.0)); *fac1 > 0; (*fac1)--) {
		if(!(r % (*fac1))) {
			*fac2 = r / (*fac1);
			break;
		}
	}
}
