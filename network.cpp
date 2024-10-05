#include <iostream>
#include <vector>
#include <cstddef>
#include <fstream>
#include <set>
#include <string>
#include <sstream>
#include "network.h"
#include "user.h"

User* Network::getUser(int id) {
	for (std::size_t i = 0; i < users_.size(); i++) {
		if (id == i) {
			return users_[i];
		}
	}
	return nullptr;
}

void Network::addUser(User* user) {
	users_.push_back(user);
	std::cout << "Added user: " << user->getName() << "with ID: " << user->getId() << std::endl;
}

int Network::addConnection(std::string s1, std::string s2) {
	int user1_id = -1;
	int user2_id = -1;
	for (std::size_t i = 0; i < users_.size(); i++) {
		if (s1 == users_[i]->getName()) {
			 user1_id = users_[i]->getId();
		}
		if (s2 == users_[i]->getName()) {
			user2_id = users_[i]->getId();
		}
		if (user1_id != -1 && user2_id != -1){
			break;
		}
	}
	if (user1_id == -1 || user2_id == -1) {
		return -1;
	}
	else {
		if (users_[user1_id]->getFriends().find(user2_id) == users_[user1_id]->getFriends().end()) {
			users_[user1_id]->addFriend(user2_id);
			users_[user2_id]->addFriend(user1_id);
			return 0; //return 0 to indicate that connection was succesfully made
		}
		else{
			return 1; // return 1 if the connection already exists
		}
	}
}

int Network::deleteConnection(std::string s1, std::string s2) {
	int user1_id = -1;
	int user2_id = -1;
	for (std::size_t i = 0; i < users_.size(); i++) {
		if (s1 == users_[i]->getName()) {
			user1_id = users_[i]->getId();
		}
		if (s2 == users_[i]->getName()) {
			user2_id = users_[i]->getId();
		}
		if (user1_id != -1 && user2_id != -1){
			break;
		}
	}
	if (user1_id == -1 || user2_id == -1) {
		return -1;
	}
	else {
		if (users_[user1_id]->getFriends().find(user2_id) == users_[user1_id]->getFriends().end()) {
			return 1; //return 1 if the connection doesn't exist
		}
		else {
			users_[user1_id]->deleteFriend(user2_id);
			users_[user2_id]->deleteFriend(user1_id);
			return 0; //return 0 if the deletion was made successfully
		}
	}
}

int Network::getId(std::string name) {
	for (std::size_t i = 0; i < users_.size(); i++) {
		if( name == users_[i]->getName()) {
			return users_[i]->getId();
		}
	}
	return -1; //return -1 if no user exists
}

int Network::numUsers() {
	return users_.size();
}


void Network::readUsers(char* fname) {
	std::ifstream fileIn(fname);
	int size;
	std::string line;

	if(fileIn.is_open()) {
		std::cout << "File opened successfully" << std::endl;
		if (!(getline(fileIn, line))) {
			std::cout << "Failed to read size" << std::endl;
			return;
		} else {
			size = stoi(line);
		}
	};
	

	for (std::size_t i = 0; i < size; i++) {
		int id;
		std::string name;
		int year;
		int zip;
		std::set<int> friends;
		std::string line;

		//Read ID
		if (std::getline(fileIn, line)) {
			try {
				id = std::stoi(line);
			} catch (const std::exception& e) {
	            std::cerr << "Error converting ID: " << e.what() << " for line: " << line << std::endl;
	            continue; // Skip to the next user
	        }
        }

        //Read Name
        if (std::getline(fileIn, line)) {
        	try {
        		name = line;
        	} catch (const std::exception& e) {
            	std::cerr << "Error converting name: " << e.what() << " for line: " << line << std::endl;
            	continue; // Skip to the next user
            }
        }

        //Read year
        if (std::getline(fileIn, line)) {
        	try {
        		year = std::stoi(line);
        	} catch (const std::exception& e) {
            	std::cerr << "Error converting year: " << e.what() << " for line: " << line << std::endl;
            	continue; // Skip to the next user
            }
        }

        //Read zip 
        if (std::getline(fileIn, line)) {
        	try {
        		zip = stoi(line);
        	} catch (const std::exception& e) {
	            std::cerr << "Error converting zip: " << e.what() << " for line: " << line << std::endl;
	            continue; // Skip to the next user
	        }
        }

        //Read friends
        if (std::getline(fileIn, line)) {
        	std::stringstream fs(line);
        	int val;
        	while (fs >> val) {
        		friends.insert(val);
        	}
        }
        User* user = new User(id, name, year, zip, friends);
    	users_.push_back(user);
 	}
 	
 	std::string name = getUser(0)->getName();
 	
 	fileIn.close();
}


void Network::writeUsers(char* fname) {
	std::ofstream fileOut(fname);
	if (fileOut.is_open()) {
		std::cout << "File opened successfully" << std::endl;

		fileOut << users_.size() << std::endl;
		int size;
		for(std::size_t i = 0; i < users_.size(); i++) {
			fileOut << users_[i]->getId() << std::endl;
			fileOut << "\t" << users_[i]->getName() << std::endl;
			fileOut << "\t" << users_[i]->getYear() << std::endl;
			fileOut << "\t" << users_[i]->getZip() << std::endl;
			const std::set<int> friends = users_[i]->getFriends();
			fileOut << "\t"; //tab before the friends list;
			for (auto it = friends.begin(); it != friends.end(); it++) {
				fileOut << *it << " ";
			}
			fileOut << std::endl;
			
		}
	}

	else {
		std::cout << "Error opening file" << std::endl;
	}

	fileOut.close();
}

