// AppforDarknet.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>

using namespace std;
int main()
{
	int wyborFormatu,wyborAkcji;
	string str, model;
	const char  *command;
	cout << "W jakim celu chcesz uzyc aplikacji darknet:" << endl;
	cout << "1.trening" << endl << "2.Testowanie" << endl;
	cin >> wyborAkcji;
	if (wyborAkcji == 1) {
		str = "darknet detector train trening/obj.data trening/yolo-obj.cfg darknet53.conv.74";
		command = str.c_str();
		system(command);
	}
	else
	{

		cout << "Jakiego formatu chcesz dokonaæ detekcji:" << endl;

		cout << "1.Zdjecie" << endl << "2.Film" << endl << "3.Kamera" << endl;
		cin >> wyborFormatu;
		if (wyborFormatu != 3) {
			cout << "Podaj nazwe pliku ";
			cin >> str;
		}

		switch (wyborFormatu)
		{
		case 1:
		{
			str = "darknet detect trening/yolo-obj.cfg trening/yolo-obj.weights " + str;
			command = str.c_str();
			system(command);
			break;
		}
		case 2:
		{
			str = "darknet detector demo trening/obj.data trening/yolo-obj.cfg trening/yolo-obj.weights "+  str;
			command = str.c_str();
			system(command);
			break;
		}
		case 3:
		{
			system("darknet detector demo trening/obj.data trening/yolo-obj.cfg trening/yolo-obj.weights");
			break;
		}
		}
	}
	system("pause");

}