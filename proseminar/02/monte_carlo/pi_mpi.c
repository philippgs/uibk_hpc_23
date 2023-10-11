#include "xoshiro256plus.c"
#include <mpi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
	if(argc < 2) {
		printf("Usage: %s <N>\n", argv[0]);
		return EXIT_FAILURE;
	}
	uint64_t N = strtoul(argv[1], NULL, 10);

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

	double starttime, endtime;
	starttime = MPI_Wtime();

	uint64_t inside = 0;
	uint64_t timeSeed = time(NULL) + myRank;

	s[0] = timeSeed;
	s[1] = timeSeed ^ (timeSeed >> 32);
	s[2] = ~timeSeed;
	s[3] = timeSeed << 32;

	uint64_t upper_bound = UINT64_MAX >> 2;

	for(long unsigned int i = 0; i < N / numProcs; ++i) {
		uint64_t x = next() >> 33;
		uint64_t y = next() >> 33;
		if(x * x + y * y <= upper_bound) {
			++inside;
		}
	}
	uint64_t total_inside;
	MPI_Reduce(&inside, &total_inside, 1, MPI_UINT64_T, MPI_SUM, 0, MPI_COMM_WORLD);

	if(myRank == 0) {

		endtime = MPI_Wtime();
    printf("Wall: %f seconds\n", endtime - starttime);
		printf("Pi approximately: %f\n", 4.0 * (double)total_inside / (double)N);
	}

	MPI_Finalize();
	return EXIT_SUCCESS;
}
