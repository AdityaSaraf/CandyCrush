#ifndef GAME_H
#define GAME_H

extern "C" {
  #include "Array2D.h"
}

class Game {
 public:
  Game();
  void Init(const char *fileName);
  int GetColor(const int row, const int col);
  int GetState(const int row, const int col);
  int GetRows();
  int GetCols();
  int GetMoves();
  int GetScore();
  int Swap(const int r1, const int c1, const int r2, const int c2);
  bool Settle();
  bool MatchTemplate(const int row, const int col, const int t);
  void ApplyGravity();
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
  void ShiftDown(int startingRow, int col);

  Game(const Game &copy) = delete;
  Game &operator=(Game rhs) = delete;
};

#endif // GAME_H
