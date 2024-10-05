#ifndef NETWORK_H
#define NETWORK_H
#include "user.h"

class Network {
public:
	//pre: None
	//post: returns a pointer to the corresponding User else nullptr if user is invalid
	User* getUser(int id);
	//pre: None
	//post: add a new user to the network database
	void addUser(User*);
	//pre: None
	//post: add connection between two users. Return -1 if user is invalid else 0
	int addConnection(std::string s1, std::string s2);
	//pre: None
	//post: delete the friend connection if it exists and return 0 else return -1
	int deleteConnection(std::string s1, std::string s2);
	//pre: None
	//post: return User's id and 0 if invalid
	int getId(std::string name);
	//pre: None
	//post: return the number of Users in the Network
	int numUsers();
	//pre: None
	//post: initialize all the network's information by reading in from a file
	void readUsers(char* fname);
	//pre: None
	//post: write all the network's information to another file
	void writeUsers(char* fname);
private:
	std::vector<User*> users_;
};

#endif