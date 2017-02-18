#include "Game.h"

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
  if (true) {
    // swap
    // settle
  }
}

void Game::Settle() {
  bool fired = true;
  while (fired) {
    fired = false;
    for (int t = 0; t < 4; t++) {
      for (int i = 0; i < boardCandies->rows; i++) {
        for (int j = 0; j < boardCandies->cols; j++) {
          if (true) {
            // fire the template, make fired squares -1
            // update the boolean to true
          }
        }
      }
    }
    // apply gravity
  }
}

bool Game::MatchTemplate(const int row, const int col, const int t) {
  // if 0, match 2 above & 1 below or 2 below & 1 above
  // if 1, match 2 left & 1 right or 2 right & 1 left
  // if 2, match 2 above or 1 above & 1 below or 2 below
  // if 3, match 2 left or 1 left & 1 right or 2 right
  return false;
}

void Game::ApplyGravity() {
  for (int i = 0; i < boardCandies->rows; i++) {
    for (int j = 0; j < boardCandies->cols; j++) {
      // if -1, get the one above and set to -1; if one above -1,
      // get the next one, etc; if we go below 0, get from extension
      // and increase extension offset
    }
  }
}

Game::~Game() {
  Array2D_destroy(boardCandies, (Array2DDataFreeFnPtr) &free);
  Array2D_destroy(boardState, (Array2DDataFreeFnPtr) &free);
  Array2D_destroy(extBoard, (Array2DDataFreeFnPtr) &free);
}
