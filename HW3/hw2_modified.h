#ifndef HW2_MODIFIED_H
#define HW2_MODIFIED_H

#include "Array2D.h"

Array2D deserialize_array(const char* filename);

void serialize_array(Array2D arr, const char* filename);

void destroy_array(Array2D arr);

#endif // HW2_MODIFIED_H