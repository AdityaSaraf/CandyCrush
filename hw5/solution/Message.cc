#include <string>

#include "Message.h"

using namespace std;

Message::Message(string msg) : data(msg) {
}

Message::Message(const Message& msg) : typeStr(msg.typeStr), data(msg.data) {
}

string Message::GetData() {
  return data;
}

string Message::GetType() {
  return typeStr;
}

HelloMessage::HelloMessage(string msg) : Message(msg) {
  typeStr = "hello";
}

HelloackMessage::HelloackMessage(string msg) : Message(msg) {
  typeStr = "helloack";
}

MoveMessage::MoveMessage(string msg) : Message(msg) {
  typeStr = "move";
}

UpdateMessage::UpdateMessage(string msg) : Message(msg) {
  typeStr = "update";
}

ByeMessage::ByeMessage(string msg) : Message(msg) {
  typeStr = "bye";
}
