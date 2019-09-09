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
	cout << "What would you like to do:" << endl;
	cout << "1.Train model" << endl << "2.Test model" << endl;
	cin >> wyborAkcji;
	if (wyborAkcji == 1) {
		str = "darknet detector train training/obj.data training/yolo-obj.cfg darknet53.conv.74";
		command = str.c_str();
		system(command);
	}
	else
	{

		cout << "Which source would you like to perform detection: " << endl;

		cout << "1.Image" << endl << "2.Video" << endl << "3.Camera" << endl;
		cin >> wyborFormatu;
		if (wyborFormatu != 3) {
			cout << "Write file name: ";
			cin >> str;
		}

		switch (wyborFormatu)
		{
		case 1:
		{
			str = "darknet detect trening/yolo-obj.cfg training/yolo-obj.weights " + str;
			command = str.c_str();
			system(command);
			break;
		}
		case 2:
		{
			str = "darknet detector demo trening/obj.data training/yolo-obj.cfg training/yolo-obj.weights "+  str;
			command = str.c_str();
			system(command);
			break;
		}
		case 3:
		{
			system("darknet detector demo trening/obj.data training/yolo-obj.cfg training/yolo-obj.weights");
			break;
		}
		}
	}
	system("pause");

}