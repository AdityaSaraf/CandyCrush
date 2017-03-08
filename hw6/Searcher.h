#ifndef SEARCHER_H
#define SEARCHER_H

#include "SimpleEvaluator.h"
#include "Move.h"
#include "Game.h"

extern "C" {
  #include "Array2D.h"
}

class Searcher {
 public:
  Searcher();
  void SetEvaluator(SimpleEvaluator evaluator);
  Move GetBestMove(Game game);

 private:
  SimpleEvaluator evaluator;
};

#endif // SEARCHER_H
