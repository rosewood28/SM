#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

int n = 3;

void print_matrix(int **m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d ", m[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void printCREW_matrix(int ***m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d ", m[i][j][0]);
		}
		printf("\n");
	}
	printf("\n");
	
}

int** allocate_matrix() {
	int **m = malloc(n * sizeof(int));
	for (int i = 0; i < n; i++) {
		m[i] = malloc(n * sizeof(int));
	}
	return m;
}

int*** allocate3D_matrix() {
	int ***m = malloc(n * sizeof(int));
	for (int i = 0; i < n; i++) {
		m[i] = malloc(n * sizeof(int));
		for(int j = 0; j < n; j++) {
			m[i][j] = malloc(n * sizeof(int));
		}
	}
	return m;
}

void free_matrix(int **m) {
	for (int i = 0; i < n; i++) {
			free(m[i]);
	}
	free(m);
}

void free3D_matrix(int ***m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			free(m[i][j]);
		}
		free(m[i]);
	}
	free(m);
}

int** create_matrix() {
	int **m = allocate_matrix(n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			m[i][j] = rand() % 8;
		}
	}

	return m;
}


//inmultire secventiala matrice 
int** secv(int **a, int **b) {
	int **c = allocate_matrix(n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			c[i][j] = 0;
			for (int k = 0; k < n; k++) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	return c;
}

int** crcw(int **a, int**b) {
	int **c = allocate_matrix(n);

	#pragma omp parallel for collapse(2)
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			c[i][j] = 0;
		}
	}

	omp_lock_t writelock;
	omp_init_lock(&writelock);

	#pragma omp parallel for collapse(3)
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
				//printf("%d:\n", omp_get_thread_num());
				omp_set_lock(&writelock);
				c[i][j] += a[i][k] * b[k][j];
				omp_unset_lock(&writelock);
			}
		}
	}

	omp_destroy_lock(&writelock);

	return c;
}

int*** crew(int **a, int**b) {
	int ***v = allocate3D_matrix();

	#pragma omp parallel for collapse(3)
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
				v[i][j][k] = a[i][k] * b[k][j];
			}
		}
	}

	#pragma omp barrier

	#pragma omp parallel for collapse(3)
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
				for (int m = 0; m < log2(n); m++) {
					int p = pow(2, m + 1);
					if (k % p == 0) {
						int q = pow(2, m);
						v[i][j][k] += v[i][j][k + q];
					}
				}
			}
		}
	}

	return v;
}

int main() {
	srand(42);

	//matrices
	int **a, **b, **c, **s;
	a = create_matrix();
	b = create_matrix();
	print_matrix(a);
	print_matrix(b);

	int ***v;

	//compute multiplication
	omp_set_num_threads(n*n*n);

	c = crcw(a, b);
	printf("CRCW-PRAM:\n");
	print_matrix(c);

	v = crew(a, b);
	printf("CREW-PRAM:\n");
	printCREW_matrix(v);

	s = secv(a, b);
	printf("Secvential result:\n");
	print_matrix(s);

	free_matrix(a);
	free_matrix(b);
	free_matrix(c);
	free_matrix(s);
	free3D_matrix(v);
}