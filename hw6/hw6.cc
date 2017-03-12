// this file will be the model, so basically a wrapper file
// for the client socket and the methods in Game.cc
#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <atomic>

using namespace std;

#include "../hw5/socketCode/ClientSocket.h"
#include "../hw5/solution/MessageHandler.h"
#include "../hw5/solution/Message.h"
#include "Game.h"
#include "Searcher.h"

extern "C" {
  #include <string.h>
  #include <jansson.h>
}

void getBestMove(Game game, Searcher searcher) {
  searcher.GetBestMove(game);
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
    Game game;

    string serverName(argv[1]);
    hw5_net::ClientSocket csock(serverName, serverPort);
    MessageHandler msgh(csock.getAsFileDescriptor());

    HelloMessage hellomsg("{\"action\": \"hello\", \"teamname\": \"agrajag\"}");
    msgh.SendMessage(hellomsg);

    Message msg("");

    try {
      msg = msgh.GetNextMessage();
    }catch (const char* msg) {
      cerr << msg << endl;
      return 0;
    }
    game.Init(msg.GetData().c_str());

    Searcher searcher;
    thread th(getBestMove, game, searcher);
    
    // find the best move and wait for a requestmove message
    while (1) {
      msg = msgh.GetNextMessage();
      if (msg.GetType() == "requestmove") {
        Move m = Searcher::setDone();
        th.join();
        if (m.GetDirection() == -1) return 0;
        cout << m.GetRow() << ", " << m.GetCol() << ", " << m.GetDirection() << endl;
        game.PrintState();
        game.ApplyMove(m);
        
        json_error_t error;
        json_t *root = json_object();
        json_t *action = json_string("mymove");
        json_t *teamname = json_string("agrajag");
        string gameStr = game.SerializeCurrentState();
        json_t *gameinstance = json_loads(gameStr.c_str(), 0, &error);
        json_t *jmove = json_object();
        json_t *row = json_integer(m.GetRow());
        json_t *col = json_integer(m.GetCol());
        json_t *dir = json_integer(m.GetDirection());
        json_object_set_new(jmove, "row", row);
        json_object_set_new(jmove, "column", col);
        json_object_set_new(jmove, "direction", dir);
        int moves = Searcher::atomic_counter.load();
        json_t *jmoves = json_integer(moves);
        Searcher::atomic_counter = 0;

        th = thread(getBestMove, game, searcher);

        json_object_set_new(root, "action", action);
        json_object_set_new(root, "teamname", teamname);
        json_object_set_new(root, "gameinstance", gameinstance);
        json_object_set_new(root, "move", jmove);
        json_object_set_new(root, "movesevaluated", jmoves);
        char *msgcStr = json_dumps(root, 0);
        string msgStr(msgcStr);
        free(msgcStr);
        json_decref(root);
        MyMoveMessage mymovemsg(msgStr);
        msgh.SendMessage(mymovemsg);
      }
      if (msg.GetType() == "bye") {
        cout << msg.GetData() << endl;
        Searcher::setDone();
        th.join();
        break;
      }
    }
  } catch (string errString) {
    cerr << errString << endl;
    return 0;
  }
  return 1;
}
