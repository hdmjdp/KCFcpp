#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "kcftracker.hpp"

#include <dirent.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]){

//	char cur_fn[255];
//	char prefix[] = "./images/";
//	char ext[] = ".jpg";
//	cv::VideoCapture capture(argv[1]);


	if (argc > 5) return -1;

	bool HOG = true;
	bool FIXEDWINDOW = false;
	bool MULTISCALE = true;
	bool SILENT = false;//true;
	bool LAB = false;
    /*
	for(int i = 0; i < argc; i++){
		if ( strcmp (argv[i], "hog") == 0 )
			HOG = true;
		if ( strcmp (argv[i], "fixed_window") == 0 )
			FIXEDWINDOW = true;
		if ( strcmp (argv[i], "singlescale") == 0 )
			MULTISCALE = false;
		if ( strcmp (argv[i], "show") == 0 )
			SILENT = false;
		if ( strcmp (argv[i], "lab") == 0 ){
			LAB = true;
			HOG = true;
		}
		if ( strcmp (argv[i], "gray") == 0 )
			HOG = false;
	}
	*/
	// Create KCFTracker object
	KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);

	// Frame readed
	Mat frame;

	// Tracker results
	Rect result;

//	// Path to list.txt
//	ifstream listFile;
//	string fileName = "/Volumes/Transcend/KCFcpp/Dudek/images.txt";
//    cout<<fileName<<endl;
//  	listFile.open(fileName);

  	// Read groundtruth for the 1st frame
  	ifstream groundtruthFile;
	string groundtruth = "/Volumes/Transcend/KCFcpp/Dudek/region.txt"; //数据集中其实只有四个值,分别是x, y, box-width, box-height
  	groundtruthFile.open(groundtruth);
  	string firstLine;
  	getline(groundtruthFile, firstLine);
	groundtruthFile.close();
  	//cout<<firstLine<<endl;
  	istringstream ss(firstLine);

  	// Read groundtruth like a dumb
//  	float x1, y1, x2, y2, x3, y3, x4, y4;
//
//  	char ch;
//	ss >> x1;
//	ss >> ch;
//	ss >> y1;
//	ss >> ch;
//	ss >> x2;
//	ss >> ch;
//	ss >> y2;
//	ss >> ch;
//	ss >> x3;
//	ss >> ch;
//	ss >> y3;
//	ss >> ch;
//	ss >> x4;
//	ss >> ch;
//	ss >> y4;
//    cout<<x1<<','<<y1<<','<<endl;
    float x, y, width0, height0;
    char ch;
    ss >> x;
    ss >> ch;
	ss >> y;
    ss >> ch;
	ss >> width0;
    ss >> ch;
	ss >> height0;
	// Using min and max of X and Y for groundtruth rectangle
	float xMin = x;//min(x1, min(x2, min(x3, x4)));
	float yMin = y;//min(y1, min(y2, min(y3, y4)));
	float width = width0;//max(x1, max(x2, max(x3, x4))) - xMin;
	float height = height0;//max(y1, max(y2, max(y3, y4))) - yMin;

    cout<<xMin <<','<<yMin<<','<<width<<','<<height<<endl;
	// Read Images
	ifstream listFramesFile;
	string listFrames = "/Volumes/Transcend/KCFcpp/Dudek/images.txt";
	listFramesFile.open(listFrames);
	string frameName;


	// Write Results
	ofstream resultsFile;
	string resultsPath = "output.txt";
	resultsFile.open(resultsPath);

	// Frame counter
	int nFrames = 0;


	while ( getline(listFramesFile, frameName) ){
		frameName = frameName;

		// Read each frame from the list
		frame = imread(frameName, CV_LOAD_IMAGE_COLOR);

		// First frame, give the groundtruth to the tracker
		if (nFrames == 0) {
			tracker.init( Rect(xMin, yMin, width, height), frame );
			rectangle( frame, Point( xMin, yMin ), Point( xMin+width, yMin+height), Scalar( 0, 255, 255 ), 1, 8 );
			resultsFile << xMin << "," << yMin << "," << width << "," << height << endl;
		}
		// Update
		else{
			result = tracker.update(frame);
			rectangle( frame, Point( result.x, result.y ), Point( result.x+result.width, result.y+result.height), Scalar( 0, 255, 255 ), 1, 8 );
			resultsFile << result.x << "," << result.y << "," << result.width << "," << result.height << endl;
		}

		nFrames++;

		if (!SILENT){
			imshow("Image", frame);
			waitKey(1);
		}
	}
	resultsFile.close();
    listFramesFile.close();
	//listFile.close();

}
