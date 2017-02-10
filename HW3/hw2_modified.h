#ifndef HW2_MODIFIED_H
#define HW2_MODIFIED_H

#include "Array2D.h"

// Deserialize an array using JANSSON; returns an Array2D
Array2D deserialize_array(const char* filename);

// Serialize an array using JANSSON
void serialize_array(Array2D arr, const char* filename);

// Free an Array2D
void destroy_array(Array2D arr);

#endif // HW2_MODIFIED_H