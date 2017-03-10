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
  Move GetBestMove(Game game);
  static int runBestMove(Game game);
  static void setDone();
  static bool done;
  static atomic<int> atomic_counter;

 private:
};

#endif // SEARCHER_H
