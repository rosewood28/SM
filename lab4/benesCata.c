#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int st;
    int dr;
} perechi;

int negate_first_bit(int val, int k) {
    return (1 << (k - 1)) ^ val;
}

int find_index_of_value(const int *input, int n, int toFind) {
    for (int i = 0; i < n; i++) {
        if (input[i] == toFind)
            return i;
    }

    return -1;
}

perechi find_perm(perechi *input, int n, int toFind, int st) {
    for (int i = 0; i < n; i++) {
        if ((input[i].st == toFind && st) || (input[i].dr == toFind && !st)) {
            return input[i];
        }
    }

    return input[0];
}

perechi *ordinePerechi(perechi *input, int *intrari, int *iesiri, int n) {
    perechi *order = (perechi *) malloc(sizeof(perechi) * n);

    int *seen_left = (int *) calloc(n, sizeof(int));
    int *seen_right = (int *) calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            order[i].st = input[intrari[i]].st;
            order[i].dr = input[intrari[i]].dr;

            seen_left[order[i].st] = 1;
            seen_right[order[i].dr] = 1;
        } else if (i % 2 == 1) {
            int last = find_index_of_value(iesiri, n, order[i - 1].dr);
            int next = last % 2 ? iesiri[last - 1] : iesiri[last + 1];
            int k = 0;
            while (seen_right[next]) {
                next = iesiri[k];
                k++;
            }

            perechi found = find_perm(input, n, next, 0);

            order[i].st = found.st;
            order[i].dr = found.dr;
            seen_left[order[i].st] = 1;
            seen_right[order[i].dr] = 1;
        } else {
            int last = find_index_of_value(intrari, n, order[i - 1].st);
            int next = last % 2 ? intrari[last - 1] : intrari[last + 1];
            int k = 0;
            while (seen_left[next]) {
                next = intrari[k];
                k++;
            }

            perechi found = find_perm(input, n, next, 1);

            order[i].st = found.st;
            order[i].dr = found.dr;
            seen_left[order[i].st] = 1;
            seen_right[order[i].dr] = 1;
        }
    }

    return order;
}

perechi *initPerm(const int *input, int n) {
    perechi *order = (perechi *) malloc(sizeof(perechi) * n);

    for (int i = 0; i < n; i++) {
        order[i].st = i;
        order[i].dr = input[i];
    }

    return order;
}

void print_conex(int conex) {
    if (conex) {
        printf("Conexiune inversa\n");
        return;
    }
    printf("Conexiune directa\n");
}

int inverse_shuffle(int val, int k) {
    char low_bit = 1 & val;

    return (low_bit << (k - 1)) | (val >> 1);
}

int main() {
    printf("k=");
    int k = 3;
    scanf("%d", &k);
    int n = 1 << k;

    int *input = (int *) malloc(sizeof(int) * n);
    int *intrari = (int *) malloc(sizeof(int) * n);
    int *iesiri = (int *) malloc(sizeof(int) * n);

    printf("perechi=");
    for (int i = 0; i < n; i++) {
        scanf("%d", &input[i]);
        intrari[i] = iesiri[i] = i;
    }

    perechi *input_perm = initPerm(input, n);

    int **etaje = (int **) malloc(sizeof(int *) * (2 * k - 1));

    for (int j = 0; j < k - 1; j++) {
        perechi *ordine = ordinePerechi(input_perm, intrari, iesiri, n);

        int etaj = j;

        etaje[etaj] = (int *) calloc(n / 2, sizeof(int));
        for (int i = 1; i < n; i += 2) {
            int y = ordine[i].dr;
            int index = find_index_of_value(iesiri, n, y);
            etaje[etaj][index / 2] = index % 2 ? 0 : 1;
        }

        int *aux = (int *) calloc(n, sizeof(int));
        int repeats = 1 << j;
        for (int t = 0; t < repeats; t++) {
            int start = t * (n / repeats);
            for (int i = 0; i < (n / repeats); i++) {
                int sh = inverse_shuffle(i, k - j);
                if (etaje[etaj][(i + start) / 2] == 1) {
                    sh = not_first_bit(sh, k - j);
                }

                aux[sh + start] = iesiri[i + start];
            }
        }
        free(iesiri);
        iesiri = aux;

        etaj = (2 * k - 1) - j - 1;

        etaje[etaj] = (int *) calloc(n / 2, sizeof(int));
        for (int i = 0; i < n; i += 2) {
            int x = ordine[i].st;
            int index = find_index_of_value(intrari, n, x);
            etaje[etaj][index / 2] = index % 2 ? 1 : 0;
        }

        aux = (int *) calloc(n, sizeof(int));
        for (int t = 0; t < repeats; t++) {
            int start = t * (n / repeats);
            for (int i = 0; i < (n / repeats); i++) {
                int sh = inverse_shuffle(i, k - j);
                if (etaje[etaj][(i + start) / 2] == 1) {
                    sh = negate_first_bit(sh, k - j);
                }

                aux[sh + start] = intrari[i + start];
            }
        }

        free(intrari);
        intrari = aux;

    }

    int etaj = ((2 * k) - 1) / 2;
    etaje[etaj] = (int *) calloc(n / 2, sizeof(int));
    for (int i = 0; i < n; i += 2) {
        perechi found = find_perm(input_perm, n, intrari[i], 1);
        if (iesiri[i] == found.dr)
            etaje[etaj][i / 2] = 0;
        else
            etaje[etaj][i / 2] = 1;
    }

    for (int j = (2 * k) - 2; j >= 0 ; j--)
    {
        etaj = j;
        printf("Etaj %d\n", etaj);
        for (int i = 0; i < (n / 2); i++) {
            printf("Bloc %d:", i);
            print_conex(etaje[etaj][i]);
        }
        printf("\n");
    }
}
