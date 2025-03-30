#include "graph.h"
#include "math.h"

#include <stdio.h>

void print_graph(weight** arr, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%f ", arr[i][j]);
        }
        printf("\n");
    }
}

bool is_euclidean_graph(graph_t* g) {
    for (int i = 0; i < g->N; i++) {
        for (int j = 0; j < g->N; j++) {
            for (int k = 0; k < g->N; k++)
            {
                if (g->G[i][j] > g->G[i][k] + g->G[j][k])
                {
                    // printf("%d, %d, %d\n", i, j, k);
                    // printf("%d, %d, %d\n", g->G[i][j], g->G[i][k], g->G[j][k] );
                    return false;
                }
            }
        }
    }
    return true;
}

double_t sum_of_cycle(graph_t* g, int* vertices)
{
    double_t sum = 0;
    for(int i = 0; i < g->N - 1; i++)
        sum += g->G[vertices[i]][vertices[i+1]];
    sum += g->G[vertices[g->N-1]][vertices[0]];
    return sum;
}

double_t naive_brute_force(graph_t* g)
{
    int n = g->N;   
    int* vertices = (int*)malloc(g->N * sizeof(int));

    for (int i = 0; i < g->N; i++)
        vertices[i] = i;

    // Heap's algorithm
    // stack is an encoding of the stack state
    int temp_swap = 0;

    int* stack = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        stack[i] = 0;
    
    // values, that saves minimal path and weight 
    weight min_weight = sum_of_cycle(g, vertices);
    int* min_vertices = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        min_vertices[i] = vertices[i];

    weight new_sum = 0;
    
    int stack_pointer = 0;
    while (stack_pointer < n)
    {
        if (stack[stack_pointer] < stack_pointer)
        {
            if (stack_pointer%2 == 0)
            {
                temp_swap = vertices[0];
                vertices[0] = vertices[stack_pointer];
                vertices[stack_pointer] = temp_swap;
            }
            else
            {
                temp_swap = vertices[stack[stack_pointer]];
                vertices[stack[stack_pointer]] = vertices[stack_pointer];
                vertices[stack_pointer] = temp_swap;
            }

            // save new path and weight
            new_sum = sum_of_cycle(g, vertices);
            if (new_sum < min_weight)
            {
                min_weight = new_sum;
                for(int i = 0; i < n; i++)
                    min_vertices[i] = vertices[i];
            }

            stack[stack_pointer]++;
            stack_pointer = 0;
        }
        else
        {
            stack[stack_pointer] = 0;
            stack_pointer++;
        }
    }
    double_t res = sum_of_cycle(g, min_vertices);
    
    free(vertices);
    free(stack);
    free(min_vertices);
    
    return res;
}


double_t eager_tsp(graph_t* g) {
    int N = g->N;           // Number of vertices
    weight** G = g->G;      // Adjacency matrix

    // Initialize visited array and path
    bool* visited = (bool*)malloc(N * sizeof(bool));
    int* path = (int*)malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        visited[i] = false;
    }

    // Start from vertex 0
    int current = 0;
    visited[current] = true;
    path[0] = current;
    weight total_weight = 0;

    // Visit all vertices
    for (int step = 1; step < N; step++) { 
        int next = -1;
        weight min_weight = INF;  // INF is a large value (e.g., 1e9)
        // Find the nearest unvisited vertex
        for (int i = 0; i < N; i++) {
            if (!visited[i] && G[current][i] < min_weight) {
                min_weight = G[current][i];
                next = i;
            }
        }

        // Move to the nearest vertex
        path[step] = next;
        total_weight += min_weight;
        visited[next] = true;
        current = next;
    }

    // Return to the starting vertex
    total_weight += G[current][path[0]];

    // Clean up
    free(visited);
    double_t res = sum_of_cycle(g, path);
    free(path);
    return res;
}

// int* path is out param
void naive_brute_force_path(graph_t* g, int* path)
{
    int n = g->N;
    int* vertices = (int*)malloc(g->N * sizeof(int));

    for (int i = 0; i < g->N; i++)
        vertices[i] = i;

    // Heap's algorithm
    // stack is an encoding of the stack state
    int temp_swap = 0;

    int* stack = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        stack[i] = 0;
    
    // values, that saves minimal path and weight 
    weight min_weight = sum_of_cycle(g, vertices);
    
    int* min_vertices = path;

    for (int i = 0; i < n; i++)
        min_vertices[i] = vertices[i];

    weight new_sum = 0;
    
    int stack_pointer = 0;
    while (stack_pointer < n)
    {
        if (stack[stack_pointer] < stack_pointer)
        {
            if (stack_pointer%2 == 0)
            {
                temp_swap = vertices[0];
                vertices[0] = vertices[stack_pointer];
                vertices[stack_pointer] = temp_swap;
            }
            else
            {
                temp_swap = vertices[stack[stack_pointer]];
                vertices[stack[stack_pointer]] = vertices[stack_pointer];
                vertices[stack_pointer] = temp_swap;
            }

            // save new path and weight
            new_sum = sum_of_cycle(g, vertices);
            if (new_sum < min_weight)
            {
                min_weight = new_sum;
                for(int i = 0; i < n; i++)
                    min_vertices[i] = vertices[i];
            }

            stack[stack_pointer]++;
            stack_pointer = 0;
        }
        else
        {
            stack[stack_pointer] = 0;
            stack_pointer++;
        }
    }
    double_t res = sum_of_cycle(g, min_vertices);
    
    free(vertices);
    free(stack);
}


void eager_tsp_path(graph_t* g, int* path) 
{
    int N = g->N;           // Number of vertices
    weight** G = g->G;      // Adjacency matrix

    // Initialize visited array and path
    bool* visited = (bool*)malloc(N * sizeof(bool));

    for (int i = 0; i < N; i++) {
        visited[i] = false;
    }

    // Start from vertex 0
    int current = 0;
    visited[current] = true;
    path[0] = current;
    weight total_weight = 0;

    // Visit all vertices
    for (int step = 1; step < N; step++) {
        int next = -1;
        weight min_weight = INF;  // INF is a large value (e.g., 1e9)
        // Find the nearest unvisited vertex
        for (int i = 0; i < N; i++) {
            if (!visited[i] && G[current][i] < min_weight) {
                min_weight = G[current][i];
                next = i;
            }
        }

        // Move to the nearest vertex
        path[step] = next;
        total_weight += min_weight;
        visited[next] = true;
        current = next;
    }

    // Return to the starting vertex
    total_weight += G[current][path[0]];

    // Clean up
    free(visited);
}

void print_cycle(int* path, int n)
{
    for (int i = 0; i < n - 1; i++)
        printf("%d -> ", path[i]);
    printf("%d -> %d", path[n-1], path[0]);
}