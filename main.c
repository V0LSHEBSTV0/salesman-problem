#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/graph.h"
#include "include/graph_rand.h"
#include "include/graph_tests.h"

#include <time.h>
#include <math.h>

weight get_num(FILE *fp) {
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
    sscanf(strdup(word), "%f", &num);

    return num;
}

graph_t* read_from_file(char* file)
{
    FILE* fptr = fopen(file, "r");

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

    graph_t* graph = (graph_t*)malloc(sizeof(graph_t));
    graph->N = vertex;
    graph->G = arr;

    return graph;
}


int main() {
    srand(time(NULL));

    char* output_file = "./analysis/output.csv";
    FILE* fp = fopen(output_file, "w");

    tests(3, 500, 13, 5, fp);
    
    fclose(fp);

    
    // char input_file[] = "input.txt";

    // graph_t* g_input = read_from_file(input_file);

    // graph_t* g = get_random_euclidean_graph(10)

    // print_graph(g->G, g->N, g->N);

    // if (!is_euclidean_graph(g))
    // {
    //     printf("Граф не евклидов\n");
    //     return -1;
    // }
    // else 
    //     printf("Граф евклидов\n");
        

    // int* path_naive = naive_brute_force(g);
    // weight dist_naive = sum_of_cycle(g, path_naive);
    // printf("Полный перебор.\n");
    // printf("Минимальная дистанция: %f\n", dist_naive);
    // printf("Путь: ");
    // print_cycle(path_naive, g->N);
    // printf("\n\n");

    // int* path_eager = eager_tsp(g);
    // weight dist_eager = sum_of_cycle(g, path_eager);
    // printf("Жадный алгоритм.\n");
    // printf("Минимальная дистанция: %f\n", dist_eager);
    // printf("Путь: ");
    // print_cycle(path_eager, g->N);
    // printf("\n\n");

    
    // int* path_dabltree = getCycleDoubleTree(g->G, &g->N);
    // printf("Алгоритм даблтрии.\nПуть: ");
    // print_cycle(path_dabltree, g->N);


    // // clean up
    // free(path_eager);
    // free(path_naive);

    // free()
    return 0;
}
