#include <iostream>
#include <atomic>
#include <thread>
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

vector<thread> threads;

mutex globalMutex;
condition_variable cv;
queue<Game> Searcher::states;
Move Searcher::bestMove;
atomic<int> Searcher::atomic_counter(0);
bool Searcher::done;

SimpleEvaluator eval;

Searcher::Searcher() {}

Move Searcher::setDone() {
  Searcher::done = true;
  cv.notify_all();
  for (auto &c : threads) {
    c.join();
  }
  threads.clear();
  Searcher::done = false;
  while (!Searcher::states.empty()) Searcher::states.pop();
  return Searcher::bestMove;
}

// g is the current game instance, moveChain is the chain of moves to get from the live game state
// to the current game state (size of moveChain is # of moves that have occurred). modify the evaluator
// (like how in chess we did .negate() to switch turns), 
void Searcher::runBestMove(int depth) {
  unique_lock<mutex> lk(globalMutex);
  lk.unlock();
  while (1) {
    lk.lock();
    while (!Searcher::done && Searcher::states.empty()) {
      cv.wait(lk);
    }
  
    if (Searcher::done) {
      lk.unlock();
      break;
    }

    Game next = Searcher::states.front();
    Searcher::states.pop();
    lk.unlock();
    vector<Move> moves = next.GenerateMoves();
    for (auto &c : moves) {
      Game newGame(next);
      //cout << c.GetRow() << ", " << c.GetCol() << ", " << c.GetDirection() << endl;
      newGame.ApplyMove(c);
      atomic_counter++;
      //cout << "test" << endl;
      int score = eval.Evaluate(newGame.GetBoardState());
      lk.lock();
      if (score > Searcher::bestMove.GetScore()) {
        Move curMove = newGame.moveHistory.at(depth);
        Searcher::bestMove.SetRow(curMove.GetRow());
        Searcher::bestMove.SetCol(curMove.GetCol());
        Searcher::bestMove.SetDirection(curMove.GetDirection());
        Searcher::bestMove.SetScore(score);
      }
      Searcher::states.push(newGame);
      lk.unlock();
      cv.notify_one();
    }
  }
}

void Searcher::GetBestMove(Game game) {
  Searcher::done = false;  
  Searcher::bestMove.SetRow(-1);
  Searcher::bestMove.SetCol(-1);
  Searcher::bestMove.SetDirection(-1);
  Searcher::bestMove.SetScore(-999999);
  Searcher::states.push(game);
  cout << game.GetMoves() << endl;
  for (int i = 0; i < 10; i++) {
    threads.push_back(thread(Searcher::runBestMove, game.GetMoves()));
  }
}
