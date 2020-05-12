/*
	Vitaliy Stepanov
	CS300A Assignment 4
	Professor Dr. Fatma Cemile Serce

	A simple user authentication controller, where you can add/delete users, change passwords of 
	the users, and log users in and out, and list all registered and active users. 

*/

#ifndef AUTHENTICATIONCONTROLLER_H
#define AUTHENTICATIONCONTROLLER_H

#include <iostream>
#include <vector>
using namespace std;

#define MAX_LOAD_FACTOR 0.65

//a structure defines user entry
struct user{        
    string username;
    string password;
};

class AuthenticationController{
    vector<user> table1;            //a dynamic array that stores registered users
    vector<string> table2;          //a dynamic array that stores active users (logged in)

	int registeredTableSize;        //capacity of table1, registered users tabls
	int activeTableSize;            //capacity of table2, active users table
	int regCount;                   //current registered user count
	int activeCount;                //current active users counr

    // double hashing
	int hashFunction(string key, int tableSize, int i) { 
		int length = key.length();
		int newK = 0;
		for (int j = 0; j < length; j++)
			newK += (int) key[j];
		return ((newK % tableSize) + i * ((newK * tableSize - 1) % tableSize)) % tableSize;
	}
  public:
	AuthenticationController(int table1Size, int table2Size); // constructor
	int addUser(string username, string pass); // adds user and password
	int delUser(string username); // deletes all users and logs them out
	int changePass(string username, string oldpass, string newpass); // changes user password
	int login(string username, string pass); // logs user in if they registered
	int logout(string username); // logs user out
	float printActiveUsers(); // prints logged in users
	float printPasswords(); // prints all registered users and all passwords
};
#endif