#include <atomic>
#include <thread>
#include <future>
#include <utility>
#include <queue>
#include <vector>

#include "Searcher.h"
#include "SimpleEvaluator.h"
#include "Move.h"
#include "Game.h"

extern "C" {
  #include "Array2D.h"
}

atomic<int> Searcher::atomic_counter(0);
SimpleEvaluator eval;
bool Searcher::done = false;

using namespace std;

Searcher::Searcher() {}

// g is the current game instance, moveChain is the chain of moves to get from the live game state
// to the current game state (size of moveChain is # of moves that have occurred). modify the evaluator
// (like how in chess we did .negate() to switch turns), 
int Searcher::runBestMove(Game game) {
  int bestScore = -21474836;
  queue<Game> q;
  q.push(game);
  while (!Searcher::done && !q.empty()) {
    Game next = q.front();
    q.pop();
    vector<Move> moves = next.GenerateMoves();
    for (auto it = moves.begin(); it == moves.end(); it++) {
      Game newGame(next);
      newGame.ApplyMove(*it);
      Searcher::atomic_counter++;
      int score = eval.Evaluate(newGame.GetBoardState()) - 25 * newGame.GetMoves();
      if (bestScore < score) {
        bestScore = score;
      }
      q.push(newGame);
    }
  }
  return bestScore;
}

Move Searcher::GetBestMove(Game game) {
  vector<Move> moves = game.GenerateMoves();
  vector<future<int>> futures;
  for (int i = 0; i < 4; i++) {
    Move m = moves.at(i);
    Game newGame(game);
    newGame.ApplyMove(m);
    Searcher::atomic_counter++;
    packaged_task<int(Game)> tsk(runBestMove);
    future<int> fut = tsk.get_future();
    thread(move(tsk), newGame).detach();
  }
  int bestScore = futures.at(0).get();
  Move bestMove = moves.at(0);
  for (int i = 1; i < 4; i++) {
    int nextScore = futures.at(i).get();
    if (bestScore < nextScore) {
      bestMove = moves.at(i);
    }
  }
  return bestMove;
}

