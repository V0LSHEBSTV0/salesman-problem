
#include "graph_rand.h"

point_t get_random_point(int x_max, int y_max)
{
    /* generates random point with coordinates between 0 and x/y_max */
    int x = rand() % x_max;
    int y = rand() % y_max;
    point_t p = {x, y};
    return p;
}

weight distance_between_points(point_t a, point_t b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

graph_t* get_random_euclidean_graph(int n)
{
    point_t* vertices = (point_t*)malloc(n * sizeof(point_t));
    for (int i = 0; i < n; i++)
    {
        vertices[i] = get_random_point(100, 100);
        // printf("%f, %f", vertices[i].x, vertices[i].y);
    }

    weight** G = (weight**)malloc(n * sizeof(weight*));
    for (int i = 0; i < n; i++)
        G[i] = (weight*)malloc(n * sizeof(weight));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            G[i][j] = distance_between_points(vertices[i], vertices[j]);

    graph_t* graph = (graph_t*)malloc(sizeof(graph_t));
    graph->N = n;
    graph->G = G;

    free(vertices);
    
    return graph;
}