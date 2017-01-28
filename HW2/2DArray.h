#ifndef 2DARRAY_H
#define 2DARRAY_H

#include <stdbool.h>

// This struct represents the 2D array. It contains a void pointer to the client's data,
// for generality and ints representing the rows and columns of the 2D array.
typedef struct arr_t
{
	(void*) arr;
	int rows;
	int cols;
}* Arr;


// A pointer to a function that frees the data of the array. This function will be invoked
// once for every element in the array, before the array struct itself is freed.
typedef void(*DataFreeFnPtr)(void* data);

// Allocates space for a 2D array of the given height and width and returns the array.
//
// Arguments:
//
// - rows: the number of rows in the 2D array
//
// - cols: the number of columns in the 2D array
//
// Returns:
//
// - NULL on error, non-NULL on success.
Arr 2DArray_create(int rows, int cols);

// Sets the value of a given index in the 2D array to the data passed in
//
// Arguments:
//
// - a: the 2D array to operate on
//
// - row: the row index in the 2D array
//
// - col: the column index in the 2D array
//
// - data: a pointer to the data
//
// Returns:
//
// - 1 on success
// - 0 if a is null
// - -1 if data is null
// - -2 if row is out of bounds
// - -3 if col is out of bounds
int 2DArray_set(Arr a, int row, int col, void* data);

// Swaps the values of two different indexes in the 2D array
//
// Arguments:
//
// - a: the 2D array to operate on
//
// - r1: the row index for the first element in the 2D array
//
// - c1: the column index for the first element in the 2D array
//
// - r2: the row index for the second element in the 2D array
//
// - c2: the column index for the second element in the 2D array
//
// Returns:
//
// - 1 on success
// - 0 if a is null
// - -1 if data is null at index 1
// - -2 if data is null at index 2
// - -3 if r1 OR c1 is out of bounds
// - -4 if r2 OR c2 is out of bounds
int 2DArray_swap(Arr a, int r1, int c1, int r2, int c2);

// Gets the value at a certain index in the 2D array. Also prints outOfBounds errors
//
// Arguments:
//
// - a: the 2D array to operate on
//
// - row: the row index in the 2D array
//
// - col: the column index in the 2D array
//
// Returns:
//
// - a pointer to the data at the given index
void* 2DArray_get(Arr a, int row, int col);

// Free a 2D array that was previously allocated by 2Darray_create
//
// Arguments:
//
// - a: the 2D array to operate on
//
// - data_free_function: a pointer to a data freeing function; see above for details
void 2DArray_destroy(Arr a, DataFreeFnPtr data_free_function);


#endif // 2DARRAY_H