#include "xoshiro256plus.c"
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

	clock_t start, end;
	start = clock();

	uint64_t inside = 0;
	uint64_t timeSeed = time(NULL);

	s[0] = timeSeed;
	s[1] = timeSeed ^ (timeSeed >> 32);
	s[2] = ~timeSeed;
	s[3] = timeSeed << 32;

	uint64_t upper_bound = UINT64_MAX >> 2;

	for(uint64_t i = 0; i < N; ++i) {
		uint64_t x = next() >> 33;
		uint64_t y = next() >> 33;
		if(x * x + y * y <= upper_bound) {
			++inside;
		}
	}

	end = clock();
	printf("Wall: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
	printf("Pi approximately: %f\n", 4.0 * (double)inside / (double)N);

	return EXIT_SUCCESS;
}
