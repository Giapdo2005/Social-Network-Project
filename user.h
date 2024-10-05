#ifndef USER_H
#define USER_H

#include <set>
#include <string>

class User {
public:
	//pre: None
	//post: Create a default User
	User();
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

private:
	int id_;
	std::string name_;
	int year_;
	int zip_;
	std::set<int> friends_;
};

#endif
