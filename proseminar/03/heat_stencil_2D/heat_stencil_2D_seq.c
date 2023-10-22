#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef double value_t;

#define RESOLUTION 120

// -- vector utilities --

typedef value_t* Vector;
typedef Vector* Vector2D;

Vector createVector(int N);
Vector2D createVectorSpace(int m, int n);

void releaseVector(Vector m);
void releaseVectorSpace(Vector2D X, int m);

void printTemperature(Vector m, int N);
void printTemperatureSpace(Vector2D X, int m, int n);

// -- simulation code ---

int main(int argc, char** argv) {
	// 'parsing' optional input parameter = problem size
	int N = 5000;
	if(argc > 1) {
		N = atoi(argv[1]);
	}
	int T = N * 500;
	printf("Computing heat-distribution for room size N=%d for T=%d timesteps\n", N, T);

	// ---------- setup ----------
	// create a buffer for storing temperature fields
	Vector2D A = createVectorSpace(N, N);
	// create a second buffer for the computation
	Vector2D B = createVectorSpace(N, N);

	// set up initial conditions in A
	for(int j = 0; j < N; j++) {
		for(int i = 0; i < N; i++) {
			A[j][i] = 273; // temperature is 0° C everywhere (273 K)
		}
	}

	// and there is a heat source in one corner
	int source_x = N / 4;
	int source_y = N / 4;
	A[source_y][source_x] = 273 + 60;

	printf("Initial:\n");
	printTemperatureSpace(A, N, N);
	printf("\n");

	// ---------- compute ----------

	clock_t start, end;
	start = clock();

	// for each time step ..
	for(int t = 0; t < T; t++) {
		for(long long j = 0; j < N; j++) {
			Vector old_row = A[j];
			Vector old_below = (j != 0) ? A[j - 1] : A[j];
			Vector old_above = (j != N - 1) ? A[j + 1] : A[j];
			Vector new_row = B[j];
			// .. we propagate the temperature
			for(long long i = 0; i < N; i++) {
				// center stays constant (the heat is still on)
				if(j == source_y && i == source_x) {
					new_row[i] = old_row[i];
					continue;
				}

				// get temperature at current position
				value_t tc = old_row[i];

				// get temperatures of adjacent cells
				value_t tl = (i != 0) ? old_row[i - 1] : tc;
				value_t tr = (i != N - 1) ? old_row[i + 1] : tc;
				value_t ta = old_above[i];
				value_t tb = old_below[i];

				// compute new temperature at current position
				new_row[i] = tc + 0.2 * (tl + tr + tb + ta + (-4 * tc));
			}
		}
		// swap matrices (just pointers, not content)
		Vector2D H = A;
		A = B;
		B = H;

		// show intermediate step
		if(!(t % 10000)) {
			printf("Step t=%d:\n", t);
			printTemperatureSpace(A, N, N);
			printf("\n");
		}
	}

	releaseVectorSpace(B, N);

	end = clock();
	printf("Wall: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
	// ---------- check ----------

	printf("Final:\t\n");
	printTemperatureSpace(A, N, N);
	printf("\n");

	int success = 1;
	for(long long i = 0; i < N; i++) {
		for(long long j = 0; j < N; j++) {
			value_t temp = A[i][j];
			if(273 <= temp && temp <= 273 + 60) continue;
			success = 0;
			break;
		}
	}

	printf("Verification: %s\n", (success) ? "OK" : "FAILED");

	// ---------- cleanup ----------

	releaseVectorSpace(A, N);

	// done
	return (success) ? EXIT_SUCCESS : EXIT_FAILURE;
}

Vector createVector(int N) {
	// create data and index vector
	return malloc(sizeof(value_t) * N);
}

Vector2D createVectorSpace(int m, int n) {
	Vector2D vectorSpace = malloc(sizeof(Vector) * m);
	for(int i = 0; i < m; i++) {
		vectorSpace[i] = createVector(n);
	}
	return vectorSpace;
}

void releaseVector(Vector m) {
	free(m);
}

void releaseVectorSpace(Vector2D X, int m) {
	for(int i = 0; i < m; i++) {
		free(X[i]);
	}
	free(X);
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

void printTemperatureSpace(Vector2D X, int m, int n) {
	const char* colors = " .-:=+*^X#%@";
	const int numColors = 12;

	// boundaries for temperature (for simplicity hard-coded)
	const value_t max = 273 + 30;
	const value_t min = 273 + 0;
	int W = RESOLUTION;
	int sH = m / W;
	int sW = n / W;
	for(int j = 0; j < W; j++) {
		printf("X");
		// actual room
		for(int i = 0; i < W; i++) {
			// get max temperature in this tile
			value_t max_t = 0;
			for(int y = sH * j; y < sH * j + sH; y++) {
				for(int x = sW * i; x < sW * i + sW; x++) {
					max_t = (max_t < X[y][x]) ? X[y][x] : max_t;
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
