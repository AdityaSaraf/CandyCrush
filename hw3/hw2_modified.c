#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jansson.h>

extern "C" {
  #include "hw2_modified.h"
}

json_t *root;

Array2D deserialize_array(const char* filename) {
	json_error_t error;
	root = json_load_file(filename, 0, &error);
	json_t *rows = json_object_get(root, "rows");
  json_t *cols = json_object_get(root, "columns");
	int rowNum = json_integer_value(rows);
  int colNum = json_integer_value(cols);
  json_decref(rows);
  json_decref(cols);

  Array2D arr2d = Array2D_create(rowNum, colNum);

  json_t *arr = json_object_get(root, "data");
  json_t* el;
  for (int i = 0; i < (rowNum * colNum); i++) {
    el = json_array_get(arr, (size_t) i);
    int* n = (int*) malloc(sizeof(int));
    *n = (int) json_integer_value(el);
    Array2D_set(arr2d, i/colNum, i%colNum, n);
  }
  //json_decref(el);
  //json_decref(arr);
  //json_decref(root);
  return arr2d;
}

void serialize_array(Array2D arr, const char* filename) {
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
      json_array_append(jarr, json_integer(*(int*)Array2D_get(arr, i, j)));
    }
  }
  json_object_set(obj, "data", jarr);
  json_dump_file(obj, filename, 0);
  json_decref(obj);
  json_decref(jarr);
}

void destroy_array(Array2D arr) {
  Array2D_destroy(arr, (Array2DDataFreeFnPtr) &free);
  json_decref(root);
}
