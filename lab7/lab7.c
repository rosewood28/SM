#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define min(a, b) ((a) < (b) ? (a) : (b))

int* v1;
int* v2;
int n_elems;
int n_threads;

void *add_function(void *arg) {
    long id = (long) arg;
    int start = id * (double) n_elems / n_threads;
    int end = min((id + 1) * (double) n_elems / n_threads, n_elems);
    for(int i = start; i < end; ++i) {
        v1[i] += v2[i];
    }
    pthread_exit(NULL);
}

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

    v1 = malloc(n_elems * sizeof(int));
    for (int i = 0; i < n_elems; i++) {
        v1[i] = i;
    }

    v2 = malloc(n_elems * sizeof(int));
    for (int i = 0; i < n_elems; i++) {
        v2[i] = 3 * i;
    }

    print_vector(v1);
    print_vector(v2);
    
    // crearea thread-urilor
    n_threads = sysconf(_SC_NPROCESSORS_CONF);
    long id;
    int r;
    pthread_t threads[n_threads];
    void *status;

    for (id = 0; id < n_threads; id++) {
		r = pthread_create(&threads[id], NULL, add_function, (void *)id);

		if (r) {
	  		printf("Eroare la crearea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}

  	for (id = 0; id < n_threads; id++) {
		r = pthread_join(threads[id], &status);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}

    print_vector(v1);

    free(v1);
    free(v2);
}
