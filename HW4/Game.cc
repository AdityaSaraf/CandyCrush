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
    Array2D_set(result, result->rows - (i/cols) - 1, i%cols, n);
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
    boardCandies = deserialize(bCandies);

    json_t *bState = json_object_get(gameState, "boardstate");
    boardState = deserialize(bState);
    json_t *jmoves = json_object_get(gameState, "moves");
    moves = json_integer_value(jmoves);
    json_decref(jmoves);

    json_t *jscore = json_object_get(gameState, "currentscore");
    score = json_integer_value(jscore);
    json_decref(jscore);
    
    json_t *joffset = json_object_get(gameState, "extensionoffset");
    json_t *el;
    for (int i = 0; i < (int) json_array_size(joffset); i++) {
      el = json_array_get(joffset, (size_t) i);
      extOffset[i] = json_integer_value(el);
    }
  } else {
    json_t *bState = json_object_get(gameDef, "boardstate");
    boardState = deserialize(bState);

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

void Game::Swap(const int r1, const int c1, const int r2, const int c2) {
  Array2D_swap(boardCandies, r1, c1, r2, c2);
  this->settle();
}

void Game::Settle() {
  bool fired = false;
  do{
    for (int t = 0; t < 4; t++) {
      for (int i = 0; i < boardCandies->rows; i++) {
        for (int j = 0; j < boardCandies->cols; j++) {
          // tries all possible templates
          // fires if template matches
          // sets fired to true if any template fired
          fired |= this->MatchTemplate(t, i, j);
        }
      }
    }
    // apply gravity
    this->ApplyGravity();
    // if something was fired then gravity had an effect and so we should settle the board again
  } while (fired);
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
  // checks template - exits with false if template misses
  for (int i = 1; i <= matches; i++)
  {
    candies[i] = (int*)(Array2D_get(boardCandies, row + (horizontalOffset * i), col + (verticalOffset * i)));
    states[i] = (int*)(Array2D_get(boardState, row + (horizontalOffset * i), col + (verticalOffset * i)));
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
    *currCandy = *(int*)(Array2D_get(boardCandies, i, col));
  }
  // fill in the highest row with the extension board
  int* highestCandy = (int*)(Array2D_get(boardCandies, boardCandies->rows -1, col));
  int replacement = *(int*)(Array2D_get(extBoard, extOffset[col], col));
  *highestCandy = replacement;
  // increment the offset, modding by # of rows
  extOffset[col] = (extOffset[col] + 1) % boardCandies->rows;
}

Game::~Game() {
  Array2D_destroy(boardCandies, (Array2DDataFreeFnPtr) &free);
  Array2D_destroy(boardState, (Array2DDataFreeFnPtr) &free);
  Array2D_destroy(extBoard, (Array2DDataFreeFnPtr) &free);
}
