#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hw2_modified.h"

int main(int argc, char *argv[]) {

	if (argc != 2)
	{
		printf("usage: %s filename\n", argv[0]);
	}
	else
	{
    Array2D arr2d = deserialize_array(argv[1]);
    destroy_array(arr2d);
	}
	return EXIT_SUCCESS;
}