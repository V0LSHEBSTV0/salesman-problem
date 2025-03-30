#pragma once

#include "graph.h"
#include "graph_rand.h"
#include <time.h>
#include <stdio.h>

void limit_of_runtime(double_t (*func)(graph_t*));
runtime_test_result time_of_c_algs_runtime(double_t (*func)(graph_t*), graph_t* g);

runtime_test_result_ccp time_of_cpp_algs_runtime(double_t (*func)(weight**, int* n), graph_t* g);

void tests(int v_min, int v_max, int v_max_bf, int n_tests, FILE* fp);