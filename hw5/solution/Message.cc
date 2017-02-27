#include <string>

#include "Message.h"

using namespace std;

Message::Message(const char *msg) {
  data = msg;
}

Message::Message(const Message& msg) {
  typeStr = msg.typeStr;
  data = msg.data;
}

string Message::GetData() {
  return data;
}

string Message::GetType() {
  return typeStr;
}

HelloMessage::HelloMessage(const char *msg) : Message(msg) {
  typeStr = "hello";
}

HelloackMessage::HelloackMessage(const char *msg) : Message(msg) {
  typeStr = "helloack";
}

MoveMessage::MoveMessage(const char *msg) : Message(msg) {
  typeStr = "move";
}

UpdateMessage::UpdateMessage(const char *msg) : Message(msg) {
  typeStr = "update";
}

ByeMessage::ByeMessage(const char *msg) : Message(msg) {
  typeStr = "bye";
}
