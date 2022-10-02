#pragma once
#include "User.h"
#include "ServiceAutorization.h"
#include "Answer.h"
#include "Question.h"
#include "QuestionView.h"
#include "menu.h"
#include "FileCryptor.h"
#include <chrono>
class Student
{
private:
	//головне меню нашої програми
	enum MainMenuOptions { STUDENT, TEACHER, EXIT };
	//головне меню програми 
	int mainMenu() {
		Menu menu({ "1. Student ",
			"2 Teacher ",
			"EXIT" });
		menu.drawOptions();
		int key;
		while (true)
		{
			menu.drawFrame();
			key = getKey();
			if (key == -1) {
				Sleep(90);
				continue;
			}
			switch (key)
			{
			case UP_ARROW:
				menu.up();
				menu.drawOptions();
				break;
			case DOWN_ARROW:
				menu.down();
				menu.drawOptions();
				break;
			case ENTER:
				system("cls");
				return menu.getSelectedOption();
			default:
				break;
			}
			Sleep(10);
		}
	}
	//меню учня 
	enum StudentMenuOptions { LOGIN, REGISTER, TEST, EXIT1 };
	//меню вчителя 
	enum TeacherMenuOptions { ANSWERS, MAKETEST, EXIT2 };
	//головне меню студента
	int mainMenuStudent() {
		Menu menu({
			"1.Login student",
			 "2.Registration student",
			 "3.Test for student",
			"EXIT" });
		menu.drawOptions();
		int key;
		while (true)
		{
			menu.drawFrame();
			key = getKey();
			if (key == -1) {
				Sleep(90);
				continue;
			}
			switch (key)
			{
			case UP_ARROW:
				menu.up();
				menu.drawOptions();
				break;
			case DOWN_ARROW:
				menu.down();
				menu.drawOptions();
				break;
			case ENTER:
				system("cls");
				return menu.getSelectedOption();
			default:
				break;
			}
			Sleep(10);
		}
	}
	//головне меню вчителя
	int mainMenuTeacher() {
		Menu menu({
			"1.True answers on test",
			"2.Teacher make test ",
			"EXIT" });
		menu.drawOptions();
		int key;
		while (true)
		{
			menu.drawFrame();
			key = getKey();
			if (key == -1) {
				Sleep(90);
				continue;
			}
			switch (key)
			{
			case UP_ARROW:
				menu.up();
				menu.drawOptions();
				break;
			case DOWN_ARROW:
				menu.down();
				menu.drawOptions();
				break;
			case ENTER:
				system("cls");
				return menu.getSelectedOption();
			default:
				break;
			}
			Sleep(10);
		}
	}
	//сервіс авторизації
	ServiceAutorization* service = nullptr;
	FileCryptor cryptor;
public:
	//конструктор
	Student()
	{
		service = new ServiceAutorization("users.txt");
	}
	//деструктор
	~Student()
	{
		delete service;
	}

	//створюємо тест
	Question createQuestion()
	{
		string text;
		cout << "Enter text question" << endl;
		getline(cin, text);
		Question question(text);
		do
		{
			cout << "Enter answer" << endl;
			getline(cin, text);
			char c;
			cout << "If answer true enter y " << endl;
			cout << "If answer not true enter any key " << endl;
			cin >> c;
			cin.ignore(99999, '\n');
			bool correct = c == 'y';
			Answer answer(text, correct);
			question.addAnswer(answer);
			cout << "You want add new answer?" << endl;
			cout << "If you want enter y or enter any key" << endl;
			cin >> c;
			cin.ignore(99999, '\n');
			if (c != 'y')
				break;
		} while (true);
		return question;
	}

	//додаємо нове питання
	vector<Question> createQustions()
	{
		vector<Question> questions;

		do
		{
			questions.push_back(createQuestion());
			cout << "Add new questions press y or enter any key" << endl;
			char q;
			cin >> q;
			cin.ignore(99999, '\n');
			if (q != 'y')
				break;
		} while (true);
		return questions;
	}

	//тест який має проходити студент (не працює)
	void test()
	{
		cryptor.decrypt("test.txt");
		ifstream file("test.txt");
		vector<Question> qustions;
		int size;

		file >> size;

		file.ignore(99999, '\n');
		for (size_t i = 0; i < size; i++)
		{
			Question question;
			file >> question;
			qustions.push_back(question);
		}
		file.close();
		cryptor.crypt("test.txt");
		//тест починається
		auto end = chrono::system_clock::now() + chrono::seconds(60 * qustions.size());

		try
		{
			for (size_t i = 0; i < qustions.size(); i++)
			{

				QuestionView view(&qustions[i]);
				view.drawOptions();
				int key;
				bool answersConfirmed = false;
				while (!answersConfirmed)
				{
					//перевірка чи не вичерпався час
					//якщо час вичерпався викидаємо виключення
					if (chrono::system_clock::now() > end)
					{
						throw 0;

					}
					view.drawFrame();
					key = getKey();
					if (key == -1) {
						Sleep(90);
						continue;
					}
					switch (key)
					{
					case UP_ARROW:
						view.up();
						view.drawOptions();
						break;
					case DOWN_ARROW:
						view.down();
						view.drawOptions();
						break;
					case SPACE:
						view.click();
						view.drawOptions();
						break;

					case ENTER:
						answersConfirmed = true;
						break;


					default:
						break;
					}
					Sleep(10);
				}
				system("cls");
			}
		}
		catch (int)
		{
			system("cls");
			cout << "Time Over( " << endl;
			system("pause");
			system("cls");
		}


		float totalScore = 0;
		float score = 0;
		for (size_t i = 0; i < qustions.size(); i++)
		{
			totalScore += 1;
			score += qustions[i].getScore();
		}
		SetCursorPosition(3, 3);

		cout << 100 * (score / totalScore) << "%";
		ofstream result(service->getAutorizedUser()->getLogin() + ".txt", ios_base::app);
		for (size_t i = 0; i < qustions.size(); i++)
		{
			result << qustions[i].getText() << endl;
			result << 100 * qustions[i].getScore() << "%" << endl;
		}
		result << "Your result is: " << 100 * (score / totalScore) << "%" << endl;
		result << "----------------" << endl;

		result.close();
		system("pause");
	}

	//реєстрація
	void Regiter()
	{
		system("cls");
		cout << "Registration" << endl;
		string login;
		string password;
		string confirmPassword;
		do
		{
			cout << "Enter login:" << endl;
			getline(cin, login);
			cout << "Enter password:" << endl;
			getline(cin, password);
			cout << "Confirm password:" << endl;
			getline(cin, confirmPassword);
		} while (!service->registerUser(login, password, confirmPassword));
		service->save();
		system("cls");
	}

	//війти 
	void Login()
	{
		system("cls");
		cout << "Login" << endl;
		string login;
		string password;
		do
		{
			cout << "Enter login:" << endl;
			getline(cin, login);
			cout << "Enter password:" << endl;
			getline(cin, password);
		} while (!service->autorize(login, password));
		system("cls");
	}


	//показати правильні відповіді для вчителя
	void showTrueAnswers()
	{
		cryptor.decrypt("test.txt");
		ifstream file("test.txt");
		vector<Question> qustions;
		int size;

		file >> size;

		file.ignore(99999, '\n');
		for (size_t i = 0; i < size; i++)
		{
			Question question;
			file >> question;
			qustions.push_back(question);
		}

		file.close();
		cryptor.crypt("test.txt");
		for (size_t i = 0; i < qustions.size(); i++)
		{
			cout << qustions[i].getText() << endl;
			for (size_t q = 0; q < qustions[i].getAnswers().size(); q++)
			{
				cout << qustions[i].getAnswers()[q].getText() << endl;
				cout << qustions[i].getAnswers()[q].getCorrect() << endl;
			}
		}

		/*ifstream file("test.txt");
		cout << "File is open? " << endl;
		cout << "Y - yes : N - no " << endl;
		cout << (file.is_open() ? "Y" : "N") << endl;
		string buffer;
		cout << "correct answer EXAMPLE:" << endl;
		cout << "Andriy" << endl;
		cout << "  1  " << endl;
		cout << "----------------------" << endl;
		while (getline(file, buffer))
		{
			cout << buffer << endl;
		}
		file.close(); */
	}

	//вчитель створює тест
	void teacherMakeTest() {
		ofstream file("test.txt");
		vector<Question> qustions = createQustions();
		file << qustions.size() << endl;
		for (size_t i = 0; i < qustions.size(); i++)
		{
			file << qustions[i];
		}
		file.close();
		cryptor.crypt("test.txt");

	}


	//меню для студента 
	void MenuStudenta() {
		while (true)
		{
			SetCursorPosition(50, 5);
			SetColor(WHITE, BLACK);
			if (service->getAutorizedUser() != nullptr)
			{
				cout << service->getAutorizedUser()->getLogin();
			}
			else
			{
				cout << "NOT autorized!";
			}

			switch (mainMenuStudent()) {
			case StudentMenuOptions::LOGIN:
				Login();
				break;
			case StudentMenuOptions::REGISTER:
				Regiter();
				break;
			case StudentMenuOptions::TEST:
				system("cls");
				if (service->getAutorizedUser() != nullptr)
				{
					test();
					system("cls");

				}
				break;
			case StudentMenuOptions::EXIT1:
				return;
			default:
				break;
			}
		}
	}

	//функціонал меню для вчителя 
	void MenuTeacher() {
		while (true)
		{
			system("cls");
			switch (mainMenuTeacher()) {
			case TeacherMenuOptions::ANSWERS:
				system("cls");
				showTrueAnswers();
				system("pause");
				system("cls");
				break;
			case TeacherMenuOptions::MAKETEST:
				teacherMakeTest();
				break;
			case TeacherMenuOptions::EXIT2:
				return;
			default:
				break;
			}
		}
	}

	//функціонал головного меню 
	void runMainMenu() {
		while (true)
		{
			switch (mainMenu()) {
			case MainMenuOptions::STUDENT:
				MenuStudenta();
				break;
			case MainMenuOptions::TEACHER:
				MenuTeacher();
				break;
			case MainMenuOptions::EXIT:
				return;
			default:
				break;
			}
		}
	}
};
