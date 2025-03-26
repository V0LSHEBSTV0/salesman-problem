#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct Graph {
    int N;
    int** G;
} Graph;


int get_num(FILE *fp){
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
    } while(EOF != (ch = fgetc(fp)) && isdigit(ch));

    word[i] = '\0';
    int num;         
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
    int** arr; 
    arr = (int**)malloc(vertex * sizeof(int*));
    for (int i = 0; i < vertex; i++)
        arr[i] = (int*)malloc(vertex * sizeof(int));

    // read array
    arr = (int**)malloc(vertex * sizeof(int*));
    for (int i = 0; i < vertex; i++)
        arr[i] = (int*)malloc(vertex * sizeof(int));

    // read array
    for (int i = 0; i < vertex; i++) {
        for (int j = 0; j < vertex; j++) {
            arr[i][j] = get_num(fptr); 
        }
    }

    fclose(fptr);

    return &(Graph){vertex, arr};
}

void print_graph(int **arr, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}


int main() {
    
    char input_file[] = "input.txt";
    
    Graph* g = read_from_file(input_file);
    
    print_graph(g->G, g->N, g->N);

    return 0;
}