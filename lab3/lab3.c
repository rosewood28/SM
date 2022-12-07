#include <stdio.h>

// Retea de dimensiune 2^k * 2^k
// Se citesc m prerechi (sursa destinatie) 

int shuffle(int i, int N) {
	return (2 * i + 2 * i / N) % N;
}

int main() {
	int k, m, s, d;

	printf("k m \n");
	scanf("%d %d", &k, &m);
	int N = 1 << k;

	printf("s d\n");
	for (int i = 0; i < m; i++) {
		scanf("%d %d", &s, &d);

		int conect, bloc;
		for (int j = k - 1; j >= 0; j--) {
			//5 = 101 => se iese in aceasta ordine: 1 - jos , 0 - sus, 1 - jos
			int bitd = (d & (1 << j)) >> j;

			if ((s < N / 2 && bitd == 1) || (s >= N / 2 && bitd == 0)) {
				//intram sus iesim jos sau intram jos iesim sus
				printf("init: %d\n", s);
				s = shuffle(s, N);
				bloc = s / 2;
				printf("%d, bloc %d, %s\n", s, bloc, "conexiune inversa");

				if (s % 2 == 0) {
					s = s + 1;
				} else {
					s = s - 1;
				}

			} else {
				printf("init: %d\n", s);
				s = shuffle(s, N);
				bloc = s / 2;
				printf("%d, bloc %d, %s\n", s, bloc, "conexiune directa");
			}

		}
	}

}