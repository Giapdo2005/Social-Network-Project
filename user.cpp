#include <iostream>
#include <set>
#include <string>
#include "user.h"
#include "post.h"

User::User() {
	id_ = 0;
	name_ = "";
	year_  = 0;
	zip_ = 0;
	friends_.clear();
}

User::~User() {
    for (Post* post : messages_) {
        delete post;
    }
    messages_.clear();
}

User::User(int id, std::string name, int year, int zip, std::set<int> friends) {
	id_ = id;
	name_ = name;
	year_ = year;
	zip_ = zip;
	friends_ = friends;
}

std::string User::getName() {
	return trim(name_);
}

int User::getId() const {
	return id_;
}

int User::getYear() const {
	return year_;
}

int User::getZip() const {
	return zip_;
}

std::set<int>& User::getFriends() {
	return friends_;
}

void User::addFriend(int id) {
	bool found = false;
	for (std::set<int>::iterator it = friends_.begin(); it != friends_.end(); it++) {
		if (id == *it) {
			found = true;
			break;
		}
	}
	if (!found) {
		friends_.insert(id);
	}
}

void User::deleteFriend(int id) {
	bool found = false;
	for (std::set<int>::iterator it = friends_.begin(); it != friends_.end(); it++) {
		if (id == *it) {
			friends_.erase(id);
			break;
		}
	}
}

std::string User::trim(std::string& name) {
    std::size_t start = 0; 
    std::size_t end = name.size(); 

    while (start < end && std::isspace(name[start])) {
        ++start;
    }

    while (end > start && std::isspace(name[end - 1])) {
        --end;
    }

    return name.substr(start, end - start);
}

void User::addPost(Post* post) {
    auto it = std::lower_bound(messages_.begin(), messages_.end(), post, [](Post* a, Post* b) {
        return a->getMessageId() < b->getMessageId();
    });
    messages_.insert(it, post);
}

std::vector<Post*> User::getPosts() {
    return messages_;
}

std::vector<std::string> User::getPostsStrings(int howMany, bool showOnlyPublic) {
    std::vector<std::string> posts;
    int count = 0;
    for (int i = messages_.size() - 1; i >= 0 && count < howMany; --i) {
        if (!showOnlyPublic || messages_[i]->getIsPublic()) {
            posts.push_back(messages_[i]->toString());
            count++;
        }
    }

    return posts;
}

int User::getSize() {
    return messages_.size();
}


void User::deletePost(int i) {
    if (i < 0 || i >= messages_.size()) {
        std::cout << "Index out of bound: " << i << std::endl;
        return;
    }
    std::cout << messages_[i]->getMessage() << std::endl;
    std::cout << messages_[i]->getMessageId() << std::endl;
    delete messages_[i];
    messages_.erase(messages_.begin() + i);
}
