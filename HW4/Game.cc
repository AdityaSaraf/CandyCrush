#include "Game.h"
#include <iostream>

extern "C" {
  #include <jansson.h>
  #include "Array2D.h"
}

Array2D deserialize(json_t *root) {
  json_t *jrows = json_object_get(root, "rows");
  json_t *jcols = json_object_get(root, "columns");
  int rows = json_integer_value(jrows);
  int cols = json_integer_value(jcols);
  json_decref(jrows);
  json_decref(jcols);

  Array2D result = Array2D_create(rows, cols);
  json_t *data = json_object_get(root, "data");
  json_t *el;
  for (int i = 0; i < (rows * cols); i++) {
    el = json_array_get(data, (size_t) i);
    int *n = (int*) malloc(sizeof(int));
    *n = json_integer_value(el);
    Array2D_set(result, i/cols, i%cols, n);
  }
  json_decref(el);
  json_decref(data);
  json_decref(root);

  return result;
}

Game::Game() {

}

void Game::Init(const char *fileName) {
  json_t *root;
  json_error_t error;
  root = json_load_file(fileName, 0, &error);

  json_t *gameDef = json_object_get(root, "gamedef");
  json_t *gameState = json_object_get(root, "gamestate");

  json_t *gameid = json_object_get(gameDef, "gameid");
  gameID = json_integer_value(gameid);
  //json_decref(gameid);

  json_t *extb = json_object_get(gameDef, "extensioncolor");
  extBoard = deserialize(extb);
  extOffset = (int*) malloc(sizeof(int) * extBoard->cols);
  if (gameState) {
    // get boardCandies and boardState from gameState, moves, score, offset
    json_t *bCandies = json_object_get(gameState, "boardcandies");
    json_t *jrows = json_object_get(bCandies, "rows");
    json_t *jcols = json_object_get(bCandies, "columns");
    int rows = json_integer_value(jrows);
    int cols = json_integer_value(jcols);
    json_decref(jrows);
    json_decref(jcols);

    boardCandies = Array2D_create(rows, cols);
    json_t *data = json_object_get(root, "data");
    json_t *el;
    for (int i = 0; i < (rows * cols); i++) {
      el = json_array_get(data, (size_t) i);
      json_t *num = json_object_get(el, "color");
      int *n = (int*) malloc(sizeof(int));
      *n = json_integer_value(num);
      Array2D_set(boardCandies, i/cols, i%cols, n);
    }
    json_decref(data);
    json_decref(bCandies);
    
    json_t *bState = json_object_get(gameState, "boardstate");
    boardState = deserialize(bState);
    json_t *jmoves = json_object_get(gameState, "moves");
    moves = json_integer_value(jmoves);
    json_decref(jmoves);

    json_t *jscore = json_object_get(gameState, "currentscore");
    score = json_integer_value(jscore);
    json_decref(jscore);
    
    json_t *joffset = json_object_get(gameState, "extensionoffset");
    for (int i = 0; i < (int) json_array_size(joffset); i++) {
      el = json_array_get(joffset, (size_t) i);
      extOffset[i] = json_integer_value(el);
    }
  } else {
    json_t *bState = json_object_get(gameDef, "boardstate");
    boardState = deserialize(bState);
    extOffset[boardState->cols] = {0};

    for (int i = 0; i < extBoard -> cols; i++) {
      extOffset[i] = 0;
    }
    boardCandies = Array2D_create(boardState->rows, boardState->cols);
    for (int i = 0; i < boardCandies->rows; i++) {
      for (int j = 0; j < boardCandies->cols; j++) {
        int *n = (int*) malloc(sizeof(int));
        *n = -1;
        Array2D_set(boardCandies, i, j, n);
      }
    }
    this->ApplyGravity();
  }
  this->Settle();

  json_decref(gameDef);
  json_decref(gameState);
  json_decref(root);
}

int Game::GetColor(const int row, const int col) {
  int *result = (int*) Array2D_get(boardCandies, row, col);
  return *result;
}

int Game::GetState(const int row, const int col) {
  int *result = (int*) Array2D_get(boardState, row, col);
  return *result;
}

int Game::GetRows() {
  return boardCandies->rows;
}

int Game::GetCols() {
  return boardCandies->cols;
}

int Game::GetMoves() {
  return moves;
}

int Game::Swap(const int r1, const int c1, const int r2, const int c2) {
  // swap, if no change, swap back
  int code = Array2D_swap(boardCandies, r1, c1, r2, c2);
  if (code < 0)
  {
    return -1;
  }
  if (!this->Settle())
  {
    Array2D_swap(boardCandies, r1, c1, r2, c2);
    return 0;
  }
  return 1;
}

bool Game::Settle() {
  bool fired = false;
  int counter = 0;
  do{
    fired = false;
    for (int t = 0; t < 4; t++) {
      int rows = 0;
      int cols = 0;
      switch (t){
        case 0:
          rows = boardCandies->rows-3;
          cols = boardCandies->cols;
          break;
        case 1:
          rows = boardCandies->rows;
          cols = boardCandies->cols-3;
          break;
        case 2:
          rows = boardCandies->rows-2;
          cols = boardCandies->cols;
          break;
        case 3:
          rows = boardCandies->rows;
          cols = boardCandies->cols-2;
          break;
        default:
           std::cout<<"Error, bad template call in Game.cc" << std::endl;
           break;
        }
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
          // tries all possible templates
          // fires if template matches
          // sets fired to true if any template fired
          fired |= this->MatchTemplate(i, j, t);
        }
      }
    }
    // apply gravity
    if (fired)
    {
      // to help debug
        int *cd = new int[boardCandies->rows*boardCandies->cols];
        for (int i = 0; i < boardCandies->rows; ++i)
        {
          for (int k = 0; k < boardCandies->cols; k++)
          {
            cd[i*boardCandies->cols+k] = *(int*)(Array2D_get(boardCandies, i, k));
          }
        }
      this->ApplyGravity();
      counter++;
    }
      // to help debug
        int *cd = new int[boardCandies->rows*boardCandies->cols];
        for (int i = 0; i < boardCandies->rows; ++i)
        {
          for (int k = 0; k < boardCandies->cols; k++)
          {
            cd[i*boardCandies->cols+k] = *(int*)(Array2D_get(boardCandies, i, k));
          }
        }
    if (counter == 7)
    {
      // to help debug
        int *cd = new int[boardCandies->rows*boardCandies->cols];
        for (int i = 0; i < boardCandies->rows; ++i)
        {
          for (int k = 0; k < boardCandies->cols; k++)
          {
            cd[i*boardCandies->cols+k] = *(int*)(Array2D_get(boardCandies, i, k));
          }
        }
      int test = 0;
    }
    // if something was fired then gravity had an effect and so we should settle the board again
  } while (fired && counter < 1000);
  // counter = 0 means that no template matched/no template fired
  if (counter == 0)
  {
    return false;
  }
  return true;
}

bool Game::MatchTemplate(const int row, const int col, const int t) {
  // if 0, match vFour
  // if 1, match hFour
  // if 2, match vThree
  // if 3, match hThree
  int horizontalOffset = 0;
  int verticalOffset = 0;
  int matches = 0;
  switch (t){
    case 0:
      verticalOffset = 1;
      matches = 3;
      break;
    case 1:
      horizontalOffset = 1;
      matches = 3;
      break;
    case 2:
      verticalOffset = 1;
      matches = 2;
      break;
    case 3:
      horizontalOffset = 1;
      matches = 2;
      break;
    default:
       std::cout<<"Error, bad template call in Game.cc" << std::endl;
       break;
  }
  // collects pointers to all the candies and their associated state square
  int** candies = new int*[matches+1];
  int** states = new int*[matches+1];
  candies[0] = (int*)(Array2D_get(boardCandies, row, col));
  states[0] = (int*)(Array2D_get(boardState, row, col));
  int color = *candies[0];
  if (color == -1)
  {
    return false;
  }
  // checks template - exits with false if template misses
  for (int i = 1; i <= matches; i++)
  {
    candies[i] = (int*)(Array2D_get(boardCandies, row + (verticalOffset * i), col + (horizontalOffset * i)));
    states[i] = (int*)(Array2D_get(boardState, row + (verticalOffset * i), col + (horizontalOffset * i)));
    int nextColor = *candies[i];
    if (nextColor != color)
    {
      delete[] states;
      delete[] candies;
      return false;
    }
  }
  // fire template + decrement the board state in the fired spaces
  for (int i = 0; i <= matches; i++)
  {
    // sets the candy "color" to -1
    *candies[i] = -1;
    // decrement board state
    (*states[i])--;
  }
  // free the arrays, not the associated pointers
  delete[] states;
  delete[] candies;
  return true;
}

void Game::ApplyGravity() {
  // check each column first
  for (int j = 0; j < boardCandies->cols; j++){
    int cap = boardCandies->cols;
    for (int i = 0; i < boardCandies->rows; i++) {
      // everything above cap is from the extension board and so cannot be -1
      if (i >= cap)
      {
        // to help debug
        int *cd = new int[boardCandies->rows];
        for (int k = 0; k < boardCandies->rows; k++)
        {
          cd[k] = *(int*)(Array2D_get(boardCandies, k, j));
        }
        break;
      }
      // keep replacing with the upper candy until candy is valid
      int* currCandy = (int*)(Array2D_get(boardCandies, i, j));
      while (*currCandy == -1)
      {
        this->ShiftDown(i, j);
        cap--;
      }
    }
  }
}

void Game::ShiftDown(int startingRow, int col) {
  // shift the rows down except the last one
  for (int i = startingRow; i < boardCandies->rows - 1; i++)
  {
    int* currCandy = (int*)(Array2D_get(boardCandies, i, col));
    *currCandy = *(int*)(Array2D_get(boardCandies, i+1, col));

  }
  // fill in the highest row with the extension board
  int* highestCandy = (int*)(Array2D_get(boardCandies, boardCandies->rows -1, col));
  int replacement = *(int*)(Array2D_get(extBoard, extOffset[col], col));
  *highestCandy = replacement;
  // increment the offset, modding by # of rows
  extOffset[col] = (extOffset[col] + 1) % boardCandies->rows;
}

Game::~Game() {
  free(extOffset);
  Array2D_destroy(boardCandies, (Array2DDataFreeFnPtr) &free);
  Array2D_destroy(boardState, (Array2DDataFreeFnPtr) &free);
  Array2D_destroy(extBoard, (Array2DDataFreeFnPtr) &free);
}
