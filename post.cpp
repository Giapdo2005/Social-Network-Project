#include "post.h"
#include <iostream>

Post::Post() {
  messageId_ = 0;
  ownerId_ = 0;
  message_ = "";
  likes_ = 0;
}

Post::Post(int messageId, int ownerId, std::string message, int likes) {
  messageId_ = messageId;
  ownerId_ = ownerId;
  message_ = message;
  likes_ = likes;
}

std::string Post::toString() {
  return message_ + " Liked by " + std::to_string(likes_) + " people.";
}

int Post::getOwnerId() {
  return ownerId_;
}

int Post::getMessageId() {
  return messageId_;
}

int Post::getLikes() {
  return likes_;
}

std::string Post::getMessage() {
  return message_;
}
bool Post::getIsPublic() {
  return true;
}
std::string Post::getAuthor() {
  return "";
}

IncomingPost::IncomingPost() : Post() {
  author_ = " ";
  isPublic_ = true;
}
IncomingPost::IncomingPost(int messageId, int ownerId, std::string message, int likes, bool isPublic, std::string author) : Post(messageId, ownerId, message, likes), author_(author), isPublic_(isPublic) {}


std::string IncomingPost::toString() {
  std::string String = Post::toString();
  std::string privacy = isPublic_ ? "" : " (private)";
  return author_ + " wrote" + privacy + ": " + String;
}

std::string IncomingPost::getAuthor() {
  return author_;
}

bool IncomingPost::getIsPublic() {
  return isPublic_;
}


