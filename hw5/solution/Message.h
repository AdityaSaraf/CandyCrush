#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>

// MessageHandler will create a message of a certain type
// and each type will have specific data to encode

using namespace std;

class Message {
 public:
  Message(string msg);
  Message(const Message& msg);
  string GetData();
  string GetType();

 protected:
  string typeStr;  
  string data;
};

class HelloMessage : public Message {
 public:
  HelloMessage(string msg);
};

class HelloackMessage : public Message {
 public:
  HelloackMessage(string msg);
};

class MoveMessage : public Message {
 public:
  MoveMessage(string msg);
};

class UpdateMessage : public Message {
 public:
  UpdateMessage(string msg);
};

class ByeMessage : public Message {
 public:
  ByeMessage(string msg);
};

#endif // MESSAGE_H
