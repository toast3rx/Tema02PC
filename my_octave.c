#include <stdio.h>
#include <stdlib.h>

#define SIZE 1

#define MOD 10007

#define ALLOC 'L'
#define DIM 'D'
#define PRINT 'P'
#define REDIM 'C'
#define MULTIPLY 'M'
#define TRANSPOSE 'T'
#define SORT 'O'
#define FREE 'F'
#define STRASSEN 'S'
#define STOP 'Q'

void add_matrix(int ****list,
				int *list_length,
				int *list_size,
				int ***dim_list);
int ***initialize_3D_array(int initial_size);
int **alloc_matrix(int n, int m);
void read_matrix(int **matrix, int n, int m);
void realloc_matrix_list(int ****matrix_list,
						 int ***dim_list,
						 int *size,
						 int length,
						 int new_size);
void print_dimensions(int **list, int list_length);
void print_array(int ***list, int **dimensions_array, int current_length);
void print_out_of_bounds_error(void);
void multiply_matrix(int ****matrix_list,
					 int *matrix_list_len,
					 int *matrix_list_size,
					 int ***dim_matrix);
void add_transpose_matrix(int ***list, int length, int **dim);
void free_matrix(int **matrix, int rows);
int matrix_sum(int **matrix, int rows, int cols);
void sort_list(int ***list, int list_length, int **dim_list);
void swap_two_numbers(int *x, int *y);
void free_index_matrix(int ***list, int *length, int **dim_list);
void free_all(int ***list, int length, int size, int **dim);
void resize_matrix(int ****list, int *length, int ***dim_list);
int is_out_of_bounds(int index, int length);
void transpose_square_matrix(int **mat, int size);
int **strassen_multiplication(int **A, int **B, int n);
void strassen(int ****matrix_list, int *matrix_list_len, int *matrix_list_size, int ***dim_matrix);

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
		switch (command) {
		case ALLOC:
			add_matrix(&list, &length, &size, &dimensions_list);
			break;
		case DIM:
			print_dimensions(dimensions_list, length);
			break;
		case PRINT:
			print_array(list, dimensions_list, length);
			break;
		case REDIM:
			resize_matrix(&list, &length, &dimensions_list);
			break;
		case MULTIPLY:
			multiply_matrix(&list, &length, &size, &dimensions_list);
			break;
		case TRANSPOSE:
			add_transpose_matrix(list, length, dimensions_list);
			break;
		case SORT:
			sort_list(list, length, dimensions_list);
			break;
		case FREE:
			free_index_matrix(list, &length, dimensions_list);
			break;
		case STRASSEN:
			strassen(&list, &length, &size, &dimensions_list);
			break;
		case STOP:
			break;
		default:
			printf("Unrecognized command\n");
			break;
		}
		getchar();

	} while (command != STOP);

	free_all(list, length, size, dimensions_list);
	return 0;
}

/** Print array for a given index
 * @***list - list of 2D arrays
 * @**dimensions_array - array for arrays dimensions
 * @list_length - current length of list
*/
void print_array(int ***list, int **dimensions_array, int list_length)
{
	int index;
	scanf("%d", &index);

	if (is_out_of_bounds(index, list_length))
		return;

	for (int i = 0; i < dimensions_array[index][0]; i++) {
		for (int j = 0; j < dimensions_array[index][1]; j++)
			printf("%d ", list[index][i][j]);
		printf("\n");
	}
}

/** Print the number of rows and columns for a matrix at a given index
 * @**list - list of 2d Arrays
 * @list_length - current length of list
*/
void print_dimensions(int **list, int list_length)
{
	int index;
	scanf("%d", &index);

	if (is_out_of_bounds(index, list_length))
		return;
	printf("%d %d\n", list[index][0], list[index][1]);
}

/** Add a new matrix to the 3D array
 * @****list - pointer to the list of 2D arrays
 * @*list_length - pointer to current length of list
 * @*list_size - total memory allocated for list
 * @***dim_list - pointer to dimensions list
*/
void add_matrix(int ****list, int *list_length, int *list_size, int ***dim_list)
{
	if (*list_length == *list_size)
		realloc_matrix_list(list, dim_list,
							list_size, *list_length,
							*list_size * 2);

	int rows, cols;
	scanf("%d%d", &rows, &cols);

	int **new_matrix = alloc_matrix(rows, cols);
	read_matrix(new_matrix, rows, cols);

	(*list)[*list_length] = new_matrix;

	//Add 2D coords to the dimensions array
	(*dim_list)[*list_length][0] = rows;
	(*dim_list)[*list_length][1] = cols;

	*list_length = *list_length + 1;
}

/** Alloc memory in a 3D array for 2D arrays
 * @initial_size - initial number of 2D arrays to be stored
 */
int ***initialize_3D_array(int initial_size)
{
	int ***list = (int ***)calloc(initial_size, sizeof(int **));
	if (!list)
		return NULL;
	return list;
}

/** Double the size of a 3D array
 * @****matrix_list - pointer to the list of 2D arrays
 * @***dim_list - pointer to the list of dimensions array
 * @*size - pointer to list max size
 */
void realloc_matrix_list(int ****matrix_list,
						 int ***dim_list,
						 int *size, int length,
						 int new_size)
{
	*size = new_size;
	*matrix_list = (int ***)realloc(*matrix_list, *size * sizeof(int **));
	if (!matrix_list) {
		fprintf(stderr, "Error at memory reallocation\n");
		return;
	}
	// Realloc memory for dimensions array
	*dim_list = (int **)realloc(*dim_list, *size * sizeof(int *));
	if (!*dim_list) {
		printf("ERROR\n");
		return;
	}
	for (int i = length; i < *size; i++) {
		(*dim_list)[i] = (int *)calloc(2, sizeof(int));
		if (!(*dim_list)[i]) {
			fprintf(stderr, "Error at memory reallocation\n");
			return;
		}
	}
}

/** Dynamically alloc matrix memory
 *	@n - number of rows
 *	@m - number of columns
 */
int **alloc_matrix(int n, int m)
{
	int **matrix = (int **)calloc(n, sizeof(int *));
	if (!matrix)
		return NULL;

	for (int i = 0; i < n; i++) {
		matrix[i] = (int *)calloc(m, sizeof(int));
		if (!matrix[i])
			return NULL;
	}
	return matrix;
}

/** Read from stdin an array with n rows and m columns2
 *	@matrix - 2D Array where the values will be stored
 *	@n - number of rows
 *	@m - number of columns
 */
void read_matrix(int **matrix, int n, int m)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			scanf("%d", &matrix[i][j]);
}

/** Multiply 2 2D arrays from 2 given indexes
 * @****matrix_list - pointer to list of 2D arrays
 * @*matrix_list_len - pointer to length of the list
 * @*matrix_list_size - pointer to total size of the list
 * @***dim_matrix - pointer to 2D array that stores arrays dimensions
 */
void multiply_matrix(int ****matrix_list,
					 int *matrix_list_len,
					 int *matrix_list_size,
					 int ***dim_matrix)
{
	int index_first_matrix;
	int index_second_matrix;

	scanf("%d%d", &index_first_matrix, &index_second_matrix);

	if (is_out_of_bounds(index_first_matrix, *matrix_list_len) ||
		is_out_of_bounds(index_second_matrix, *matrix_list_len))
		return;

	int first_rows = (*dim_matrix)[index_first_matrix][0];
	int first_cols = (*dim_matrix)[index_first_matrix][1];

	int second_rows = (*dim_matrix)[index_second_matrix][0];
	int second_cols = (*dim_matrix)[index_second_matrix][1];

	if (first_cols != second_rows) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}

	if (*matrix_list_len == *matrix_list_size)
		realloc_matrix_list(matrix_list, dim_matrix,
							matrix_list_size,
							*matrix_list_len,
							*matrix_list_size * 2);

	int **result = alloc_matrix(first_rows, second_cols);
	if (!result) {
		printf("ERROR\n");
		return;
	}

	for (int i = 0; i < first_rows; i++) {
		for (int j = 0; j < second_cols; j++) {
			for (int k = 0; k < second_rows; k++)
				result[i][j] = ((result[i][j] % MOD) +
				(((*matrix_list)[index_first_matrix][i][k] % MOD) *
				(((*matrix_list)[index_second_matrix][k][j]) % MOD))) %
							   MOD;
			while (result[i][j] < 0)
				result[i][j] += MOD;
		}
	}

	(*matrix_list)[*matrix_list_len] = result;
	(*dim_matrix)[*matrix_list_len][0] = first_rows;
	(*dim_matrix)[*matrix_list_len][1] = second_cols;
	*matrix_list_len = *matrix_list_len + 1;
}

/** Replace a matrix at the given index with its transpose
 * @list - list of 2D arrays
 * @length - list length
 * @dim - list that stores arrays rows and columns number
 */
void add_transpose_matrix(int ***list, int length, int **dim)
{
	int index;
	scanf("%d", &index);

	if (index >= length || index < 0)
		print_out_of_bounds_error();

	int rows = dim[index][0];
	int cols = dim[index][1];

	dim[index][0] = cols;
	dim[index][1] = rows;

	if (rows != cols) {
		int max = rows > cols ? rows : cols;
		list[index] = (int **)realloc(list[index], max * sizeof(int *));

		if (cols > rows) {
			list[index] = realloc(list[index], cols * sizeof(int *));
			for (int i = rows; i < cols; i++)
				list[index][i] = (int *)calloc(max, sizeof(int));

			transpose_square_matrix(list[index], max);

			for (int i = 0; i < max; i++)
				list[index][i] = realloc(list[index][i], rows * sizeof(int));
		} else {
			for (int i = 0; i < rows; i++)
				list[index][i] = realloc(list[index][i], rows * sizeof(int));

			transpose_square_matrix(list[index], max);

			for (int i = cols; i < rows; i++)
				free(list[index][i]);

			list[index] = realloc(list[index], cols * sizeof(int *));
		}
	}
}

/** Transpose matrix in place
 * @**mat - 2D array to be tranposed
 * @size - number for rows/columns
 */
void transpose_square_matrix(int **mat, int size)
{
	for (int i = 0; i < size; ++i) {
		for (int j = i; j < size; ++j) {
			int temp = mat[i][j];
			mat[i][j] = mat[j][i];
			mat[j][i] = temp;
		}
	}
}

/** Free memory allocated for a 2D arrays at a given index
 * @***list - list of 2D arrays
 * @*length - pointer to list length
 * @**dim_list - array with dimensions for 2D arrays
 */
void free_index_matrix(int ***list, int *length, int **dim_list)
{
	int index;
	scanf("%d", &index);

	if (is_out_of_bounds(index, *length))
		return;

	// Delete 2D array from given index
	free_matrix(list[index], dim_list[index][0]);

	// Move to "left" the arrays from index to end
	for (int i = index; i < *length - 1; i++)
		list[i] = list[i + 1];
	// Remove last pointer from array
	list[*length - 1] = NULL;

	// Delete dimensions for the deleted array
	free(dim_list[index]);
	for (int i = index; i < *length - 1; i++)
		dim_list[i] = dim_list[i + 1];
	dim_list[*length - 1] = NULL;
	dim_list[*length - 1] = (int *)calloc(2, sizeof(int));

	*length = *length - 1;

	// BIG PROBLEM HERE
	// if(*length < *size / 2) {
	//	realloc_matrix_list(&list, &dim_list, size, *length, *size / 2);
	//	}
}

/** Free a matrix of memory and its rows
 * @matrix -- the matrix to be freed
 * @rows -- number of rows
 */
void free_matrix(int **matrix, int rows)
{
	for (int i = 0; i < rows; i++)
		free(matrix[i]);
	free(matrix);
}

/** Free all memory allocated
 * @***list - list of 2D arrays
 * @length - list length
 * @size - total memory allocated in list
 * @**dim_list - array with dimensions for 2D arrays
 */
void free_all(int ***list, int length, int size, int **dim)
{
	for (int i = 0; i < length; i++)
		free_matrix(list[i], dim[i][0]);
	free(list);
	free_matrix(dim, size);
}

/** Sort the matrices in the list based on the sum of the elements
 * @***list - list of 2D arrays
 * @list_length - length of the list
 *  @**dim_list - array with dimensions for 2D arrays
 */
void sort_list(int ***list, int list_length, int **dim_list)
{
	for (int i = 0; i < list_length - 1; i++) {
		for (int j = i + 1; j < list_length; j++) {
			int first_sum = matrix_sum(list[i],
									   dim_list[i][0],
									   dim_list[i][1]);
			int second_sum = matrix_sum(list[j],
										dim_list[j][0],
										dim_list[j][1]);
			if (first_sum > second_sum) {
				int **temp = list[i];
				list[i] = list[j];
				list[j] = temp;

				swap_two_numbers(&dim_list[i][0], &dim_list[j][0]);
				swap_two_numbers(&dim_list[i][1], &dim_list[j][1]);
			}
		}
	}
}

/** Swap the values of two numbers
 * @*x - pointer to first number
 * @*y 0 pointer to second number
 */
void swap_two_numbers(int *x, int *y)
{
	int temp = *x;
	*x = *y;
	*y = temp;
}

/** Return the sum of the elements in a matrix
 * @**matrix - 2D arrays
 * @rows - number of rows for the array
 * @cols - number of cols for the array
 * @return - sum of the elements
*/
int matrix_sum(int **matrix, int rows, int cols)
{
	int sum = 0;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			sum += matrix[i][j] % MOD;
	sum = sum % MOD;
	if (sum < 0)
		sum += MOD;

	return sum;
}

/** Resize a 2D arrays based on rows and columns
 *	read from stdin
 *	@****list - pointer to list of 2D arrays
 *	@*length - pointer to list length
 *	@***dim_list - pointer to array that sores 2D arrays dimensions
 */
void resize_matrix(int ****list, int *length, int ***dim_list)
{
	int index;
	scanf("%d", &index);

	int number_of_rows;
	scanf("%d", &number_of_rows);

	int *rows = (int *)calloc(number_of_rows, sizeof(int));
	for (int i = 0; i < number_of_rows; i++)
		scanf("%d", &rows[i]);

	int number_of_cols;
	scanf("%d", &number_of_cols);

	int *cols = (int *)calloc(number_of_cols, sizeof(int));
	for (int i = 0; i < number_of_cols; i++)
		scanf("%d", &cols[i]);

	if (is_out_of_bounds(index, *length)) {
		free(cols);
		free(rows);
		return;
	}

	int **result = (int **)calloc(number_of_rows, sizeof(int *));
	for (int i = 0; i < number_of_rows; i++)
		result[i] = (int *)calloc(number_of_cols, sizeof(int));

	for (int i = 0; i < number_of_rows; i++)
		for (int j = 0; j < number_of_cols; j++)
			result[i][j] = (*list)[index][rows[i]][cols[j]];

	free_matrix((*list)[index], (*dim_list)[index][0]);
	(*list)[index] = result;

	(*dim_list)[index][0] = number_of_rows;
	(*dim_list)[index][1] = number_of_cols;

	free(cols);
	free(rows);
}

/** Print an error if given index doesn't correspond to any array */
void print_out_of_bounds_error(void)
{
	printf("No matrix with the given index\n");
}

/** Check if the index is greater or equal with
 *	the length of the list or if its a negative number
 * @index - index to be checked
 * @length - current length
 */
int is_out_of_bounds(int index, int length)
{
	if (index >= length || index < 0) {
		print_out_of_bounds_error();
		return 1;
	}
	return 0;
}

////////////////////// STRASSEN ////////////////
int **add(int **matrix1, int **matrix2, int n)
{
	int **temp = alloc_matrix(n, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			temp[i][j] = ((matrix1[i][j] % MOD) + (matrix2[i][j] % MOD)) % MOD;
			if (temp[i][j] < 0)
				temp[i][j] += MOD;
		}

	return temp;
}
int **subtract(int **matrix1, int **matrix2, int n)
{
	int **temp = alloc_matrix(n, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			temp[i][j] = ((matrix1[i][j] % MOD) - (matrix2[i][j] % MOD)) % MOD;
			if (temp[i][j] < 0)
				temp[i][j] += MOD;
		}

	return temp;
}

int **strassen_multiplication(int **A, int **B, int n)
{
	if (n == 1) {
		int **C = alloc_matrix(1, 1);
		C[0][0] = ((A[0][0] % MOD) * (B[0][0] % MOD)) % MOD;
		if (C[0][0] < 0)
			C[0][0] += MOD;
		return C;
	}
	int **C = alloc_matrix(n, n);
	int k = n / 2;

	int **A11 = alloc_matrix(k, k);
	int **A12 = alloc_matrix(k, k);
	int **A21 = alloc_matrix(k, k);
	int **A22 = alloc_matrix(k, k);
	int **B11 = alloc_matrix(k, k);
	int **B12 = alloc_matrix(k, k);
	int **B21 = alloc_matrix(k, k);
	int **B22 = alloc_matrix(k, k);

	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++) {
			A11[i][j] = A[i][j];
			A12[i][j] = A[i][k + j];
			A21[i][j] = A[k + i][j];
			A22[i][j] = A[k + i][k + j];
			B11[i][j] = B[i][j];
			B12[i][j] = B[i][k + j];
			B21[i][j] = B[k + i][j];
			B22[i][j] = B[k + i][k + j];
		}

	int **sub_p1 = subtract(B12, B22, k);
	int **add_p2 = add(A11, A12, k);
	int **add_p3 = add(A21, A22, k);
	int **sub_p4 = subtract(B21, B11, k);
	int **add_p5_0 = add(A11, A22, k);
	int **add_p5_1 = add(B11, B22, k);
	int **sub_p6 = subtract(A12, A22, k);
	int **add_p6 = add(B21, B22, k);
	int **sub_p7 = subtract(A11, A21, k);
	int **add_p7 = add(B11, B12, k);

	int **P1 = strassen_multiplication(A11, sub_p1, k);
	int **P2 = strassen_multiplication(add_p2, B22, k);
	int **P3 = strassen_multiplication(add_p3, B11, k);
	int **P4 = strassen_multiplication(A22, sub_p4, k);
	int **P5 = strassen_multiplication(add_p5_0, add_p5_1, k);
	int **P6 = strassen_multiplication(sub_p6, add_p6, k);
	int **P7 = strassen_multiplication(sub_p7, add_p7, k);

	int **add_C11_0 = add(P5, P4, k);
	int **add_C11_1 = add(add_C11_0, P6, k);

	int **C11 = subtract(add_C11_1, P2, k);
	int **C12 = add(P1, P2, k);
	int **C21 = add(P3, P4, k);

	int **add_C22_0 = add(P5, P1, k);
	int **sub_C22_1 = subtract(add_C22_0, P3, k);

	int **C22 = subtract(sub_C22_1, P7, k);

	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++) {
			C[i][j] = C11[i][j];
			C[i][j + k] = C12[i][j];
			C[k + i][j] = C21[i][j];
			C[k + i][k + j] = C22[i][j];
		}

	free_matrix(sub_p1, k);
	free_matrix(add_p2, k);
	free_matrix(add_p3, k);
	free_matrix(sub_p4, k);
	free_matrix(add_p5_0, k);
	free_matrix(add_p5_1, k);
	free_matrix(sub_p6, k);
	free_matrix(add_p6, k);
	free_matrix(add_p7, k);
	free_matrix(sub_p7, k);

	free_matrix(add_C11_0, k);
	free_matrix(add_C11_1, k);
	free_matrix(add_C22_0, k);
	free_matrix(sub_C22_1, k);

	free_matrix(P1, k);
	free_matrix(P2, k);
	free_matrix(P3, k);
	free_matrix(P4, k);
	free_matrix(P5, k);
	free_matrix(P6, k);
	free_matrix(P7, k);
	free_matrix(C11, k);
	free_matrix(C12, k);
	free_matrix(C21, k);
	free_matrix(C22, k);

	free_matrix(A11, k);
	free_matrix(A12, k);
	free_matrix(A21, k);
	free_matrix(A22, k);
	free_matrix(B11, k);
	free_matrix(B12, k);
	free_matrix(B21, k);
	free_matrix(B22, k);

	return C;
}

void strassen(int ****matrix_list,
			  int *matrix_list_len,
			  int *matrix_list_size,
			  int ***dim_matrix)
{
	int index_first_matrix;
	int index_second_matrix;

	scanf("%d%d", &index_first_matrix, &index_second_matrix);

	if (is_out_of_bounds(index_first_matrix, *matrix_list_len) ||
		is_out_of_bounds(index_second_matrix, *matrix_list_len))
		return;

	int first_rows = (*dim_matrix)[index_first_matrix][0];
	int first_cols = (*dim_matrix)[index_first_matrix][1];

	int second_rows = (*dim_matrix)[index_second_matrix][0];
	int second_cols = (*dim_matrix)[index_second_matrix][1];

	if ((first_rows != first_cols ||
		second_rows != second_cols) ||
		first_rows != second_cols) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}

	if (*matrix_list_len == *matrix_list_size)
		realloc_matrix_list(matrix_list, dim_matrix,
							matrix_list_size,
							*matrix_list_len,
							*matrix_list_size * 2);

	int **result = strassen_multiplication((*matrix_list)[index_first_matrix],
											(*matrix_list)[index_second_matrix],
											first_rows);

	(*matrix_list)[*matrix_list_len] = result;
	(*dim_matrix)[*matrix_list_len][0] = first_rows;
	(*dim_matrix)[*matrix_list_len][1] = second_cols;
	*matrix_list_len = *matrix_list_len + 1;
}
