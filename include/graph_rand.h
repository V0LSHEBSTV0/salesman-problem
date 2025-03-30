#pragma once

#include "graph.h"

typedef struct point_t {
    weight x;
    weight y;
} point_t; 

point_t get_random_point(int x_max, int y_max);

weight distance_between_points(point_t a, point_t b);

graph_t* get_random_euclidean_graph(int n);