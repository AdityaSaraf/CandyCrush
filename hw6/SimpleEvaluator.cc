#include <iostream>
#include "SimpleEvaluator.h"

extern "C" {
  #include "Array2D.h"
}

using namespace std;

int SimpleEvaluator::Evaluate(Array2D states) {
  int sum = 0;
  for (int i = 0; i < states->rows; i++) {
    for (int j = 0; j < states->cols; j++) {
      int state = *(int*) Array2D_get(states, i, j);
      sum += (states->rows - i) * (2 - state);
    }
  }
  return sum;
}
