#ifndef CANDY_H
#define CANDY_H

class Candy {
 public:
  Candy(int color, int state) {
    this->color = color;
    this->state = state;
  };

 private:
  int color;
  int state;
};

#endif // CANDY_H
