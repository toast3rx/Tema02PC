#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

#define ALLOC 'L'
#define DIM 'D'
#define PRINT 'P'
#define STOP 'Q'

void add_matrix(int ***list, int *list_length, int *list_size, int **dim_list, int *dim_length, int *dim_size);
int ***initialize_3D_array(int initial_size);
int **alloc_matrix(int n, int m);
void read_matrix(int **matrix, int n, int m);
void resize_3D_array(int ****list, int *size);
void resize_2D_array(int ***list, int *size);
void print_dimensions(int **list, int list_length);
void print_array(int ***list, int **dimensions_array, int current_length);

int main(void)
{

	// 2D Array that stores the dimensions for every matrix read from stdin
	int dimensions_length = 0;
	int dimensions_size = SIZE;
	int **dimensions_list = alloc_matrix(dimensions_size, 2);

	int length = 0;
	int size = SIZE;
	int ***list = initialize_3D_array(size);

	char command;

	do {
		scanf("%c", &command);
		switch (command)
		{
		case ALLOC:
			add_matrix(list, &length, &size, dimensions_list, &dimensions_length, &dimensions_size);
			break;
		case DIM:
			print_dimensions(dimensions_list, dimensions_length);
			break;
		case PRINT:
			print_array(list, dimensions_list, dimensions_length);
			break;
		default:
			printf("Unknown command!\n");
			break;
		}
		getchar();

	} while (command != STOP);

	free(list);
	free(dimensions_list);
	return 0;
}

/** Print array for a given index */
void print_array(int ***list, int **dimensions_array, int current_length)
{
	int index;
	scanf("%d", &index);

	if (index >= current_length || index < 0)
	{
		printf("No matrix with the given index\n");
		return;
	}

	for (int i = 0; i < dimensions_array[index][0]; i++)
	{
		for (int j = 0; j < dimensions_array[index][1]; j++)
			printf("%d ", list[index][i][j]);
		printf("\n");
	}
}

/** Print the number of rows and columns for a matrix at a given index */
void print_dimensions(int **list, int list_length)
{
	int index;
	scanf("%d", &index);

	if (index >= list_length || index < 0)
	{
		printf("No matrix with the given index\n");
		printf("Length: %d\n", list_length);
		return;
	}

	printf("%d %d\n", list[index][0], list[index][1]);
}

/** Add a new matrix to the 3D array */
void add_matrix(int ***list, int *list_length, int *list_size, int **dim_list, int *dim_length, int *dim_size)
{
	if (*list_length == *list_size) {
		resize_3D_array(&list, list_size);
		resize_2D_array(&dim_list, dim_size);
	}

	int rows, cols;
	scanf("%d%d", &rows, &cols);

	int **new_matrix = alloc_matrix(rows, cols);

	read_matrix(new_matrix, rows, cols);

	list[*list_length] = new_matrix;

	*list_length = *list_length + 1;

	//Add 2D coords to the dimensions array
	dim_list[*dim_length][0] = rows;
	dim_list[*dim_length][1] = cols;
	printf("TEST%d\n", dim_list[*dim_length][1]);
	*dim_length = *dim_length + 1;
}

// First initialization of a 3D array
int ***initialize_3D_array(int initial_size)
{
	int ***list = (int ***) malloc(initial_size * sizeof(int **));
	if (!list)
		return NULL;
	return list;
}

// Double the size of a 3D array
void resize_3D_array(int ****list, int *size)
{
	*size += SIZE;
	*list = (int ***)realloc(*list, *size * sizeof(int **));

}

// Double the size of a 2D Array
void resize_2D_array(int ***list, int *size)
{
	*size += SIZE;
	*list = (int **)realloc(*list, *size * sizeof(int *));
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
		if (!(matrix + i))
			return NULL;
	}

	return matrix;
}

// Read from stdin an array with n rows and m columns2
void read_matrix(int **matrix, int n, int m)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			scanf("%d", (*(matrix + i) + j));
}