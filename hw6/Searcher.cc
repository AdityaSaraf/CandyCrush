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
SimpleEvaluator eval;

using namespace std;

Searcher::Searcher() {}

void Searcher::SetDone() {
  done = !done;
}

void Searcher::SetEvaluator(SimpleEvaluator evaluator) {
  this->evaluator = evaluator;
}

// g is the current game instance, moveChain is the chain of moves to get from the live game state
// to the current game state (size of moveChain is # of moves that have occurred). modify the evaluator
// (like how in chess we did .negate() to switch turns), 
Move runBestMove(Game g, vector<Move> moveChain) {
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
    lk.unlock();    
    vector<Move> moves = next.GenerateMoves();

    for (auto it = moves.begin(); it == moves.end(); it++) {
      Game newGame(next);
      newGame.ApplyMove(*it);
      atomic_counter++;
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
  vector<Move> moves;
  vector<Move> moveList ;
  return runBestMove(game, moves);
}

