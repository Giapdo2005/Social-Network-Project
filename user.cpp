#include <iostream>
#include <set>
#include <string>
#include "user.h"

User::User() {
	id_ = 0;
	name_ = "";
	year_  = 0;
	zip_ = 0;
	friends_.clear();
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
	 // Initialize start and end indices
    std::size_t start = 0; // Starting index
    std::size_t end = name.size(); // Ending index

    // Move the start index forward until we find a non-whitespace character
    while (start < end && std::isspace(name[start])) {
        ++start;
    }

    // Move the end index backward until we find a non-whitespace character
    while (end > start && std::isspace(name[end - 1])) {
        --end;
    }

    // Return the substring from start to end
    return name.substr(start, end - start);
}
