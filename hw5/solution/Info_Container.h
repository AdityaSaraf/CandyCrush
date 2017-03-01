#ifndef INFO_CONTAINER_H
#define INFO_CONTAINER_H

#include <iostream>

extern "C" {
  #include "Array2D.h"
}

class Info_Container {
 public:
  Info_Container();
  void Init(const char *jstring);
  int GetColor(const int row, const int col);
  int GetState(const int row, const int col);
  int GetRows();
  int GetCols();
  int GetMoves();
  int GetScore();
  ~Info_Container();

 private:
  int gameID;
  int moves;
  int score;
  int colors;
  int *extOffset;
  Array2D extBoard;
  Array2D boardState;
  Array2D boardCandies;

  Info_Container(const Info_Container &copy) = delete;
  Info_Container &operator=(Info_Container rhs) = delete;
};

#endif // INFO_CONTAINER_H
