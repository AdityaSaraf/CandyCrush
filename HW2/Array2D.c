#include <stdlib.h>

#include "Array2D.h"

// Verifies a 2D array index - returns 0 if the array is null, -1 if the row
// is out of bounds and -2 if the columns is out of bounds
int VerifyArray2D(Array2D a, int row, int col) {
  if (!a) {
    return 0;
  }
  if (row < 0 || row > a->rows - 1) {
    return -1;
  }
  if (col < 0 || col > a->cols - 1) {
    return -2;
  }
  return 1;
}

Array2D Array2D_create(int rows, int cols) {
  if (rows < 1 || cols < 1) {
    return NULL;
  }

  Array2D a = malloc(sizeof(*a));
  if (!a) {
    return NULL;
  }

  a->arr = malloc(sizeof(Array2DData_t)*(rows*cols));
  if (!(a->arr)) {
    return NULL;
  }
  
  a->rows = rows;
  a->cols = cols;
  return a;
}

int Array2D_set(Array2D a, int row, int col, Array2DData_t data) {
  int v = VerifyArray2D(a, row, col);
  if (v < 1) {
    return v;
  }
  if (!data) {
    return -3;
  }

  a->arr[(row * a->cols) + col] = data;
  return 1;
}

int Array2D_swap(Array2D a, int r1, int c1, int r2, int c2) {
  int v1 = VerifyArray2D(a, r1, c1);
  int v2 = VerifyArray2D(a, r2, c2);
  if (v1 < 1) {
    return v1;
  }
  if (v2 < 1) {
    return v2;
  }

  // fetch the data from both indexes
  Array2DData_t data1 = a->arr[(r1 * a->cols) + c1];
  Array2DData_t data2 = a->arr[(r2 * a->cols) + c2];
  
  if (!data1) {
    return -3;
  }
  if (!data2) {
    return -4;
  }

  //swap the data
  a->arr[(r1 * a->cols) + c1] = data2;
  a->arr[(r2 * a->cols) + c2] = data1;
  return 1;
}

Array2DData_t Array2D_get(Array2D a, int row, int col) {
  return a->arr[(row * a->cols) + col];
}

int Array2D_destroy(Array2D a, Array2DDataFreeFnPtr data_free_function) {
  if (!a) {
    return 0;
  }
  if (!data_free_function) {
    return -1;
  }
  for (int i = 0; i < (a->rows * a->cols); i++) {
    data_free_function(a->arr[i]);
  }
  free(a->arr);
  free(a);
  return 1;
}
