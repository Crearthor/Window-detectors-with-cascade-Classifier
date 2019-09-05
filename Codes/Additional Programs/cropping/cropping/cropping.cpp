// cropping.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>  

#include <direct.h>
using namespace std;
using namespace cv;




int main()
{
	ifstream inputList, inputFile;
	int sum = 0;
	int x1, x2, y1, y2,n,i=0,width,height;
	Mat img1, img2;
	string text,FileName,EndName;

	inputList.open("Original/list.txt");
	if (inputList.fail()) {
		cout << " this directory doesnt exist";
	}

	/*while (input >> x1 >> y1 >> x2 >> y2) {
		cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
	}*/
	cout<<"Cropping Starting..."<<endl;
	while (getline(inputList, FileName))
	{
		_mkdir("Cropped");
		inputFile.open("Original/" + FileName);
		FileName =  FileName.substr(0, FileName.size() - 4);
		if (inputFile.fail()) {
			cout <<FileName +" this directory doesnt exist"<<endl;
		}
		else {
			
			getline(inputFile, text);
			n = stoi(text);
			//cout << n << endl;
			while (getline(inputFile, text))
			{
				istringstream iss(text);
				if (!(iss >> x1 >> y1 >> x2 >> y2)) { break; }				
				//cout << width << " " << height << endl;
				img1 = imread("Original/" + FileName + ".jpg");
				if (x2 > img1.size().width)
					x2 = img1.size().width;
				if (y2 > img1.size().height)
					y2 = img1.size().height;
				width = x2 - x1;
				height = y2 - y1;
				img2 = img1(Rect(x1, y1, width, height));
				//cout <<  " cols= " + img1.col << endl;
				EndName = FileName + "(" + to_string(i) + ").jpg";
				imwrite("Cropped/" + EndName, img2);
				/*namedWindow("cropped", WINDOW_AUTOSIZE);
				imshow("cropped", img2);*/
				i++;
				//waitKey(0);
			}
			i = 0;
			cout << FileName << " cropped" << endl;
		}
		inputFile.close();
	}
	inputList.close();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
