#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <condition_variable>

#include "Searcher.h"
#include "SimpleEvaluator.h"
#include "Move.h"
#include "Game.h"

extern "C" {
  #include "Array2D.h"
}

atomic<int> atomic_counter(0);

queue<Game> q;
mutex globalMutex;
condition_variable cv;
bool done;

using namespace std;

Searcher::Searcher() {}

void Searcher::SetDone() {
  done = true;
}

void Searcher::SetEvaluator(SimpleEvaluator evaluator) {
  this->evaluator = evaluator;
}

Move runBestMove(int depth) {
  Move m;
  unique_lock<mutex> lk(globalMutex);
  while (1) {
    lk.lock();
    while(q.empty() & !done) {
      cv.wait(lk);
    }
    
    if (done) {
      lk.unlock();
      break;
    }

    Game next = q.front();
    q.pop();
    vector<Move> moves = next.GenerateMoves();
    lk.unlock();    

    for (auto it = moves.cbegin(); it == moves.cend(); it++) {
      Game newGame(next);
      newGame.ApplyMove(*it);
      thread tr(runBestMove, depth + 1);
      lk.lock();
      q.push(newGame);
      lk.unlock();
      cv.notify_one();
    }
  }
  return m;
}

Move Searcher::GetBestMove(Game game) {
  q.push(game);
  return runBestMove(0);
}

