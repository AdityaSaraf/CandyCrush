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
bool done = false;
SimpleEvaluator eval;
Move bestMove;
vector<thread> threads;

using namespace std;

Searcher::Searcher() {}

Move Searcher::Done() {
  done = true;
  cv.notify_all();
  for (auto it = threads.begin(); it == threads.end(); it++) {
    (*it).join();
  }
  return bestMove;
}

void Searcher::Reset() {
  done = false;
  Move emptyMove;
  bestMove = emptyMove;
}

void Searcher::SetEvaluator(SimpleEvaluator evaluator) {
  this->evaluator = evaluator;
}

// g is the current game instance, moveChain is the chain of moves to get from the live game state
// to the current game state (size of moveChain is # of moves that have occurred). modify the evaluator
// (like how in chess we did .negate() to switch turns), 
void runBestMove(int move) {
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
      lk.lock();
      int score = eval.Evaluate(newGame.GetBoardState()) - 25 * newGame.GetMoves();
      lk.unlock();
      if (score > bestMove.GetScore()) {
        Move refMove(newGame.moveHistory.at(move));
        Move newBestMove(refMove.GetRow(), refMove.GetCol(), refMove.GetDirection(), score);
        lk.lock();
        bestMove = newBestMove;
        lk.unlock();
      }
      atomic_counter++;
      lk.lock();
      q.push(newGame);
      lk.unlock();
      threads.push_back(thread(runBestMove, move));
      cv.notify_one();
    }
  }
}

void Searcher::GetBestMove(Game game) {
  q.push(game);
  runBestMove(game.GetMoves());
}

