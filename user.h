#ifndef USER_H
#define USER_H

#include <set>
#include <string>
#include "post.h"

class User {
public:
	//pre: None
	//post: Create a default User
	User();
    ~User();
	//pre: None
	//post: Create a User with all the parameters
	User(int id, std::string name, int year, int zip, std::set<int> friends);
	//pre: None
	//post: add friend to the set
	//pre: None
	//post: get Name from private field
	std::string getName();
	//pre: None
	//post: get ID from private field
	int getId() const;
	//pre: None
	//post: get year from private field
	int getYear() const;
	//pre: None
	//post: get Zip from private field
	int getZip() const;
	//pre: None
	//None: get Friends from private field
	std::set<int>& getFriends();
	void addFriend(int id);
	//pre: None
	//post: delete friend from set
	void deleteFriend(int id);
	//pre: None
	//post: Help remove whitespace from getName()
	std::string trim(std::string& name);
	//pre: None
	//post: 
	void addPost(Post*);
	//pre: None
	//post: a post is added to user
	std::vector<Post*> getPosts();
	//pre: None
	//post:
    std::vector<std::string> getPostsStrings(int howMany, bool showOnlyPublic);
    //pre: None
    //post: get messages size
    int getSize();
    //pre: None
    //post: delete post from messages_
    void deletePost(int i);
    void refreshPostsTable();
private:
	int id_;
	std::string name_;
	int year_;
	int zip_;
	std::set<int> friends_;
	std::vector<Post*> messages_;
};

#endif
