#ifndef SEARCHER_H
#define SEARCHER_H

#include <queue>
#include <mutex>
#include <condition_variable>

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
  void GetBestMove(Game game);
  Move Done();
  void Reset();
  SimpleEvaluator evaluator;

 private:
};

#endif // SEARCHER_H
