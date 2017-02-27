#include <iostream>

#include "MessageHandler.h"
#include "Message.h"
#include "ClientSocket.h"

extern "C" {
  #include <string.h>
  #include <jansson.h>
}

using namespace std;

MessageHandler::MessageHandler(int fd) : cs(fd) {}

Message MessageHandler::GetNextMessage() {
  // wait for a message from the other socket, decode the action, do it
  char buf[1024];
  int bracketCount = 1;
  int place = 1;
  int readCount;
  while ((readCount = cs.WrappedRead(buf, 1023))) {
    while (bracketCount > 0) {
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
    if (!strcmp(type, "hello")) {
      HelloMessage msg(buf);
      return msg;
    } else if (!strcmp(type, "helloack")) {
      HelloackMessage msg(buf);
      return msg;
    } else if (!strcmp(type, "move")) {
      MoveMessage msg(buf);
      return msg;
    } else if (!strcmp(type, "update")) {
      UpdateMessage msg(buf);
      return msg;
    } else if (!strcmp(type, "bye")) {
      ByeMessage msg(buf);
      return msg;
    } else {
      Message msg(buf);
      return msg;
    }
  }
  Message msg("error");
  return msg;
}

void MessageHandler::SendMessage(Message msg) {
  // encode the message and send it to the other socket
  string strMsg = msg.GetData();
  cs.WrappedWrite(strMsg.c_str(), strMsg.length());
}
