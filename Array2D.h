#ifndef ARRAY2D_H
#define ARRAY2D_H

// Each piece of data in the array is a general pointer. If the data no larger
// than a pointer, then it can be casted to this type, otherwise a pointer to
// the data is stored.
typedef void* Array2DData_t;

// This struct represents the 2D array. It contains a void pointer to the client's data,
// for generality and ints representing the rows and columns of the 2D array.
typedef struct array2D_t {
  Array2DData_t* arr;
  int rows;
  int cols;
}* Array2D;

// A pointer to a function that frees the data of the array. This function will be invoked
// once for every element in the array, before the array struct itself is freed.
typedef void(*Array2DDataFreeFnPtr)(void* data);

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
Array2D Array2D_create(int rows, int cols);

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
// - -1 if row is out of bounds
// - -2 if col is out of bounds
// - -3 if data is null
int Array2D_set(Array2D a, int row, int col, Array2DData_t data);

// Swaps the values of two different indices in the 2D array
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
// - -1 if r1 OR r2 is out of bounds
// - -2 if c1 OR c2 is out of bounds
// - -3 if data is null at index 1
// - -4 if data is null at index 2
int Array2D_swap(Array2D a, int r1, int c1, int r2, int c2);

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
// - dataP: return pointer for the data at the index
//
// Returns:
//
// - 1 on success
// - 0 if a is null
// - -1 if row is out of bounds
// - -2 if col is out of bounds
int Array2D_get(Array2D a, int row, int col, Array2DData_t *dataP);

// Free a 2D array that was previously allocated by 2Darray_create
//
// Arguments:
//
// - a: the 2D array to operate on
//
// - data_free_function: a pointer to a data freeing function; see above for details
//
// Returns:
//
// - 1 on success
// - 0 if a is null
// - -1 if data_free_function is null
int Array2D_destroy(Array2D a, Array2DDataFreeFnPtr data_free_function);


#endif // 2DARRAY_H
