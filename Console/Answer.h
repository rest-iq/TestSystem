#pragma once
#include <string>
#include <fstream>
using namespace std;
class Answer {
private:
	string text;
	bool correct = false;
	bool selected = false;
public:
	Answer() {  }
	Answer(string text, bool correct)
	{
		this->correct = correct;
		this->text = text;
		this->selected = false;
	}
	string getText()
	{
		return text;
	}
	bool  getCorrect()
	{
		return correct;
	}
	bool getSelected()
	{
		return selected;
	}
	void setSelected(bool selected)
	{
		this->selected = selected;
	}
	friend ofstream& operator << (ofstream& out, Answer& answer)
	{
		out << answer.text << endl;
		out << answer.correct << endl;
		return out;
	}
	friend ifstream& operator >> (ifstream& in, Answer& answer)
	{
		getline(in, answer.text);
		in >> answer.correct;
		in.ignore(99999, '\n');
		return in;
	}

};