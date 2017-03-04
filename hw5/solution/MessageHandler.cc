#include <iostream>

#include "MessageHandler.h"
#include "Message.h"
#include "ClientSocket.h"

extern "C" {
  #include <string.h>
  #include <jansson.h>
}

using namespace std;

MessageHandler::MessageHandler() : cs(0) {}

MessageHandler::MessageHandler(int fd) : cs(fd){}

Message MessageHandler::GetNextMessage() {
  // wait for a message from the other socket, decode the action, do it
  char buf[5000];
  int bracketCount = 1;
  int place = 1;
  int readCount;
  while ((readCount = cs.WrappedRead(buf, 4999))) {
    while (bracketCount > 0 && place < 5000) {
      if (buf[place] == '{') bracketCount++;
      if (buf[place] == '}') bracketCount--;
      place++;
    }
    buf[place] = '\0';
    json_t *root;
    json_error_t error;
    root = json_loads(buf, 0, &error);
    json_t *jtype = json_object_get(root, "action");
    const char *type = json_string_value(jtype);
    string data(buf);
    if (!strcmp(type, "hello")) {
      HelloMessage msg(data);
      return msg;
    } else if (!strcmp(type, "helloack")) {
      HelloackMessage msg(data);
      return msg;
    } else if (!strcmp(type, "move")) {
      MoveMessage msg(data);
      return msg;
    } else if (!strcmp(type, "update")) {
      UpdateMessage msg(data);
      return msg;
    } else if (!strcmp(type, "bye")) {
      ByeMessage msg(data);
      return msg;
    } else if (!strcmp(type, "requestmove")) {
      ByeMessage msg(data);
      return msg;
    } else {
      Message msg(data);
      return msg;
    }
  }
  string data(buf);
  Message msg(data);
  return msg;
}

void MessageHandler::SendMessage(Message msg) {
  // encode the message and send it to the other socket
  string strMsg(msg.GetData());
  cs.WrappedWrite(strMsg.c_str(), strMsg.length());
}
