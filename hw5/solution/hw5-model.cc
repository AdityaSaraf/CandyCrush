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
    cout << hellomsg.GetType() << endl;
    msgh.SendMessage(hellomsg);
    Message helloackmsg = msgh.GetNextMessage();
    cout << helloackmsg.GetData() <<endl;
    game.Init(helloackmsg.GetData().c_str());
  } catch (string errString) {
    cerr << errString << endl;
    return 0;
  }

  return 1;
}
