#pragma once
#include "Question.h"
#include "console.h"
#include <iostream>
using namespace std;
class QuestionView {
private:
	Question* question;


	int activeOption = 0;

	size_t getMaxItemSize() const {
		size_t max = 0;
		for (Answer item : question->getAnswers())
		{
			if (item.getText().size() > max) {
				max = item.getText().size();
			}
		}
		return max;
	}


public:
	QuestionView(Question* question) {
		this->question = question;
	}
	void drawFrame() {
		//отобразить рамку
		size_t max = getMaxItemSize();
		if (max < question->getText().size()) {
			max = question->getText().size();
		}
		size_t widht = max + 9;

		size_t height = question->getAnswers().size() + 6;
		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < widht; x++)
			{
				if (x == 0 || x == widht - 1 || y == 0 || y == height - 1) {
					SetCursorPosition(x, y);
					SetColor(ConsoleColor::WHITE, (ConsoleColor)rand() % 16);
					cout << ' ';
				}
			}
		}
		SetColor(WHITE, BLACK);
	}

	void drawOptions() {
		int startX = 3;
		int startY = 2;

		SetCursorPosition(startX, startY);
		cout << question->getText();
		startY += 2;

		int max = getMaxItemSize();
		for (size_t i = 0; i < question->getAnswers().size(); i++)
		{
			SetCursorPosition(startX, startY + i);
			if (activeOption == i) {
				SetColor(RED, WHITE);
			}
			else {
				SetColor(WHITE, BLACK);
			}
			auto answer = question->getAnswers()[i];
			auto item = answer.getText();

			item.append(string(max - item.size(), ' '));
			bool isChecked = answer.getSelected();
			cout << "[" << (isChecked ? 'X' : ' ') << "] " << item;
		}
	}

	void down() {
		activeOption++;
		if (activeOption >= question->getAnswers().size()) {
			activeOption = 0;
		}
	}

	void up() {
		activeOption--;
		if (activeOption < 0) {
			activeOption = question->getAnswers().size() - 1;
		}
	}

	void click() {
		question->getAnswers()[activeOption].setSelected(!question->getAnswers()[activeOption].getSelected());
	}

};