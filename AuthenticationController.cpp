/*
	Vitaliy Stepanov
	CS300A Assignment 4
	Professor Dr. Fatma Cemile Serce

	A simple user authentication controller, where you can add/delete users, change passwords of 
	the users, and log users in and out, and list all registered and active users. 

*/

#include "AuthenticationController.h"

// constructor initializes table values to empty
AuthenticationController::AuthenticationController(int table1Size, int table2Size) {
    table1.resize(table1Size); // resizes vector<user>
    for(int i = 0; i < table1Size; i++) { // sets all values to EMPTY
        table1[i].username = "EMPTY";
        table1[i].password = "EMPTY";
    }
    table2.resize(table2Size); // resizes vector<string>
    for(int i = 0; i < table2Size; i++) { // sets all values to EMPTY
        table2[i] = "EMPTY";
    }
    registeredTableSize = table1Size; //capacity of table1, registered users tabls
	activeTableSize = table2Size; //capacity of table2, active users table
	regCount = 0; //current registered user count
	activeCount = 0; // current logged in users
}
    
// register the given user with username and password to registered users table if not registered
// return 1 if successful, 0 if not.
int AuthenticationController::addUser(string username, string pass) {
	for (int i = 0; i < registeredTableSize; i++) {
		int key = hashFunction(username, registeredTableSize, i);
		if (table1[key].username == username) {  // no duplicate usernames
			//cout << "Username already exists." << endl;
			return 0;
		}
		if (table1[key].username == "EMPTY" || table1[key].username == "DELETED") { // adds user to table
			table1[key].username = username;
			table1[key].password = pass;
			regCount++;
			return 1;
		}
	}
	return 0;
}

// delete all entries of that user
// Put <"DELETED","DELETED"> as deleted entry
// return 1 if the user is registered else just return 0 
int AuthenticationController::delUser(string username) {
	bool registered = false;
	for (int i = 0; i < registeredTableSize; i++) {
		int key = hashFunction(username, registeredTableSize, i);
		if (table1[key].username == username) { // deletes all users of that username
			table1[key].username = "DELETED";
			table1[key].password = "DELETED";
			regCount--; // same username can have many passwords so decrement regCount everytime
			registered = true;
		}
	}
	if(registered) { 
		logout(username); // assertion, only one unique username will be here so outside for loop
		return 1;
	}
	return 0;
}

//change the password of the given user if user is registered and 
//oldpassword is equal the current password. While changing the password, 
//keep the old entries <username,oldpassword> in the table and add 
//new entry <username,newpassword> To the next empty cell that is 
//found with the hash function. Last entry addes is the current 
//password of that user. If operation is successful return 1, else return 0
int AuthenticationController::changePass(string username, string oldpass, string newpass) {
	bool currentPassword = false;
	for (int i = 0; i < registeredTableSize; i++) { 
		int key = hashFunction(username, registeredTableSize, i);
		if (table1[key].username == username && table1[key].password == oldpass) {// check if user exists first
			int key2 = hashFunction(username, registeredTableSize, i + 1);
			if (table1[key2].username != username) { // check if most recent password
				currentPassword = true;
				break;
			} else { // user tried to change password with old password
				//cout << "Old password." << endl;
				return 0;
			}
		}
	}
	if (currentPassword) { // add user to registered table
		for (int i = 0; i < registeredTableSize; i++) {
			int key = hashFunction(username, registeredTableSize, i);
			if (table1[key].username == "EMPTY" || table1[key].username == "DELETED") {
				table1[key].username = username;
				table1[key].password = newpass;
				regCount++; 
				//cout << "Password successfully changed." << endl;
				return 1;
			}
		}
	}
	//cout << "Invalid username or password" << endl;
	return 0;
}

//if the user is registered and password is the current 
//password of given user, log the user in and return 1, 
//if not do nothing and return 0.
int AuthenticationController::login(string username, string pass) {
	bool correctLogin = false;	
	for (int i = 0; i < registeredTableSize; i++) {
		int key = hashFunction(username, registeredTableSize, i);
		if (table1[key].username == username && table1[key].password == pass) { // finds username/pass
			int key2 = hashFunction(username, registeredTableSize, i + 1); // check if same user exists
			if (table1[key2].username != username) { // if doesn't exists, then password is newest
				correctLogin = true;
				break;
			} else {
				//cout << "Old password. Please type newest password." << endl;
				return 0;
			}
		}
	}
	// put user from table 1 to table 2 to mark as logged in
	if (correctLogin) {
		for (int j = 0; j < registeredTableSize; j++) {
			int key = hashFunction(username, registeredTableSize, j);
			if (table2[key] == "EMPTY" || table2[key] == "DELETED") {
				table2[key] = username;
				activeCount++;
				return 1;	
			}
		}
	}
	return 0;
}

//if the given user is logged in, log that user out and retun 1, 
//if not return 0
//Put “DELETED” for logged out users. 
int AuthenticationController::logout(string username) {
	for (int i = 0; i < registeredTableSize; i++) {
		int key = hashFunction(username, registeredTableSize, i);
		if (table2[key] == username) {
			table2[key] = "DELETED";
			activeCount--;
			return 1;
		} 
	}
	return 0;
}

//print all logged in users 
//print “DELETED” for deleted entries, “EMPTY” for empty entries
//returns load factor.
float AuthenticationController::printActiveUsers() {
	cout << "Logged in users:" << endl;
    for (int i = 0; i < table2.size(); i++) {
		cout << table2[i] << endl;
    }
	cout << "Load Factor: "; 
    return (0.0 + activeCount) / activeTableSize; // Percent of logged in users relative to capacity of table2
}

//print all users and passwords, print “DELETED DELETERED” for deleted entries
//print “EMPTY EMPTY” for empty entries
//returns load factor. 
float AuthenticationController::printPasswords() {
	cout << "User:   Password: " << endl;
	for (int i = 0; i < table1.size(); i++) {
		cout << table1[i].username + "   " + table1[i].password << endl;
	}
	cout << "Load Factor: ";
	return (0.0 + regCount) / registeredTableSize; // Percent of users relative to capacity of table1
}
