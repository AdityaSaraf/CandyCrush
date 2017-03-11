#ifndef SEARCHER_H
#define SEARCHER_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "SimpleEvaluator.h"
#include "Move.h"
#include "Game.h"

extern "C" {
  #include "Array2D.h"
}

using namespace std;

class Searcher {
 public:
  Searcher();
  void GetBestMove(Game game);
  static void runBestMove(int depth);
  static Move setDone();
  
  static Move bestMove;
  static queue<Game> states;
  static bool done;
  static atomic<int> atomic_counter;

 private:
};

#endif // SEARCHER_H
