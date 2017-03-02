// this file will be the model, so basically a wrapper file
// for the client socket and the methods in Game.cc
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

#include "ClientSocket.h"
#include "MessageHandler.h"
#include "Message.h"
#include "../../hw4/Game.h"

extern "C" {
  #include <string.h>
  #include <jansson.h>
}

Game game;

void deserializeAndApplyMove(const char *data) {
  json_t *root;
  json_error_t error;
  root = json_loads(data, 0, &error);
  json_t *jrow = json_object_get(root, "row");
  json_t *jcol = json_object_get(root, "column");
  json_t *jdir = json_object_get(root, "direction");
  int row = game.GetRows() - json_integer_value(jrow) - 1;
  int col = json_integer_value(jcol);
  int dir = json_integer_value(jdir);
  json_decref(jrow);
  json_decref(jcol);
  json_decref(jdir);
  if (dir == 0) game.Swap(row, col, row, col - 1);
  else if (dir == 1) game.Swap(row, col, row, col + 1);
  else if (dir == 2) game.Swap(row, col, row - 1, col);
  else if (dir == 3) game.Swap(row, col, row + 1, col);
  json_decref(root);
}

void usage(const char *exeName) {
  cout << "Usage: " << exeName << " <hostname> <port>" << endl;
}

int main(int argc, char **argv) {
  
  if (argc != 3) 
  {
    usage(argv[0]);
    return EXIT_FAILURE;
  }

  int serverPort;
  try {
    serverPort = stoi(argv[2]);
  } catch (...) {
    usage(argv[0]);
    return 1;
  }

  try {
    string serverName(argv[1]);
    hw5_net::ClientSocket csock(serverName, serverPort);
    MessageHandler msgh(csock.getAsFileDescriptor());
    HelloMessage hellomsg("{\"action\": \"hello\"}");
    msgh.SendMessage(hellomsg);
    Message msg = msgh.GetNextMessage();
    game.Init(msg.GetData().c_str());
    string updateStr = game.SerializeCurrentState();
    UpdateMessage update(updateStr);
    msgh.SendMessage(update);
    while (1) {
      msg = msgh.GetNextMessage();
      if (msg.GetType() == "move") {
        deserializeAndApplyMove(msg.GetData().c_str());
        updateStr = game.SerializeCurrentState();
        // send update to view
        UpdateMessage updatemsg(updateStr);
        msgh.SendMessage(updatemsg);
      }
      if (msg.GetType() == "bye") {
        return 1;
      }
    }
  } catch (string errString) {
    cerr << errString << endl;
    return 0;
  }

  return 1;
}
