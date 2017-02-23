#include "MessageHandler.h"
#include "Message.h"
#include "socketCode/ClientSocket.h"

MessageHandler::MessageHandler(hw5_net::ClientSocket csocket) : cs(csocket.getAsFileDescriptor()) {
 // nothing to do; the socket is intitialized above
}

void MessageHandler::GetNextMessage() {
  // wait for a message from the other socket, decode the action, do it
}

void MessageHandler::SendMessage(Message msg) {
  // encode the message and send it to the other socket
}
