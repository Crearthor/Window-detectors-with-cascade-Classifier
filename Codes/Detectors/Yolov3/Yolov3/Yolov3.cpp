#include "pch.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <direct.h>


using namespace cv;
using namespace dnn;
using namespace std;

// Initialize the parameters
float confThreshold = 0.25; // Confidence threshold
float nmsThreshold = 0.2;  // Non-maximum suppression threshold

//mniejszy Rozmiar obrazka przyspiesza działanie, ale zmniejsza dokładnosc
int inpWidth = 416;  // Szerokość obrazu wejściowego
int inpHeight = 416; // Wysokość obrazu wejściowego
vector<string> classes;

// Remove the bounding boxes with low confidence using non-maxima suppression
void postprocess(Mat& frame, const vector<Mat>& out);

// Draw the predicted bounding box
void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame);

// Get the names of the output layers
vector<String> getOutputsNames(const Net& net);

int main(int argc, char** argv)
{

	// Load names of classes
	string classesFile = "obj.names"; //plik zawierający nazwy obiektow
	ifstream ifs(classesFile.c_str()); //
	string line;
	while (getline(ifs, line)) classes.push_back(line);

	// Give the configuration and weight files for the model
	String modelConfiguration = "yolo-obj.cfg";
	String modelWeights = "yolo-obj.weights";

	// Load the network
	Net net = readNetFromDarknet(modelConfiguration, modelWeights);
	net.setPreferableBackend(DNN_BACKEND_OPENCV);
	net.setPreferableTarget(DNN_TARGET_OPENCL);

	// Open a video file or an image file or a camera stream.
	string str, outputFile;
	VideoCapture cap;
	VideoWriter video;
	Mat frame, blob;
	vector<cv::String> fn;
	size_t count = 1;
	int detectionSource, numberPhotos=0;
		
	cout << "Do you want to detect windows in photos, video or camera image ?: "<<endl;
	cout << "1. Photos" << endl << "2. Videos" << endl << "3. Camera";
	cin >> detectionSource;
	if (detectionSource != 3) {
		cout << "Do you want perform detection for multiple files or one: " << endl;
		cout << "1. All files from Test folder" << endl << "2. One file" << endl;
		cin >> numberPhotos;
		if (numberPhotos == 2) {
			cout << "Write file name:";
			cin >> str;
		}
		else
		{
			glob("Test/", fn, false);
			
			vector<Mat> images;
			count = fn.size();
		}
	}outputFile = "yolo_out_cpp.avi";
	for (size_t i = 0; i < count; i++)
	{
		if (numberPhotos == 1)
			str = fn[i];
		switch (detectionSource)
		{
			case 1:
			{
				ifstream ifile(str);
				if (!ifile) throw("error");
				cap.open(str);
				str.replace(str.end() - 4, str.end(), "_yolo_out_cpp.jpg");
				str.erase(0, 5);
				_mkdir("Scanned");
				outputFile = "Scanned/" + str;
				break;
			}
			case 2:
			{
				ifstream ifile(str);
				if (!ifile) throw("error");
				cap.open(str);
				str.replace(str.end() - 4, str.end(), "_yolo_out_cpp.avi");
				str.erase(0, 5);
				_mkdir("Scanned");
				outputFile = "Scanned/" + str;
				break;
			}
			// Open the webcam
			case 3:
			{
				cap = VideoCapture(0);
				break;
			}
		}

		// Get the video writer initialized to save the output video
		if (detectionSource != 1) {
			video.open(outputFile, VideoWriter::fourcc('M', 'J', 'P', 'G'), 28, Size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT)));
		}

		// Create a window
		static const string kWinName = "Window Detector YOLOv3";
		namedWindow(kWinName, WINDOW_NORMAL);
		
		// Process frames.
		while (waitKey(1) < 0)
		{
			// get frame from the video
			cap >> frame;
			// Stop the program if reached end of video
			if (frame.empty()) {
				cout << "Detection completed" << endl;
				cout << "Detection result has been saved to a file: " << outputFile << endl;
				waitKey(0);
				break;
			}
			// Create a 4D blob from a frame.
			blobFromImage(frame, blob, 1 / 255.0, cvSize(inpWidth, inpHeight), Scalar(0, 0, 0), true, false);

			//Sets the input to the network
			net.setInput(blob);

			// Runs the forward pass to get output of the output layers
			vector<Mat> outs;
			net.forward(outs, getOutputsNames(net));

			// Remove the bounding boxes with low confidence
			postprocess(frame, outs);
		
			// Put efficiency information. The function getPerfProfile returns the overall time for inference(t) and the timings for each of the layers(in layersTimes)
			vector<double> layersTimes;
			double freq = getTickFrequency() / 1000;
			double t = net.getPerfProfile(layersTimes) / freq;
			string label = format("Inference time for a frame : %.2f ms", t);
			putText(frame, label, Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));

			imshow(kWinName, frame);
			if (detectionSource == 1) {
				imwrite(outputFile, frame);
			}
			else video.write(frame);		
		}

		cap.release();
		if (detectionSource != 1) video.release();
		//waitKey(0);
	}
	return 0;
}

// Remove the bounding boxes with low confidence using non-maxima suppression
void postprocess(Mat& frame, const vector<Mat>& outs)
{
	vector<int> classIds;
	vector<float> confidences;
	vector<Rect> boxes;

	for (size_t i = 0; i < outs.size(); ++i)
	{

		float* data = (float*)outs[i].data;
		for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
		{
			Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
			Point classIdPoint;
			double confidence;
			// Get the value and location of the maximum score
			minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
			if (confidence > confThreshold)
			{
				int centerX = (int)(data[0] * frame.cols);
				int centerY = (int)(data[1] * frame.rows);
				int width = (int)(data[2] * frame.cols);
				int height = (int)(data[3] * frame.rows);
				int left = centerX - width / 2;
				int top = centerY - height / 2;

				classIds.push_back(classIdPoint.x);
				confidences.push_back((float)confidence);
				boxes.push_back(Rect(left, top, width, height));
			}
		}
	}

	vector<int> indices;
	NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
	for (size_t i = 0; i < indices.size(); ++i)
	{
		int idx = indices[i];
		Rect box = boxes[idx];
		drawPred(classIds[idx], confidences[idx], box.x, box.y,
			box.x + box.width, box.y + box.height, frame);
	}
}

// Draw the predicted bounding box
void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame)
{
	//Draw a rectangle displaying the bounding box
	rectangle(frame, Point(left, top), Point(right, bottom), Scalar(255, 178, 50), 3);

	//Get the label for the class name and its confidence
	string label = format("%.2f", conf);
	if (!classes.empty())
	{
		CV_Assert(classId < (int)classes.size());
		label = classes[classId] + ":" + label;
	}

	//Display the label at the top of the bounding box
	int baseLine;
	Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
	top = max(top, labelSize.height);
	rectangle(frame, Point(left, top - round(1.5*labelSize.height)), Point(left + round(1.5*labelSize.width), top + baseLine), Scalar(255, 255, 255), FILLED);
	putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 0), 1);
}

// Get the names of the output layers
vector<String> getOutputsNames(const Net& net)
{
	static vector<String> names;
	if (names.empty())
	{
		//Get the indices of the output layers, i.e. the layers with unconnected outputs
		vector<int> outLayers = net.getUnconnectedOutLayers();

		//get the names of all the layers in the network
		vector<String> layersNames = net.getLayerNames();

		// Get the names of the output layers in names
		names.resize(outLayers.size());
		for (size_t i = 0; i < outLayers.size(); ++i)
			names[i] = layersNames[outLayers[i] - 1];
	}
	return names;
}