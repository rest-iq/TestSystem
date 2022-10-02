#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "Answer.h"
using namespace std;
class Question {
private:
	string text;
	vector<Answer> answers;
public:

	Question() {    }
	Question(string text)
	{
		this->text = text;
	}
	string getText()
	{
		return text;
	}
	void addAnswer(Answer answer)
	{
		answers.push_back(answer);
	}
	vector<Answer>& getAnswers()
	{
		return answers;
	}
	float getScore()
	{
		int correctAnswers = 0;
		int correctAnswersSelected = 0;
		int uncorrectAnswersSelected = 0;

		for (size_t i = 0; i < answers.size(); i++)
		{

			if (answers[i].getCorrect() and answers[i].getSelected())
			{
				correctAnswersSelected++;
			}
			if (!answers[i].getCorrect() and answers[i].getSelected())
			{
				uncorrectAnswersSelected++;
			}

			if (answers[i].getCorrect())
			{
				correctAnswers++;
			}
		}
		float result = ((float)correctAnswersSelected - uncorrectAnswersSelected) / correctAnswers;
		if (result <= 0)
		{
			return 0;
		}
		return result;
	}
	friend ofstream& operator << (ofstream& out, Question& question)
	{
		out << question.text << endl;
		out << question.answers.size() << endl;
		for (size_t i = 0; i < question.answers.size(); i++)
		{
			out << question.answers[i];
		}
		return out;
	}
	friend ifstream& operator >> (ifstream& in, Question& question)
	{
		int size;
		getline(in, question.text);
		in >> size;
		in.ignore(99999, '\n');
		for (size_t i = 0; i < size; i++)
		{
			Answer answer;
			in >> answer;
			question.addAnswer(answer);
		}
		return in;
	}
};