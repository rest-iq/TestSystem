#pragma once
#include <string>
#include <vector>
#include "User.h"

class ServiceAutorization
{
private:
	string file;
	vector<User> users;
	User* autorizedUser = nullptr;
	void load()
	{
		users.clear();
		ifstream in(file);
		int size;
		in >> size;
		in.ignore(99999, '\n');
		for (size_t i = 0; i < size; i++)
		{
			User user;
			in >> user;
			users.push_back(user);
		}
		in.close();
	}
public:
	void save()
	{
		ofstream out(file);
		out << users.size() << endl;
		for (size_t i = 0; i < users.size(); i++)
		{
			out << users[i];
		}
		out.close();
	}
	ServiceAutorization(string file)
	{
		this->file = file;
		load();
	}

	bool registerUser(string login, string password, string confirmPassword)
	{
		if (password != confirmPassword)
		{
			return false;
		}
		for (size_t i = 0; i < users.size(); i++)
		{
			if (users[i].getLogin() == login)
			{
				return false;
			}
		}
		User user(login);
		user.setPassword(password);
		users.push_back(user);
		return autorize(login, password);

	}
	bool autorize(string login, string password)
	{

		for (size_t i = 0; i < users.size(); i++)
		{
			if (users[i].getLogin() == login and users[i].validatePassword(password))
			{
				autorizedUser = &users[i];
				return true;
			}

		}
		return false;
	}
	User* getAutorizedUser() {
		return autorizedUser;
	}
	~ServiceAutorization()
	{
		save();
	}
};