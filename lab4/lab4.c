#include <stdio.h>

// Retea de dimensiune 2^k * 2^k
// Se citeste o permutare

typedef struct {
	int in;
	int out;
} pair;

//shiftare la dreapta, ultimul bit devine primul bit
int invers_shuffle(int val, int k) {
	//salvez bitul care se pierde prin shiftare la dreapta
	char bit = 1 & val; 

	//shiftare la dreapta si sau cu valoarea ultimului bit pe prima pozitie
	return (bit << (k - 1)) | (val >> 1);
}

int main() {
	int k; //nr biti

	printf("k = \n");
	scanf("%d", &k);
	int N = 1 << k;

/*
	for (int i = 0; i < ; i++) {
		
	}*/

}