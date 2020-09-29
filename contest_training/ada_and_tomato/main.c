#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define N_MAX 20000000
#define X_MAX 1000000007

typedef unsigned long long_t;


//long_t x_cache[X_MAX];
long_t M = 1000000007;
long_t sizes[1000000008];


long_t slice_bits(long_t number, int s, int offset) {
	int selector = 0;
	int i;
	for (i = 0; i < s; i++) {
		selector = (selector << 1)|1;
	}
	return (number >> s*offset) & selector;
}

void print_arr(long_t *data, int size) {
	printf("data: ");
	int i;
	for (i = 0; i < size; i++) {
		printf("%ld ", data[i]);
	}
	printf("\n");
}

void radix_sort() {
	// 50 = 110010
	long_t data[] = {4, 50, 33, 12, 1};
	long_t N = 5;
	long_t b = 6;
	long_t s = 2;
	long_t rounds = b/s;
	long_t bucket[rounds][N];
	memset(bucket, 0, (rounds)*N*sizeof(long_t));

	long_t i, j;
	long_t index;
	long_t buck_index = 0;

//	for (i = 0; i < rounds; i++) {
//		for (j = 0; j < N; j++) {
//			printf("%ld ", bucket[i][j]);
//		}
//	}
//	printf("\n");
	// printf("start sorting\n");

	for (i = 0; i < rounds; i++) {
		print_arr(data, N);
		for (j = 0; j < N; j++) {
			index = slice_bits(data[j], s, i);
//			printf("slice_bits(%ld, %ld, %ld)=%ld\n", data[j],s,i, index);
			buck_index = 0;
			while (bucket[index][buck_index] != 0) buck_index++;
			printf("add %ld to bucket at index [%ld,%ld]\n", data[j], index, buck_index);
			bucket[index][buck_index] = data[j];
			// printf("added %ld\n", bucket[index][buck_index]);
		}
		// printf("collect\n");
		// collect
		printf("bucket: ");
		index = 0;
		for (j = 0; j < rounds; j++) {
			// printf("i=%ld\n", j);
			buck_index = 0;
			while (bucket[j][buck_index] != 0) {
//				printf("%ld\n", bucket[j][buck_index]);
				// printf("buck_index=%ld\n", buck_index);
				printf("%ld ", bucket[j][buck_index]);
//				data[index] = bucket[j][buck_index];
				bucket[j][buck_index] = 0;
//				index++;
				buck_index++;
			}
		}
		printf("\n");
	}
	//printf("finished\n");
}

void solve(long_t N, long_t a, long_t b, long_t X1) {
	long_t i;
	long_t x = X1;
	long_t price = X1;

	sizes[1] = x;

	for (i = 2; i < N; i++) {
		x = (((x*a)%M)+b)%M;
		sizes[i] = x;
	}
}

int main() {
//	long_t cases, N, a, b, X1;
//	scanf("%ld\n", &cases);
//	memset(results, 0, M * sizeof(long_t));
//
//	size_t i;
//	for (i = 0; i < cases; i++) {
//		scanf("%ld%ld%ld%ld\n", &N, &a, &b, &X1);
//		solve(N, a, b, X1);
//	}
	 radix_sort();

//	long_t num = 4;
//	long_t slice = slice_bits(num, 2, 1);
//	printf("%ld\n", slice);
	return 0;
}