#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int in;
    int out;
} pair;

int inverse_shuffle(int val, int k) {
    char low_bit = 1 & val;

    return (low_bit << (k - 1)) | (val >> 1);
}

int not_first_bit(int val, int k) {
    return (1 << (k - 1)) ^ val;
}

int find_index_of_value(const int *input, int n, int needle) {
    for (int i = 0; i < n; i++) {
        if (input[i] == needle)
            return i;
    }

    return -1;
}

pair find_perm(pair *input, int n, int needle, int top) {
    for (int i = 0; i < n; i++) {
        if ((input[i].in == needle && top) || (input[i].out == needle && !top)) {
            return input[i];
        }
    }

    return input[0];
}

pair *get_perm_order(pair *input_perm, int *left, int *right, int n) {
    pair *order = (pair *) malloc(sizeof(pair) * n);

    int *seen_left = (int *) calloc(n, sizeof(int));
    int *seen_right = (int *) calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            order[i].in = input_perm[left[i]].in;
            order[i].out = input_perm[left[i]].out;

            seen_left[order[i].in] = 1;
            seen_right[order[i].out] = 1;
        } else if (i % 2 == 1) {
            int last = find_index_of_value(right, n, order[i - 1].out);
            int next = last % 2 ? right[last - 1] : right[last + 1];
            int k = 0;
            while (seen_right[next]) {
                next = right[k];
                k++;
            }

            pair found = find_perm(input_perm, n, next, 0);

            order[i].in = found.in;
            order[i].out = found.out;
            seen_left[order[i].in] = 1;
            seen_right[order[i].out] = 1;
        } else {
            int last = find_index_of_value(left, n, order[i - 1].in);
            int next = last % 2 ? left[last - 1] : left[last + 1];
            int k = 0;
            while (seen_left[next]) {
                next = left[k];
                k++;
            }

            pair found = find_perm(input_perm, n, next, 1);

            order[i].in = found.in;
            order[i].out = found.out;
            seen_left[order[i].in] = 1;
            seen_right[order[i].out] = 1;
        }
    }

    return order;
}

pair *create_perm(const int *input, int n) {
    pair *order = (pair *) malloc(sizeof(pair) * n);

    for (int i = 0; i < n; i++) {
        order[i].in = i;
        order[i].out = input[i];
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

int main() {
    int k;
    printf("k=");
    scanf("%d", &k);
    int n = 1 << k;

    int *input = (int *) malloc(sizeof(int) * n);
    int *left_side = (int *) malloc(sizeof(int) * n);
    int *right_side = (int *) malloc(sizeof(int) * n);

    printf("pair=");
    for (int i = 0; i < n; i++) {
        scanf("%d", &input[i]);
        left_side[i] = right_side[i] = i;
    }

    pair *input_perm = create_perm(input, n);

    int **etaje = (int **) malloc(sizeof(int *) * (2 * k - 1));

    for (int j = 0; j < k - 1; j++) {
        pair *order = get_perm_order(input_perm, left_side, right_side, n);

        int etaj = (2 * k - 1) - j - 1;

        etaje[etaj] = (int *) calloc(n / 2, sizeof(int));
        for (int i = 0; i < n; i += 2) {
            int in = order[i].in;
            int index = find_index_of_value(left_side, n, in);
            etaje[etaj][index / 2] = index % 2 ? 1 : 0;
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

                aux[sh + start] = left_side[i + start];
            }
        }

        free(left_side);
        left_side = aux;

        etaj = j;

        etaje[etaj] = (int *) calloc(n / 2, sizeof(int));
        for (int i = 1; i < n; i += 2) {
            int out = order[i].out;
            int index = find_index_of_value(right_side, n, out);
            etaje[etaj][index / 2] = index % 2 ? 0 : 1;
        }

        aux = (int *) calloc(n, sizeof(int));
        for (int t = 0; t < repeats; t++) {
            int start = t * (n / repeats);
            for (int i = 0; i < (n / repeats); i++) {
                int sh = inverse_shuffle(i, k - j);
                if (etaje[etaj][(i + start) / 2] == 1) {
                    sh = not_first_bit(sh, k - j);
                }

                aux[sh + start] = right_side[i + start];
            }
        }
        free(right_side);
        right_side = aux;

    }

    int etaj = ((2 * k) - 1) / 2;
    etaje[etaj] = (int *) calloc(n / 2, sizeof(int));
    for (int i = 0; i < n; i += 2) {
        pair found = find_perm(input_perm, n, left_side[i], 1);
        if (right_side[i] == found.out)
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
