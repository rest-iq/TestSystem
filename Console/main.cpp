#include "menu.h"
#include <stdio.h>
#include "Student.h"

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