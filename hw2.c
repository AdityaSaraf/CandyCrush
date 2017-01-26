#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jansson.h>

#include "Array2D.h"

int operations;

void print_array(Array2D arr) {
  printf("Array is %d x %d\n", arr->rows, arr->cols);
  for (int i = 0; i < arr->rows; i++) {
    for (int j = 0; j < arr->cols; j++) {
      printf("%d ", (int) json_integer_value(Array2D_get(arr, i, j)));
    }
  }
  printf("\n\n");
}

Array2D deserialize_array(const char* filename) {
  operations++;
  printf("[%d] Deserialize %s\n", operations, filename);
	json_error_t error;
	json_t *root = json_load_file(filename, 0, &error);
	json_t *rows = json_object_get(root, "rows");
  json_t *cols = json_object_get(root, "columns");
	int rowNum = json_integer_value(rows);
  int colNum = json_integer_value(cols);
  json_decref(rows);
  json_decref(cols);

  Array2D arr2d = Array2D_create(rowNum, colNum);

  json_t *arr = json_object_get(root, "data");
    
  for (int i = 0; i < (rowNum * colNum); i++) {
    json_t* el = json_array_get(arr, (size_t) i);
    int n = (int) json_integer_value(el);
    printf("\t\tdeserialized %d\n", (int) n);
    Array2D_set(arr2d, i/colNum, i%colNum, el);
  }
  print_array(arr2d);
  return arr2d;
}

void set_array(Array2D arr, int row, int col, int val) {
  operations++;
  printf("[%d] Set [%d][%d] to %d\n", operations, row, col, val);
  Array2D_set(arr, row, col, json_integer(val));
  print_array(arr);
}

void swap_array(Array2D arr, int r1, int c1, int r2, int c2) {
  operations++;
  printf("[%d] swap [%d][%d] and [%d][%d]\n", operations, r1, c1, r2, c2);
  Array2D_swap(arr, r1, c1, r2, c2);
  print_array(arr);
}

void serialize_array(Array2D arr, const char* filename) {
  operations++;
  printf("[%d] serialize array to file %s\n", operations, filename);
  json_t *obj = json_object();
  json_t *rows = json_integer((json_int_t) arr->rows);
  json_t *cols = json_integer((json_int_t) arr->cols);
  json_object_set(obj, "rows", rows);
  json_object_set(obj, "columns", cols);
  json_decref(rows);
  json_decref(cols);
  json_t *jarr = json_array();
  for (int i = 0; i < arr->rows; i++) {
    for (int j = 0; j < arr->cols; j++) {
      printf("\t\tserialized %d\n", (int) json_integer_value(Array2D_get(arr, i, j)));
      json_array_append(jarr, Array2D_get(arr, i, j));
    }
  }
  json_object_set(obj, "data", jarr);
  json_dump_file(obj, filename, 0);
  json_decref(obj);
  json_decref(jarr);
  printf("\n");
}

void destroy_array(Array2D arr) {
  operations++;
  printf("[%d] destroy array\n", operations);
  for (int i = 0; i < arr->rows; i++) {
    for (int j = 0; j < arr->cols; j++) {
      json_t *el = Array2D_get(arr, i, j);
      printf("\t\tdestroyed %d\n", (int) json_integer_value(el));
    }
  }
  Array2D_destroy(arr, (Array2DDataFreeFnPtr) &json_decref);
}

int main(int argc, char *argv[]) {

	if (argc != 2)
	{
		printf("usage: %s filename\n", argv[0]);
	}
	else
	{
    Array2D arr2d = deserialize_array(argv[1]);
    set_array(arr2d, 1, 1, 100);
    swap_array(arr2d, 1, 1, 0, 0);
    swap_array(arr2d, 1, 1, 10, 10);
    serialize_array(arr2d, "./json.out");
    destroy_array(arr2d);
	}
	return EXIT_SUCCESS;
}

