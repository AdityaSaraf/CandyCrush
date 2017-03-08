#ifndef MOVE_H
#define MOVE_H

class Move {
 public:
  Move() { Move(-1, -1, -1, 0, 0); }
  Move(int row, int col, int direction) { Move(row, col, direction, 0, 0); }
  Move(int row, int col, int direction, 
       int depth, int score) : row(row), 
                               col(col),
                               direction(direction),
                               depth(depth),
                               score(score) {}
  Move(const Move &other) {
    this->row = other.row;
    this->col = other.col;
    this->direction = other.direction;
    this->depth = other.depth;
    this->score = other.score;
  }

  int GetRow() { return row; }
  int GetCol() { return col; }
  int GetDirection() { return direction; }
  int GetDepth() { return depth; }
  int GetScore() { return score; }
  
 private:
  int row;
  int col;
  int direction;
  int depth;
  int score;
};

#endif //MOVE_H
