#ifndef MOVE_H
#define MOVE_H

class Move {
 public:
  Move() :  _row(-1),
            _col(-1),
            _direction(-1),
            _score(0) {}
  Move(int row, int col, int direction) : _row(row),
                                          _col(col),
                                          _direction(direction),
                                          _score(0) {}
  Move(int row, int col, int direction, int score) : _row(row), 
                                                     _col(col),
                                                     _direction(direction),
                                                     _score(score) {}
  Move(const Move &other) {
    this->_row = other._row;
    this->_col = other._col;
    this->_direction = other._direction;
    this->_score = other._score;
  }
  Move operator=(const Move &other) {
    Move result(other._row, other._col, other._direction, other._score);
    return result;
  }

  int GetRow() { return _row; }
  void SetRow(const int row) { _row = row; }
  int GetCol() { return _col; }
  void SetCol(const int col) { _col = col; }
  int GetDirection() { return _direction; }
  void SetDirection(const int dir) { _direction = dir; }
  int GetScore() { return _score; }
  void SetScore(const int score) { _score = score; }
  
 private:
  int _row;
  int _col;
  int _direction;
  int _score;
};

#endif //MOVE_H
