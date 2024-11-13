#ifndef NETWORK_H
#define NETWORK_H
#include "user.h"

class Network {
public:
	//default constructor
	Network();
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
	//pre: None
	//post: return a vector of users on the shortest path
	std::vector<int> shortestPath(int from, int to);
	//pre: None
	//post: return a vector of users_ids on the path if that path exists else returns an empty vector and set to to -1
	std::vector<int> distanceUser(int from, int& to, int distance);
	//pre: None
	//post: return a vector of suggest friends if found else return empty vector and set score to 0
	std::vector<int> suggestFriends(int who, int& score);
	//pre: None
	//post: return a list of lists of connected users
	std::vector<std::vector<int> > groups(); 
	int diameter();
	//pre: None
	//post: add a post to the users_ vector
	void addPost(int ownerId, std::string message, int likes, bool isInComing, std::string authorName, bool isPublic);
	//pre: None
	//post: get posts from a user
    std::vector<std::string> getPostsString(int ownerId, int howMany, bool showOnlyPublic);
	//pre: None
	//post: 
	int readPosts(char* fname);
    //pre: None
    //post: write posts to file
	int writePosts(char* fname);
	std::string trim(std::string& data);
	static bool compare(Post* post1, Post* pos2);
    //pre: None
    //post: delete a post from the user's post
    void deletePost(int userId, int i);
private:
	std::vector<User*> users_;
	int messageIdCounter;
};

#endif
