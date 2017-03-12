#include <iostream>
#include <string>

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
  char buf[50000];
  int bracketCount = 0;
  int place = 0;
  int readCount;
  string strBuf("");
  while ((readCount = cs.WrappedRead(buf, 49999))) {
    place = 0;
    while ((bracketCount > 0 || place == 0) && place < 50000 && readCount > 0) {
      if (buf[place] == '{') bracketCount++;
      if (buf[place] == '}') bracketCount--;
      readCount--;
      place++;
    }
    if (place == 50000)
    {
      throw "Error! Message from server was too long!";
    }
    buf[place] = '\0';
    json_t *root;
    json_error_t error;
    strBuf += buf;
    root = json_loads(strBuf.c_str(), 0, &error);
    if (root) {
      json_t *jtype = json_object_get(root, "action");
      const char *type = json_string_value(jtype);
      if (!strcmp(type, "hello")) {
        HelloMessage msg(strBuf);
        return msg;
      } else if (!strcmp(type, "helloack")) {
        HelloackMessage msg(strBuf);
        return msg;
      } else if (!strcmp(type, "move")) {
        MoveMessage msg(strBuf);
        return msg;
      } else if (!strcmp(type, "update")) {
        UpdateMessage msg(strBuf);
        return msg;
      } else if (!strcmp(type, "bye")) {
        ByeMessage msg(strBuf);
        return msg;
      } else if (!strcmp(type, "requestmove")) {
        RequestMoveMessage msg(strBuf);
        return msg;
      } else {
        Message msg(strBuf);
        return msg;
      }
    }
  }
  Message msg(strBuf);
  return msg;
}

void MessageHandler::SendMessage(Message msg) {
  // encode the message and send it to the other socket
  string strMsg(msg.GetData());
  cs.WrappedWrite(strMsg.c_str(), strMsg.length());
}
