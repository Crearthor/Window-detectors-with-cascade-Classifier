#include <opencv2/highgui/highgui.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include <direct.h>
using namespace cv;
using namespace std;
CascadeClassifier window_cascade;
string window_cascade_name = "HAAR.xml";
string window_name = "Haar Window detector";
float j = 0, z = 0, data_selection, detection_selection, information_selection, number_photos = 0, incorrect_detections = 0, number_windows, no_window;
int slider1 = 0, slider2 = 0, slider3 = 0;
float wynik;
int typPomiaru;
bool wykryto_obiekt = false;
vector<String> filenames;

void detectWindow(Mat img, String out);

int main() {
	string out, str;


	_mkdir("Converted");
	Mat frame, img;
	// Get all jpg in the folder
	cout << "Do you want to detect windows in photos or camera image?" << endl << "1.Photos"
		<< endl << "2.Camera";
	cin >> typPomiaru;
	if (typPomiaru == 2) {
		VideoCapture capture = VideoCapture(0);
		if (!window_cascade.load(window_cascade_name)) {
			cout << "File not found " << window_cascade_name << ".";
			return 0;
		}
		if (detection_selection == 1)
			namedWindow(window_name, CV_WINDOW_AUTOSIZE);
		while (waitKey(1) != 27) {
			capture >> frame;
			frame.copyTo(img);
			detectWindow(img, "");
		}
		capture.release();
	}
	if (typPomiaru == 1) {
		cout << "Wchich photos do you want check:" << endl
			<< "1. Photos with windows" << endl
			<< "2. Photos without windows" << endl;
		cin >> data_selection;

		if (data_selection == 1) {
			cv::glob("positives/*", filenames);
		}
		if (data_selection == 2) {
			cv::glob("negatives/*.jpg", filenames);
		}
		cout
			<< "Do you want check all detection results one by one or do you want to perform detection automatically with overall detection result? (Photos with detected windows are saved in \"Converted\" folder for both options)"
			<< endl << "1. Check results photos one by one" << endl
			<< "2. Perform detection automatically" << endl;
		cin >> detection_selection;
		if (data_selection == 1 && detection_selection == 1) {
			cout
				<< "Do you want to get basic information about detection like number of detections, photos and in how many photos object was detected, or do you want to get extended information. By choosing second option you must manually select number of incorrect dettections, how many windows was detecked right and how many windows wasn`t detected in photos. "
				<< endl << "1. Basic" << endl << "2. Extended"
				<< endl;
			cin >> information_selection;
		}
		for (size_t i = 0; i < filenames.size(); i++) {

			Mat img = imread(filenames[i]);
			if (!window_cascade.load(window_cascade_name)) {
				cout << "Fike not found" << window_cascade_name << ".";
				return 0;
			}
			if (detection_selection == 1)
				namedWindow(window_name, CV_WINDOW_AUTOSIZE);
			str = filenames[i];
			str.erase(0, 10);
			cout << str<<endl;
			out = "Converted/" + str;
			detectWindow(img, out);
			imwrite(out, img);
			z++;

			if (detection_selection == 1)
				if ((char)cv::waitKey(0) == 27) {
					return 0;
				}
			if (information_selection == 2) {
				incorrect_detections += getTrackbarPos("errors", window_name);
				number_windows += getTrackbarPos("Windows detected", window_name);
				no_window += getTrackbarPos("windows undetected", window_name);
			}
		}

		cout << "Number of detected windows: " << j << endl
			<< "number of scanned photos : " << z << endl
			<< "Object was detected in " << number_photos
			<< " images" << endl;

		if (data_selection == 2) {
			wynik = (z - number_photos) / z * 100;
			cout << "Classifier has detected windows for " << wynik << "% of the images"<< endl;
		}
		else {
			if (information_selection == 2) {
				wynik = number_windows / (number_windows + no_window) * 100;
				cout << "Number of incorrect detections: " << incorrect_detections << endl;
				cout << "Number of detected windows: " << number_windows
					<< "Number of multiple detections: " << j - number_windows << endl;
				cout << "Number of undetected windows: " << no_window << endl;
				cout << "The correctness of detection is "
					<< wynik << "%" << endl;
				cout << "Percent incorrect detections: " << incorrect_detections / j * 100
					<< "%";
			}
			else {
				wynik = number_photos / z * 100;
				cout << "Classifier has detected windows for: " << wynik
					<< "% images" << endl;
			}
		}
	}
	waitKey(0);
	return 0;
}

void detectWindow(Mat img, String out) {

	vector<Rect> windows;
	Mat img_gray;
	if (information_selection == 2) {
		createTrackbar("Incorrect detections", window_name, &slider1, 20);
		createTrackbar("Windows detected", window_name, &slider2, 20);
		createTrackbar("Windows undetected", window_name, &slider3, 20);
	}
	cvtColor(img, img_gray, CV_BGR2GRAY);
	window_cascade.detectMultiScale(img_gray, windows, 1.1, 2,
		0 | CV_HAAR_FIND_BIGGEST_OBJECT, Size(24, 24)); 
	for (unsigned i = 0; i < windows.size(); i++) {
		wykryto_obiekt = true;
		Rect rect_window(windows[i]);
		j++;
		rectangle(img, rect_window, Scalar(255, 0, 0), 4, 8, 0);

	}
	if (wykryto_obiekt == true) {
		number_photos++;
		wykryto_obiekt = false;
	}

	if (detection_selection == 1 || typPomiaru == 2) {
		imshow(window_name, img);
		if (detection_selection == 1)
			imwrite(out, img);
	}
}
