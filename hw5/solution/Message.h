#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>

// MessageHandler will create a message of a certain type
// and each type will have specific data to encode

using namespace std;

class Message {
 public:
  Message(const char *msg);
  Message(const Message& msg);
  string GetData();
  string GetType();

 protected:
  string typeStr;  
  string data;
};

class HelloMessage : public Message {
 public:
  HelloMessage(const char *msg);
};

class HelloackMessage : public Message {
 public:
  HelloackMessage(const char *msg);
};

class MoveMessage : public Message {
 public:
  MoveMessage(const char *msg);
};

class UpdateMessage : public Message {
 public:
  UpdateMessage(const char *msg);
};

class ByeMessage : public Message {
 public:
  ByeMessage(const char *msg);
};

#endif // MESSAGE_H
