#pragma once
#include <fstream>
#include <string>
using namespace std;


class User {
private:
	string login;
	string password;
public:
	User() { }
	User(string login)
	{
		this->login = login;
	}
	string getLogin()
	{
		return login;
	}
	void setPassword(string password)
	{
		this->password = password;
	}
	bool validatePassword(string password)
	{
		return this->password == password;
	}
	friend ofstream& operator << (ofstream& out, User& user)
	{
		out << user.login << endl;
		out << user.password << endl;
		return out;
	}
	friend ifstream& operator >> (ifstream& in, User& user)
	{
		getline(in, user.login);
		getline(in, user.password);
		return in;
	}
};