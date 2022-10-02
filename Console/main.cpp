#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <string>
#include <vector>
#include <Windows.h>
#include "menu.h"
#include <stdio.h>
#include "queue"
#include "FileCryptor.h"
#include <chrono>

/*
Система тестування

2 режими (вчитель та учень)

Вчитель - може створювати тести.
Учень - проходити тести.

З варіантів лише один правильний.
За результатами тестуквання рахує бали у відсотках

Учень реєструєтсья (логін/пароль)
Вчитель може не реєструватися (він створює тести він їх не проходить)

Тести зберігаються у файлах.
Кількість варіантів може бути різною.

Було б непогано реалізувати щоб питання та верні варіанти відповіді шифрувалися.

Тест обмежується часом (це також задається під час створення тесту)
*/

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
		float result = ((float )correctAnswersSelected - uncorrectAnswersSelected) / correctAnswers;
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
			score+= qustions[i].getScore();		
		}
		SetCursorPosition(3, 3);

		cout << 100*(score / totalScore) << "%";
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

int main() {
	/*Student test;
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
   for (size_t i = 0; i < qustions.size(); i++)
   {
	   cout << qustions[i].getText() << endl;

	   for (size_t q = 0; q < qustions[i].getAnswers().size(); q++)
	   {
		   cout << qustions[i].getAnswers()[q].getText() << endl;
	   }
   }*/
   //  vector<Question> qustions = test.createQustions();
   //  ofstream file("test.txt");
   //  for (size_t i = 0; i < qustions.size(); i++)
   //  {
	//     file << qustions[i];
	// }
   //  file.close(); 
	int s;
	int var;
	do {

		Student game;
		game.runMainMenu();
		//system("CLS");
		cout << "Do you want return menu?\t1.yes\t2.no" << endl;
		cin >> s;
		cin.ignore(99999, '\n');

	} while (s == 1);
	cout << "Program end" << endl;
	return 0;
}

/*
* int main1()
{
	int s;
	int var;
	do {

		Student2 game;
		game.run();
		//system("CLS");
		cout << "Do you want return menu?\t1.yes\t2.no" << endl;
		cin >> s;
		cin.ignore();

	} while (s == 1);
	cout << "Program end" << endl;

	return 0;

   //E:/Console/Console
   // E:/Console/Console
   // C:/Riot/test15.txt
   //4831284/dir
}
*/

//case MainMenuOptions::RENAME:
//    teacherEnter("E:/Console/Console/test2.txt");
//    system("pause");
//    break;