#ifndef EVALUATOR_H
#define EVALUATOR_H

extern "C" {
  #include "Array2D.h"
}

class SimpleEvaluator {
 public:
  SimpleEvaluator();
  int Evaluate(Array2D state);

 private:
};

#endif // EVALUATOR_H
