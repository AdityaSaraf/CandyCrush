#include "Info_Container.h"
#include <iostream>

extern "C" {
  #include <jansson.h>
  #include "Array2D.h"
}

Array2D deserializeF(json_t *root) {
  json_t *jrows = json_object_get(root, "rows");
  json_t *jcols = json_object_get(root, "columns");
  int rows = json_integer_value(jrows);
  int cols = json_integer_value(jcols);
  //json_decref(jrows);
  //json_decref(jcols);
  Array2D result = Array2D_create(rows, cols);
  json_t *data = json_object_get(root, "data");
  json_t *el;
  for (int i = 0; i < (rows * cols); i++) {
    el = json_array_get(data, (size_t) i);
    int *n = (int*) malloc(sizeof(int));
    *n = json_integer_value(el);
    Array2D_set(result, i/cols, i%cols, n);
  }
  //json_decref(el);
  //json_decref(data);
  //json_decref(root);
  return result;
}

Info_Container::Info_Container() {

}

void Info_Container::Init(const char *fileName) {
  json_t *root;
  json_error_t error;
  root = json_loads(fileName, 0, &error);

  json_t *gameinstance = json_object_get(root, "gameinstance");

  json_t *gameDef = json_object_get(gameinstance, "gamedef");
  json_t *gameState = json_object_get(gameinstance, "gamestate");

  json_t *gameid = json_object_get(gameDef, "gameid");
  gameID = json_integer_value(gameid);
  json_decref(gameid);

  json_t *jcolors = json_object_get(gameDef, "colors");
  colors = json_integer_value(jcolors);
  json_decref(jcolors);

  json_t *extb = json_object_get(gameDef, "extensioncolor");
  extBoard = deserializeF(extb);
  extOffset = (int*) malloc(sizeof(int) * extBoard->cols);
  if (gameState) {
    // get boardCandies and boardState from gameState, moves, score, offset
    json_t *bCandies = json_object_get(gameState, "boardcandies");
    json_t *jrows = json_object_get(bCandies, "rows");
    json_t *jcols = json_object_get(bCandies, "columns");
    int rows = json_integer_value(jrows);
    int cols = json_integer_value(jcols);
    //json_decref(jrows);
    //json_decref(jcols);

    boardCandies = Array2D_create(rows, cols);
    json_t *data = json_object_get(bCandies, "data");
    json_t *el;
    for (int i = 0; i < (rows * cols); i++) {
      el = json_array_get(data, (size_t) i);
      json_t *num = json_object_get(el, "color");
      int *n = (int*) malloc(sizeof(int));
      *n = json_integer_value(num);
      Array2D_set(boardCandies, i/cols, i%cols, n);
    }
    //json_decref(data);
    //json_decref(bCandies);
    
    json_t *bState = json_object_get(gameState, "boardstate");
    boardState = deserializeF(bState);
    json_t *jmoves = json_object_get(gameState, "movesmade");
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
    boardState = deserializeF(bState);
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
  }
}

int Info_Container::GetColor(const int row, const int col) {
  int *result = (int*) Array2D_get(boardCandies, row, col);
  return *result;
}

int Info_Container::GetState(const int row, const int col) {
  int *result = (int*) Array2D_get(boardState, row, col);
  return *result;
}

int Info_Container::GetRows() {
  return boardCandies->rows;
}

int Info_Container::GetCols() {
  return boardCandies->cols;
}

int Info_Container::GetMoves() {
  return moves;
}

int Info_Container::GetScore() {
  return score;
}

Info_Container::~Info_Container() {
  Array2D_destroy(boardCandies, (Array2DDataFreeFnPtr) &free);
  Array2D_destroy(boardState, (Array2DDataFreeFnPtr) &free);
  Array2D_destroy(extBoard, (Array2DDataFreeFnPtr) &free);
}
