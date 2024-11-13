#include <iostream>
#include <vector>
#include <cstddef>
#include <fstream>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>
#include "network.h"
#include "user.h"

Network::Network() {
	messageIdCounter = 0;
}

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
		//std::cout << "File opened successfully" << std::endl;
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
		//std::cout << "File opened successfully" << std::endl;

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

std::vector<int> Network::shortestPath(int from, int to) {
	std::queue<int> q;
	std::vector<bool> visited (users_.size(), false);
	std::vector<int> prev (users_.size(), - 1);

	visited[from] = true;
	q.push(from);

	while (q.size() > 0) {
		int cur = q.front();
		q.pop();

		for (int neighbor : users_[cur]->getFriends()) {
			if (!visited[neighbor]) {
				prev[neighbor] = cur;
				visited[neighbor] = true;
				q.push(neighbor);
			}
		}
	}

	std::vector<int> path;
	int cur = to;
	while (cur != -1) {
		path.push_back(cur);
		cur = prev[cur];
	}
	reverse(path.begin(), path.end());
	return path;
}

std::vector<int> Network::distanceUser(int from, int& to, int distance) {
	std::queue<int> q;
	std::vector<bool> visited (users_.size(), false);
	std::vector<int> prev (users_.size(), -1);

	visited[from] = true;
	q.push(from);

	std::vector<int> userAtDistance;

	while (q.size() > 0 && distance >= 0) {
		int size = q.size();
		for (int i = 0; i < size; i++) {
			int cur = q.front();
			q.pop();
			if (distance == 0) {
				to = cur;
				userAtDistance.push_back(cur);
				std::vector<int> path;
				while (cur != -1) {
					path.push_back(cur);
					cur = prev[cur];
				}
				reverse(path.begin(), path.end());
				return path;
			}
			else {
				for (int neighbor : users_[cur]->getFriends())
					if (!visited[neighbor]) {
						prev[neighbor] = cur;
						visited[neighbor] = true;
						q.push(neighbor);
					}
			}
		}
		distance--;
	}
	to = -1;
	return {};
}

std::vector<int> Network::suggestFriends(int who, int& score) {
    std::set<int> whosFriends = users_[who]->getFriends();
    std::vector<int> user_score (users_.size(), 0);

    for (int friendId : whosFriends) {
        for (int mutualFriendId : users_[friendId]->getFriends()) {
            if (mutualFriendId != who && whosFriends.find(mutualFriendId) == whosFriends.end()) {
                user_score[mutualFriendId]++;
            }
        }
    }

    std::vector<int> suggestedFriends;
    score = 0;
    for (std::size_t i = 0; i < user_score.size(); i++) {
        if (user_score[i] > score) {
            score = user_score[i];
            suggestedFriends.clear();
            suggestedFriends.push_back(i);
        } else if (user_score[i] == score && score > 0) {
            suggestedFriends.push_back(i);
        }
    }
    return suggestedFriends;
}

std::vector<std::vector<int> > Network::groups() {
	std::stack<int> s;
	std::vector<bool> visited (users_.size(), false);
	std::vector<std::vector<int> > groups;

	for (std::size_t i = 0; i < users_.size(); i++) {
		std::vector<int> group;
		if (!visited[i]) {
			s.push(i);
			visited[i] = true;
		}
		while (s.size() > 0) {
			int cur = s.top();
			s.pop();
			group.push_back(cur);
			for (int neighbor : users_[cur]->getFriends()) {
				if (!visited[neighbor]) {
					visited[neighbor] = true;
					s.push(neighbor);
				}
			}
		}
		if (!group.empty()) {
			groups.push_back(group);
		}
	}
	return groups;
}

int Network::diameter() {
	int diameter = 0;
  for (std::size_t from = 0; from < users_.size(); from++) {
		std::vector<bool> visited (users_.size(), false);
		std::vector<int> dist(users_.size(), -1);
		std::queue<int> q;

    visited[from] = true;
    q.push(from);
    dist[from] = 0;
    while (q.size() > 0) {
      int cur = q.front();
      q.pop();
      for (int neighbor : users_[cur]->getFriends()) {
				if (!visited[neighbor]) {
					dist[neighbor] = dist[cur] + 1;
					visited[neighbor] = true;
					q.push(neighbor);
				}
			}
    }
		for (std::size_t i = 0; i < users_.size(); i++) {
			if (dist[i] == -1) return -1;
			diameter = std::max(diameter, dist[i]);
		}
  }
	return diameter;
}

std::string Network::trim(std::string& data) {
    std::size_t start = 0; 
    std::size_t end = data.size(); 

    while (start < end && std::isspace(data[start])) {
        ++start;
    }

    while (end > start && std::isspace(data[end - 1])) {
        --end;
    }

    return data.substr(start, end - start);
}


void Network::addPost(int ownerId, std::string message, int likes, bool isInComing, std::string authorName, bool isPublic) {
	int messageId = messageIdCounter++;
	if (isInComing) {
		IncomingPost* post = new IncomingPost(messageId, ownerId, message, likes, isPublic, authorName);
		users_[ownerId]->addPost(post);
	}
	else {
		Post* post = new Post(messageId, ownerId, message, likes);
		users_[ownerId]->addPost(post);
	}
}

std::vector<std::string> Network::getPostsString(int ownerId, int howMany, bool showOnlyPublic) {
	return users_[ownerId]->getPostsStrings(howMany, showOnlyPublic);
}


int Network::readPosts(char* fname) {
	std::ifstream fileIn(fname);

	if (!fileIn) {
		return -1;
	}

	std::cout<< fname << std::endl;
	std::string line;
	int size;
	if (!getline(fileIn, line)) {
		std::cout<< "Can't read size" << std::endl;
		return -1;
	}
	else {
		std::cout << "Size: " << line << std::endl;
		size = stoi(line);
	}
    int messageId, maxMessageId = 0;
	std::string message;
	int ownerId;
	int likes;
	std::string author;
	for (std::size_t i = 0; i < size; i++) {
		//read messageId
		if (getline(fileIn, line)) {
			messageId = stoi(line);
            if (messageId > maxMessageId) maxMessageId = messageId;
		} else {
			std::cout << "Error reading in messageId" << std::endl;
			continue;
		}
		//read message text
		if (getline(fileIn, line)) {
            message = trim(line);
		} else {
			std::cout << "Error reading in message" << std::endl;
			continue;
		}
		//read ownerId
		if (getline(fileIn, line)) {
			ownerId = stoi(line);
		} else {
			std::cout << "Error reading in ownerId" << std::endl;
			continue;
		}
		//read likes
		if (getline(fileIn, line)) {
			likes = stoi(line);
		} else {
			std::cout << "Error reading in likes" << std::endl;
			continue;
		}
		//read public or private or empty line then have another conditional if needed
		if (getline(fileIn, line)) {
			if (trim(line) == "public" || trim(line) == "private") {
				bool isPublic = (trim(line) == "public");

				if (getline(fileIn, line)) {
						author = line;
				} else {
						std::cout << "Error getting author" << std::endl;
						continue;
				}

				IncomingPost* post = new IncomingPost(messageId, ownerId, message, likes, isPublic, trim(author));
				users_[ownerId]->addPost(post);
			} 
			else {
				Post* post = new Post(messageId, ownerId, message, likes);
				users_[ownerId]->addPost(post);
				getline(fileIn, line); 
			}
			}
		}
    messageIdCounter = maxMessageId + 1;
	return 0;
}

int Network::writePosts(char* fname) {
	std::ofstream fileOut(fname);
	if (fileOut.is_open()) {
		std::cout << "File written to successfully" << std::endl;
		std::cout << fname << std::endl;

		std::vector<Post*> allPosts;

		for (std::size_t i = 0; i < users_.size(); i++) {
			if (!users_[i]) {
				std::cerr << "Error: User " << i << " is null" << std::endl;
				continue;
      }
			std::vector<Post*> posts = users_[i]->getPosts();
			if (posts.empty()) {
				std::cout << "No posts for user " << i << std::endl;
				continue;
			}

			allPosts.insert(allPosts.end(), posts.begin(), posts.end());
		}

		std::cout << allPosts.size() << std::endl;
 		fileOut << allPosts.size() << std::endl;
		std::sort(allPosts.begin(), allPosts.end(), compare);

		for (Post* post: allPosts) {
			if (!post) {
                std::cerr << "Error: Post is null" << std::endl;
                continue;
            }

			fileOut << post->getMessageId() << std::endl;
			fileOut << "\t" << post->getMessage() << std::endl;
            fileOut << "\t" << post->getOwnerId() << std::endl;
			fileOut << "\t" << post->getLikes() << std::endl;


			if (!post->getAuthor().empty()) { 
				fileOut << "\t" << (post->getIsPublic() ? "public" : "private") << std::endl;
				fileOut << "\t" << post->getAuthor() << std::endl;
			} else {
				fileOut << "\n\n";
			}
		}
	}
	return 0;
}

bool Network::compare(Post* post1, Post* post2) {
	return post1->getMessageId() < post2->getMessageId();
}

void Network::deletePost(int userId, int i) {
    users_[userId]->deletePost(i);
}


