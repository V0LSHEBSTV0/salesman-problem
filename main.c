#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


#define INF 1e9
#define DOT 46 

typedef int weight;

typedef struct Graph {
    int N;
    weight** G;
} Graph;

weight get_num(FILE *fp){
    /*
        Returns NUll, no more digits in file
    */
    char word[100];
    int ch, i = 0;

    while (EOF != (ch = fgetc(fp)) && !isdigit(ch)) // Skip to first digit
        ;
    if(ch == EOF)
        return 0;
    do {
        word[i++] = ch;
    } while(EOF != (ch = fgetc(fp)) && (isdigit(ch) || ch == DOT));

    word[i] = '\0';

    weight num;         
    sscanf(strdup(word), "%d", &num);

    return num;
}

Graph* read_from_file(char* file)
{
    FILE* fptr = fopen(file, "r");
    // if(!fptr) {
    //     printf("Ошибка при открытии файла\n");
    //     return NULL;
    // }

    // read n and convert to int
    int vertex = get_num(fptr);
    
    // init array 
    weight** arr = (weight**)malloc(vertex * sizeof(weight*));
    for (int i = 0; i < vertex; i++)
        arr[i] = (weight*)malloc(vertex * sizeof(weight));

    // read array
    arr = (weight**)malloc(vertex * sizeof(weight*));
    for (int i = 0; i < vertex; i++)
        arr[i] = (weight*)malloc(vertex * sizeof(weight));

    // read array
    for (int i = 0; i < vertex; i++) {
        for (int j = 0; j < vertex; j++) {
            arr[i][j] = get_num(fptr); 
        }
    }

    fclose(fptr);

    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->N = vertex;
    graph->G = arr;

    return graph;
}

void print_graph(weight** arr, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

bool is_euclidean_graph(Graph* g) {
    for (int i = 0; i < g->N; i++) {
        for (int j = 0; j < g->N; j++) {
            for (int k = 0; k < g->N; k++)
            {
                if (g->G[i][j] > g->G[i][k] + g->G[j][k])
                    return false;
            }
        }
    }
    return true;
}

weight sum_of_cycle(Graph* g, int* vertices)
{
    weight sum = 0;
    for(int i = 0; i < g->N - 1; i++)
        sum += g->G[vertices[i]][vertices[i+1]];
    sum += g->G[vertices[g->N-1]][vertices[0]];
    return sum;
}

int* naive_brute_force(Graph* g)
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

    free(stack);
    free(vertices);

    return min_vertices;
}

int* eager_tsp(Graph* g) {
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
        if (next == -1) {
            printf("No path found\n");
            free(visited);
            free(path);
            return NULL;
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
    return path;
}


void print_cycle(int* path, int n)
{
    for (int i = 0; i < n - 1; i++)
        printf("%d -> ", path[i]);
    printf("%d -> %d", path[n-1], path[0]);
}

int main() {
    
    char input_file[] = "input.txt";
    Graph* g = read_from_file(input_file);
    
    print_graph(g->G, g->N, g->N);
    // if (!is_euclidean_graph(g))
    // {
    //     printf("Граф не евклидов");
    //     return -1;
    // }

    int* path_naive = naive_brute_force(g);
    weight dist_naive = sum_of_cycle(g, path_naive);
    printf("Полный перебор.\n");
    printf("Минимальная дистанция: %d\n", dist_naive);
    printf("Путь: ");
    print_cycle(path_naive, g->N);
    printf("\n\n");


    int* path_eager = eager_tsp(g);
    weight dist_eager = sum_of_cycle(g, path_eager);
    printf("Жадный алгоритм.\n");
    printf("Минимальная дистанция: %d\n", dist_eager);
    printf("Путь: ");
    print_cycle(path_eager, g->N);
    printf("\n\n");


    // clean up
    free(path_eager);
    free(path_naive);
    return 0;
}