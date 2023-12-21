// File to test the camera and C++ OpenCV libraries
// compile with
// g++ $(pkg-config --libs --cflags opencv) -o camera_test main.cpp
// run with
// ./cameratest

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "camera.h"


using namespace cv;
using namespace std;

int collected;

static int coll =0;
void camera_detect( )
{
  
	
	
	collected = 0;
	
	VideoCapture cap(0);
	
  if (!cap.isOpened()) {
    cerr << "ERROR: Unable to open the camera" << endl;
    //return 0;
  }

  Mat frame;
  while(!collected) {
    cap >> frame;
    Rect crop_frames(0, 120, frame.cols, frame.rows - 120);
    frame = frame(crop_frames);
    if (frame.empty()) {
        cerr << "ERROR: Unable to grab from the camera" << endl;
        break;
    }
    Mat frame_hsv;
    
    cvtColor(frame,frame_hsv, COLOR_BGR2HSV);
    Mat mask1,mask, mask2;
    inRange(frame_hsv,Scalar(116, 142, 0),Scalar(139, 255, 255),mask);
    cout <<"Inside camera function\n";
	int whitePixelCount = 0;
	MatConstIterator_<uchar> it = mask.begin<uchar>();
	MatConstIterator_<uchar> end = mask.end<uchar>();
	for (; it != end; ++it)
		{
			if (*it == 255)  // Assuming white pixels have an intensity value of 255
				++whitePixelCount;
				
			}
	 cout << "whitePixelCount: "<< whitePixelCount << endl;

	 if (whitePixelCount >4000)
		{
		cout << "object detected\n";	
		coll+=1;
		if (coll >50)
		{
			cout << "breaking " << coll << endl;	
			collected=1;
			coll =0;
			
			break;
		}
	}

		int key = cv::waitKey(5);
		key = (key==255) ? -1 : key; //#Solve bug in 3.2.0
		if (key>=0)
		  break;
	  
  }
  cout << "Closing the camera" << endl;
  cap.release();
  destroyAllWindows();
  cout << "bye!" <<endl;
  //return 0;
  
}



