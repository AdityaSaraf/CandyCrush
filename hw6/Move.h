#ifndef MOVE_H
#define MOVE_H

class Move {
 public:
  Move() { Move(-1, -1, -1, 0); }
  Move(int row, int col, int direction) { Move(row, col, direction, 0); }
  Move(int row, int col, int direction, int score) : row(row), 
                                                     col(col), 
                                                     direction(direction), 
                                                     score(score) {}
  int GetScore() { return score; }
  int GetDirection() { return direction; }
  int GetRow() { return row; }
  int GetCol() { return col; }
  
 private:
  int row;
  int col;
  int direction;
  int score;
};

#endif //MOVE_H
