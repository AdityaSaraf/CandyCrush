// this file will be the view, so it will implement the gtk things
// with the callback functions being mainly message sending
#include <iostream>
#include <cstdlib>

using namespace std;

#include "ServerSocket.h"
#include "ClientSocket.h"
#include "MessageHandler.h"
#include "Message.h"

void usage(const char *name) {
  cout << "Usage: " << name << " file [port]" << endl;
  cout << "  Gives the file to connecting models " << endl;
  cout << "  to initialize from. Uses a random " << endl;
  cout << "  if not given one. " << endl;
  exit(1);
}

int main(int argc, char **argv) {
  // make a socket
  // deserialize the json
  // wait for a connection
  // send the gamestate to model
  // start gtk upon getting the update from the model
  
  if (argc != 2 && argc != 3) usage(argv[0]);

  int port = 0;
  try {
    if (argc == 2) port = stoi(argv[2]);
  } catch (...) {
    usage(argv[0]);
  }

  try {
    int socketFd;
    hw5_net::ServerSocket ssock(port);
    ssock.BindAndListen(AF_INET, &socketFd);
    cout << "Created bound socket on port = " << ssock.port() << endl;
    
    while (1) {
      int acceptedFd;
      string clientAddr;
      uint16_t clientPort;
      string clientDNSName;
      string serverAddress;
      string serverDNSName;

      ssock.Accept(&acceptedFd,
                   &clientAddr,
                   &clientPort,
                   &clientDNSName,
                   &serverAddress,
                   &serverDNSName);

      hw5_net::ClientSocket csock(acceptedFd);

    }

  } catch (std::string strError) {
    std::cout << strError << std::endl;
    return EXIT_FAILURE;
  }

  return 1;
}
