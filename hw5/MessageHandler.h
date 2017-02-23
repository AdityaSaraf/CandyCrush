#ifndef MESSAGEM_H
#define MESSAGEM_H

#include "socketCode/ClientSocket.h"
#include "Message.h"

// MessageHandler will receive messages from the socket and decode them
// and then send a responding message

class MessageHandler {
 public:
  MessageHandler(hw5_net::ClientSocket csocket);
  void GetNextMessage();
  void SendMessage(Message msg);

 private:
  hw5_net::ClientSocket cs;
};

#endif // MESSAGEM_H
