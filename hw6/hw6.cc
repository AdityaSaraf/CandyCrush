// this file will be the model, so basically a wrapper file
// for the client socket and the methods in Game.cc
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

#include "../hw5/socketCode/ClientSocket.h"
#include "../hw5/solution/MessageHandler.h"
#include "../hw5/solution/Message.h"
#include "Game.h"

extern "C" {
  #include <string.h>
  #include <jansson.h>
}

Game game;

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

    while (1) {
      msg = msgh.GetNextMessage();
      if (msg.GetType() == "requestmove") {
        // find best move
        // searcher.getBestMove();
        // this has action, teamname and gameinstance
        string initialStr = game.SerializeCurrentState();

        json_error_t error;

        json_auto_t *root = json_loads(initialStr.c_str(), 0, &error);
        // set move
        json_t *move = json_string(/*call some function that serializes move returned from searcher*/"");
        json_object_set(root, "move", move);
        // set movesevaluated
        json_t *moves = json_integer(/*searcher.getMovesEvaluated()*/0);
        json_object_set(root, "movesevaluated", moves);

        char *jresult = json_dumps(root, 0);
        std::string MyMoveStr(jresult);
        free(jresult);

        MyMoveMessage myMoveMessage(MyMoveStr);
        msgh.SendMessage(myMoveMessage);
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
