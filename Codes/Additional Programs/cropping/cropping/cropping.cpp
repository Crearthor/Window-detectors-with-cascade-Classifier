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
			while (getline(inputFile, text))
			{
				istringstream iss(text);
				if (!(iss >> x1 >> y1 >> x2 >> y2)) { break; }				
				img1 = imread("Original/" + FileName + ".jpg");
				if (x2 > img1.size().width)
					x2 = img1.size().width;
				if (y2 > img1.size().height)
					y2 = img1.size().height;
				width = x2 - x1;
				height = y2 - y1;
				img2 = img1(Rect(x1, y1, width, height));
				EndName = FileName + "(" + to_string(i) + ").jpg";
				imwrite("Cropped/" + EndName, img2);
				i++;
				
			}
			i = 0;
			cout << FileName << " cropped" << endl;
		}
		inputFile.close();
	}
	inputList.close();

	return 0;
}

