#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jansson.h>

#include "Array2D.h"

int main(int argc, char *argv[]) {

	if (argc != 2)
	{
		printf("usage: %s filename\n", argv[0]);
	}
	else
	{
		json_error_t error;
		json_t *root = json_load_file(argv[1], 0, &error);
		json_t *rows = json_object_get(root, "rows");
		int rowNum = json_integer_value(rows);
		printf("%i\n", rowNum);
	}
	return EXIT_SUCCESS;
}
