#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int** graph;
    int n;
} DoubleTree;

DoubleTree* createDoubleTree(int** graph_, int n_) {
    DoubleTree* dt = (DoubleTree*)malloc(sizeof(DoubleTree));
    dt->graph = graph_;
    dt->n = n_;
    return dt;
}

int** AlgPrima(DoubleTree* dt) {
    int* U = (int*)malloc(dt->n * sizeof(int));
    U[0] = 0;
    int U_size = 1;

    int** ost = (int**)malloc(dt->n * sizeof(int*));
    for (int i = 0; i < dt->n; i++) {
        ost[i] = (int*)calloc(dt->n, sizeof(int));
    }

    while (U_size != dt->n) {
        int min = INT_MAX;
        int imin1 = -1, imin2 = -1;

        for (int i = 0; i < U_size; i++) {
            int j = U[i];
            for (int k = 0; k < dt->n; k++) {
                int found = 0;
                for (int l = 0; l < U_size; l++) {
                    if (U[l] == k) {
                        found = 1;
                        break;
                    }
                }
                if (!found && dt->graph[j][k] != 0) {
                    if (dt->graph[j][k] < min) {
                        min = dt->graph[j][k];
                        imin1 = j;
                        imin2 = k;
                    }
                }
            }
        }

        if (imin1 != -1 && imin2 != -1) {
            ost[imin1][imin2] = min;
            U[U_size++] = imin2;
        }
    }

    free(U);
    return ost;
}

void print(int** ost, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", ost[i][j]);
        }
        printf("\n");
    }
}

int** DoublePath(DoubleTree* dt) {
    int** ost = AlgPrima(dt);

    for (int i = 0; i < dt->n; i++) {
        for (int j = 0; j < dt->n; j++) {
            if (ost[i][j] != 0) ost[j][i] = ost[i][j];
        }
    }

    return ost;
}

int main() {
    int** graph = (int**)malloc(8 * sizeof(int*));
    for (int i = 0; i < 8; i++) {
        graph[i] = (int*)malloc(8 * sizeof(int));
    }

    int data[8][8] = {
        {0, 15, 11, 11, 18, 27, 18, 21},
        {15, 0, 7, 14, 10, 14, 16, 20},
        {11, 7, 0, 7, 7, 10, 10, 11},
        {11, 14, 7, 0, 10, 20, 7, 11},
        {18, 10, 7, 10, 0, 10, 7, 5},
        {27, 14, 10, 20, 10, 0, 16, 11},
        {18, 16, 10, 7, 7, 16, 0, 10},
        {21, 20, 11, 11, 5, 11, 10, 0}
    };

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            graph[i][j] = data[i][j];
        }
    }

    DoubleTree* dt = createDoubleTree(graph, 8);
    int** ost = DoublePath(dt);
    print(ost, dt->n);

    for (int i = 0; i < dt->n; i++) {
        free(ost[i]);
    }
    free(ost);
    free(dt);
    for (int i = 0; i < 8; i++) {
        free(graph[i]);
    }
    free(graph);

    return 0;
}