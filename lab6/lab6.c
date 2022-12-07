#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define min(a, b) ((a) < (b) ? (a) : (b))

int n_elems;

void print_vector(int* v) {
    for (int i = 0; i < n_elems; i++) {
        printf("%d", v[i]);
        if (i != n_elems - 1) {
            printf(" ");
        } else {
            printf("\n");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Specificati dimensiunea array-ului\n");
        exit(-1);
    }

    n_elems = atoi(argv[1]);
    int* v1 = malloc(n_elems * sizeof(int));
    int* v2 = malloc(n_elems * sizeof(int));

    v1 = malloc(n_elems * sizeof(int));
    for (int i = 0; i < n_elems; i++) {
        v1[i] = i;
        v2[i] = 3 * i;
    }

    print_vector(v1);
    print_vector(v2);
    
    #pragma omp parallel for
    for(int i = 0; i < n_elems; ++i) {
        v1[i] += v2[i];
    }

    print_vector(v1);

    free(v1);
    free(v2);
}
