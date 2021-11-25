#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

#define ALLOC 'L'
#define DIM 'D'
#define PRINT 'P'
#define MULTIPLY 'M'
#define STOP 'Q'

// TODO cons./

void add_matrix(int ***list, int *list_length, int *list_size, int **dim_list);
int ***initialize_3D_array(int initial_size);
int **alloc_matrix(int n, int m);
void read_matrix(int **matrix, int n, int m);
void resize_matrix_list(int ****matrix_list, int ***dim_list, int *size);
void print_dimensions(int **list, int list_length);
void print_array(int ***list, int **dimensions_array, int current_length);
void print_out_of_bounds_error();
void multiply_matrix(int ***matrix_list, int *matrix_list_len, int *matrix_list_size, int **dim_matrix);

int main(void)
{
	int length = 0;
	int size = SIZE;
	int ***list = initialize_3D_array(size);
	// 2D Array that stores the dimensions for every matrix read from stdin
	int **dimensions_list = alloc_matrix(size, 2);

	char command;

	do {
		scanf("%c", &command);
		switch (command)
		{
		case ALLOC:
			add_matrix(list, &length, &size, dimensions_list);
			break;
		case DIM:
			print_dimensions(dimensions_list, length);
			break;
		case PRINT:
			print_array(list, dimensions_list, length);
			break;
		case MULTIPLY:
			multiply_matrix(list, &length, &size, dimensions_list);
			break;
		case STOP:
				continue;

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
		print_out_of_bounds_error();
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
		print_out_of_bounds_error();
		return;
	}

	printf("%d %d\n", list[index][0], list[index][1]);
}

/** Add a new matrix to the 3D array */
void add_matrix(int ***list, int *list_length, int *list_size, int **dim_list)
{
	if (*list_length == *list_size) {
		resize_matrix_list(&list, &dim_list, list_size);
	}

	int rows, cols;
	scanf("%d%d", &rows, &cols);

	int **new_matrix = alloc_matrix(rows, cols);
	read_matrix(new_matrix, rows, cols);

	list[*list_length] = new_matrix;
	//Add 2D coords to the dimensions array
	dim_list[*list_length][0] = rows;
	dim_list[*list_length][1] = cols;

	*list_length = *list_length + 1;
}

// First initialization of a 3D array
int ***initialize_3D_array(int initial_size)
{
	int ***list = (int ***) malloc(initial_size * sizeof(int **));
	if (!list)
		return NULL;
	return list;
}

/** Double the size of a 3D array
 * @****matrix_list - pointer to the list of 2D arrays
 * @***dim_list - pointer to the list of dimensions array
 * @*size - pointer to list max size
 */
void resize_matrix_list(int ****matrix_list, int ***dim_list, int *size)
{
	*size += SIZE;
	*matrix_list = (int ***)realloc(*matrix_list, *size * sizeof(int **));
	// Realloc memory for dimensions array
	*dim_list = (int **)realloc(*dim_list, *size * sizeof(int *));
}

/** Dynamically alloc matrix memory
 *  @n - number of rows
 * 	@m - number of columns
 */
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

/** Read from stdin an array with n rows and m columns2
 *  @matrix - 2D Array where the values will be stored
 *  @n - number of rows
 * 	@m - number of columns
 */ 
void read_matrix(int **matrix, int n, int m)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			scanf("%d", (*(matrix + i) + j));
}


void multiply_matrix(int ***matrix_list, int *matrix_list_len, int *matrix_list_size, int **dim_matrix) {
	int index_first_matrix;
	int index_second_matrix;

	scanf("%d%d", &index_first_matrix, &index_second_matrix);

	if(index_first_matrix >= *matrix_list_len || index_first_matrix < -1 || 
		index_second_matrix >= *matrix_list_len || index_second_matrix < -1) {
			print_out_of_bounds_error();
			return;
		}

	int first_rows = dim_matrix[index_first_matrix][0];
	int first_cols = dim_matrix[index_first_matrix][1];
	
	int second_rows = dim_matrix[index_second_matrix][0];
	int second_cols = dim_matrix[index_second_matrix][1];

	if(first_cols != second_rows) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}

	if(*matrix_list_len == *matrix_list_size) 
		resize_matrix_list(&matrix_list, &dim_matrix, matrix_list_size);

	int **result = alloc_matrix(first_rows, second_cols);
	
	for(int i = 0; i < first_rows; i++)
		for(int j = 0; j < second_cols; j++)
			for(int k = 0; k < second_rows; k++)
				result[i][j] += matrix_list[index_first_matrix][i][k] * matrix_list[index_second_matrix][k][j];
	
	matrix_list[*matrix_list_len] = result;
	dim_matrix[*matrix_list_len][0] = first_rows;
	dim_matrix[*matrix_list_len][1] = second_cols;
	*matrix_list_len = *matrix_list_len + 1;
	
}

/** Print an error if given index doesn't correspond to any array */
void print_out_of_bounds_error() {
	printf("No matrix with the given index\n");
}