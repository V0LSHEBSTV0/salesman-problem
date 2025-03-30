#pragma once

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "DoubleTree.h"

#define INF 1e9
#define DOT 46 
#define EPS 1e-4


typedef float weight;

typedef struct graph_t {
    int N;
    weight** G;
} graph_t; 

typedef struct runtime_test_result {
    double_t sum;
    double_t delta;
} runtime_test_result;

typedef struct runtime_test_result_ccp {
    double_t sum;
    double_t delta;
} runtime_test_result_ccp;

void print_graph(weight** arr, int rows, int cols);

bool is_euclidean_graph(graph_t* g);

double_t sum_of_cycle(graph_t* g, int* vertices);


// TSP algorithms
double_t naive_brute_force(graph_t* g);
double_t eager_tsp(graph_t* g);


void eager_tsp_path(graph_t* g, int* path);
void naive_brute_force_path(graph_t* g, int* path);



void print_cycle(int* path, int n);