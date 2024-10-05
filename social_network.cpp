#include <iostream> 
#include "network.h"
#include "user.h"

int main(int argc, char* argv[]) {

	if (argc < 2) {
		std::cerr << "Error: Please provide the filename for data" << std::endl;
		return 1;
	}
	
	char* filename = argv[1];
	std::cout << "Filename provided: " << filename << std::endl;

	Network myNetwork;

	myNetwork.readUsers(filename);
	std::cout << "Finished reading users. Entering the loop..." << std::endl;

	bool running = true;

	while (running) {
		std::cout << "Please select one of the options" << std::endl;
		std::cout << "Option 1: Add a user" << std::endl;
		std::cout << "Option 2: Add Friend connection" << std::endl;
		std::cout << "Option 3: Delete friend connection" << std::endl;
		std::cout << "Option 4: Write to file" << std::endl;
		std::cout << "Enter any option outside of 1-4 to exit: " << std::endl;
		int val;
		std::cout << "Enter the number corresponding to the choice: ";
		std::cin >> val;

		if (val == 1) {
			std::string name;
			std::string last;
			std::string fullname;
			int year; 
			int zip;


			std::cout << "Please enter your name, year, and zip (all on the same line with one space in between): " << std::endl;
			std::cin >> name >> last >> year >> zip;
			fullname = name + " " + last;
			std::set<int> friends;
			User* user = new User(myNetwork.numUsers(), fullname, year, zip, friends);
			
			int initialCount = myNetwork.numUsers();
		    myNetwork.addUser(user);
		    int newCount = myNetwork.numUsers();

		    if (newCount == initialCount + 1) {
		        std::cout << "User added successfully." << std::endl;
		    } else {
		        std::cout << "Failed to add user." << std::endl;
		    }

		    // Debug output for the user
		    std::cout << "User created: " << fullname << ", ID: " << user->getId() << ", Year: " << user->getYear() << ", Zip: " << user->getZip() << std::endl;
		}

		if (val == 2) {
			std::string firstname1;
			std::string lastname1;
			std::string firstname2;
			std::string lastname2;
			std::string fullname1;
			std::string fullname2;
			std::cout << "Please enter the two users to make friends: (firstname lastname firstname lastname): " << std::endl;
			std:: cin >> firstname1 >> lastname1 >> firstname2 >> lastname2;
			fullname1 = firstname1 + " " + lastname1;
			fullname2 = firstname2 + " " + lastname2;
			std::cout << fullname1 << std::endl;
			std::cout << fullname2 << std::endl;


			int result = myNetwork.addConnection(fullname1, fullname2);

			if (result == 0) {
				std::cout << "Friendship added successfully." << std::endl;
				continue;
			} 
			if (result == -1) {
				std::cout << "One of the users or both is invalid" << std::endl;
			} 
			if (result == 1) {
				std::cout << "This connetion already exists" << std::endl;
			}
		}

		if (val == 3) {
			std::string firstname1;
			std::string lastname1;
			std::string firstname2;
			std::string lastname2;
			std::string fullname1;
			std::string fullname2;
			std::cout << "Please enter the two users to delete connection: (firstname lastname firstname lastname): " << std::endl;
			std:: cin >> firstname1 >> lastname1 >> firstname2 >> lastname2;
			fullname1 = firstname1 + " " + lastname1;
			fullname2 = firstname2 + " " + lastname2;

			int result = myNetwork.deleteConnection(fullname1, fullname2);

			if (result == 0) {
				std::cout << "Friendship deleted successfully." << std::endl;
				continue;
			} 
			else if (result == -1) {
				std::cout << "One of the users or both is invalid" << std::endl;
			} 
			else if (result == 1) {
				std::cout << "This connetion doesn't exists" << std::endl;
			}
		}
		if (val == 4) {
			char* filename = new char[100];
			std::cout << "Please enter a filename to write to" << std::endl;
			std::cin >> filename;

			myNetwork.writeUsers(filename);
		}

		if (val < 1 || val > 4) {
			running = false;
		}
	}
	return 0;
};

		

	