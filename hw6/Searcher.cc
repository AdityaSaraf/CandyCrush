#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>

#include "Searcher.h"
#include "SimpleEvaluator.h"
#include "Move.h"
#include "Game.h"

extern "C" {
  #include "Array2D.h"
}

using namespace std;

mutex globalMutex;

atomic<int> moves_counter (0);
bool done = false;

Searcher::Searcher() {}

void Searcher::SetEvaluator(SimpleEvaluator evaluator) {
  this->evaluator = evaluator;
}

Move Searcher::GetBestMove(Game game) {
  Move m;
  queue<Game> q;
  q.push(game);
  while (1) {
    if (done) {
      break;
    }
    lock_guard<mutex> lock(globalMutex);
    Game next = q.front();
    q.pop();
    vector<Move> moves;
  }
  return m;
}
