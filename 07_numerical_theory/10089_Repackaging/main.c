#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double vector_norm(int *v) {
	return sqrt(v[0]*v[0]+v[1]*v[1]);
}

double angle_between_vectors(int *v1, int *v2) {
	return acos((v1[0]*v2[0] + v1[1]*v2[1])/(vector_norm(v1) * vector_norm(v2)));
}

void check(double angles[1024], int N) {
	int i, j;
	const double pi = acos(-1);
	for (i = 0; i < N; i++) {
		for (j = i+1; j < N; j++) {
			if (angles[j] - angles[i] > pi) {
				printf("No\n");
				return;
			}
		}
	}
	printf("Yes\n");
}

int comparator(const void *el1, const void *el2) {
	return (*((double*)el1)-*((double*)el2)) > 0;
}

int main() {

	int N, S1, S2, S3;
	size_t i, j;
	int packages[1024][2];
	const double pi = acos(-1);

	double angles[1024];
	double max_angle, tmp_angle;
	while (scanf("%d", &N) && N != 0) {
		for (i = 0; i < N; i++) {
			scanf("%d %d %d", &S1, &S2, &S3);
			// tan(phi) = y/x
			angles[i] = atan2(S2-S1, S3-S1);
		}
		qsort(angles, N, sizeof(angles[0]), comparator);

//		for (i=0;i<N;i++){
//			printf("%f ", angles[i]);
//		}
//		printf("\n");

		max_angle = 0;
		for (i = 1; i < N; i++) {
			tmp_angle = angles[i] - angles[i-1];
			max_angle = max_angle >= tmp_angle ? max_angle : tmp_angle;
		}
		tmp_angle = 2*pi - (angles[N-1] - angles[0]);
		max_angle = max_angle >= tmp_angle ? max_angle : tmp_angle;
		if (max_angle > pi) {
			printf("No\n");
		} else {
			printf("Yes\n");
		}

	}
	return 0;
}