#ifndef GAME_H
#define GAME_H

#include <vector>

#include "Move.h"

using namespace std;

extern "C" {
  #include "Array2D.h"
}

class Game {
 public:
  vector<Move> moveHistory;
  Game();
  Game(const Game &other);
  void Init(const char *fileName);
  int GetColor(const int row, const int col);
  int GetState(const int row, const int col);
  int GetRows();
  int GetCols();
  int GetMoves();
  int GetScore();
  Array2D GetBoardState();
  vector<Move> GenerateMoves();
  void ApplyMove(Move m);
  int Swap(const int r1, const int c1, const int r2, const int c2);
  std::string SerializeCurrentState(); 
  bool IsWon();
  ~Game();

 private:
  int gameID;
  int moves;
  int score;
  int colors;
  int *extOffset;
  Array2D extBoard;
  Array2D boardState;
  Array2D boardCandies;
  bool Settle(bool fire);
  bool MatchTemplate(const int row, const int col, const int t, bool fire);
  void ApplyGravity();
  void ShiftDown(int startingRow, int col);
  Array2D CopyBoard(Array2D other);
};
#endif // GAME_H
