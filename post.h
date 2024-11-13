#ifndef POST_H   
#define POST_H 
#include <iostream>

class Post {
private:
  int messageId_;
  int ownerId_;
  std::string message_;
  int likes_;
  bool isOwner;
public:
  Post();
  Post (int messageId, int ownerId, std::string message, int likes);
  std::string toString();
  int getMessageId();
  int getLikes();
  int getOwnerId();
  std::string getMessage();
  virtual std::string getAuthor();
  virtual bool getIsPublic();
};

class IncomingPost : public Post {
private:
  std::string author_;
  bool isPublic_;
public:
  IncomingPost();
  IncomingPost(int messageId, int ownerId, std::string message, int likes, bool isPublic, std::string author);
  std::string toString();
  std::string getAuthor();
  bool getIsPublic();
};

#endif