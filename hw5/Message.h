#ifndef MESSAGE_H
#define MESSAGE_H

// MessageHandler will create a message of a certain type
// and each type will have specific data to encode

class Message {
 public:
  Message(char *msg, int length);
  char *Serialize(int *retLength);

 protected:
};

class HelloMessage : Message {
 public:

 private:
  // whatever data a hello message needs
};

class HelloackMessage : Message {
 public:

 private:
  // whatever data a helloack message needs
};

class MoveMessage : Message {
 public:
  
 private:
  // whatever data a move message needs
};

class UpdateMessage : Message {
 public:

 private:
  // whatever data an update message needs
};

class ByeMessage : Message {
 public:

 private:
  // whatever data a bye message needs
};

#endif // MESSAGE_H
