#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

#define ALLOC 'L'
#define STOP 'Q'

void add_matrix(int ****list, int *list_length, int *list_size);
int ***initialize_list(int initial_size);
int **alloc_matrix(int n, int m);
void read_matrix(int **matrix, int n, int m);
void resize_list(int ****list, int *size);

int main(void)
{

    int length = 0;
    int size = SIZE;
    int ***list = initialize_list(size);

    char command;

    do {
        scanf("%c", &command);
        switch (command)
        {
        case 'L':
            add_matrix(&list, &length, &size);
            break;
        }
    } while (command != STOP);

    //free(list);
    return 0;
}

void add_matrix(int ****list, int *list_length, int *list_size)
{
    if (list_length == list_size)
        resize_list(list, list_size);

    int rows, cols;
    scanf("%d%d", &rows, &cols);

    int **new_matrix = (int **)alloc_matrix(rows, cols);
    read_matrix(new_matrix, rows, cols);

    *list[*list_length++] = new_matrix;
}

// First initialization of list
int ***initialize_list(int initial_size)
{
    int ***list = (int ***)malloc(initial_size * sizeof(int **));
    if (!list)
        return NULL;
    return list;
}

// Double the size of the list
void resize_list(int ****list, int *size)
{
    *size += SIZE;
    *list = (int ***)realloc(*list, *size);
}

// Dynamically alloc matrix memory
int **alloc_matrix(int n, int m)
{

    int **matrix = (int **)malloc(n * sizeof(int *));
    if (!matrix)
        return NULL;

    for (int i = 0; i < n; i++)
    {
        matrix[i] = (int *)malloc(m * sizeof(int));
        if (!matrix[i])
            return NULL;
    }

    return matrix;
}

// Read from stdin an array with n rows and m columns2
void read_matrix(int **matrix, int n, int m)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &matrix[i][j]);
}