
#include <stdio.h> //create a file, this is a default library
#include <iostream> //system library


//Below are the three important OpenCV library header files
#include <opencv2/core/core.hpp>  //the very basics tool for image processing
#include <opencv2/highgui/highgui.hpp>   //for input/output, e.g. load the image into RAM
//show the image to the screen
#include <opencv2/imgproc/imgproc.hpp>  //some advanced tool for image processing


using namespace cv; //tell the compiler all the types involved are from OpenCV library


int main(int argc, char** argv)
{
	printf("Project Photobooth by Thomas Sherk");
	VideoCapture cap;
	//initializing kernels
	double dOrig[3][3] = { { 0,0,0 },{ 0,1,0 },{ 0,0,0 } };
	Mat kOrig = Mat(3, 3, CV_64FC1, dOrig);
	double dGaus[3][3] = { { 1,2,1 },{ 2,4,2 },{ 1,2,1 } };
	Mat kGaus = Mat(3, 3, CV_64FC1, dGaus)*((double)1 / 16);
	double dMean[3][3] = { { 1,1,1 },{ 1,1,1 },{ 1,1,1 } };
	Mat kMean = Mat(3, 3, CV_64FC1, dMean)*((double)1 / 9);
	double dEdge[3][3] = { { -1,-1,-1 },{ -1,8,-1 },{ -1,-1,-1 } };
	Mat kEdge = Mat(3, 3, CV_64FC1, dEdge);
	double dVEdge[3][3] = { { -1,0,1 },{ -2,0,2 },{ -1,0,1 } };
	Mat kVEdge = Mat(3, 3, CV_64FC1, dVEdge);
	double dHEdge[3][3] = { { -1,-2,-1 },{ 0,0,0 },{ 1,2,1 } };
	Mat kHEdge = Mat(3, 3, CV_64FC1, dHEdge);
	double dSharp[3][3] = { { 0,-1,0 },{ -1,5,-1 },{ 0,-1,0 } };
	Mat kSharp = Mat(3, 3, CV_64FC1, dSharp);


	if (!cap.open(0))
		return 0;

	//variable that indicates which filter to be used
	int c = 0;


	////for loop, copies video to MAT 'frame', applies filter through filter2D every iteration and then shows above video capture
	//either my computer is slow or something else, the waitkeys need to be held down for a little bit to work
	

	namedWindow("", WINDOW_AUTOSIZE);
	for (;;) {
		Mat frame;
		cap >> frame;

		resize(frame, frame, Size(frame.cols / 2, frame.rows / 2)); //resize the image to half size


		if (frame.empty()) break;
		//Gaussian
		if (c == 1) {
			filter2D(frame, frame, -1, kGaus, Point(-1, -1), 0, BORDER_DEFAULT);
		}
		//mean
		if (c == 2) {
			filter2D(frame, frame, -1, kMean, Point(-1, -1), 0, BORDER_DEFAULT);
		}
		//edge
		if (c == 3) {
			filter2D(frame, frame, -1, kEdge, Point(-1, -1), 0, BORDER_DEFAULT);
		}
		//vertical edge
		if (c == 4) {
			filter2D(frame, frame, -1, kVEdge, Point(-1, -1), 0, BORDER_DEFAULT);
		}
		//horizontal edge
		if (c == 5) {
			filter2D(frame, frame, -1, kHEdge, Point(-1, -1), 0, BORDER_DEFAULT);
		}
		//sharpen
		if (c == 6) {
			filter2D(frame, frame, -1, kSharp, Point(-1, -1), 0, BORDER_DEFAULT);
		}

		if (c == 7) {
			//this is to compare to the other blurring effects
			blur(frame, frame, Size(3, 3), Point(-1, -1), 4);
		}

		imshow("i-orig, g-gaus, m-mean, e-edge, v-vedge, h-hedge, s-sharp", frame);
		

		//original
		if (waitKey(1) == 105) {
			c = 0;
			printf("Original\n");
		}
		//gaussian blur
		if (waitKey(1) == 103) {
			c = 1;
			printf("Gaussian\n");
		}
		//mean/average blur
		if (waitKey(1) == 109) {
			c = 2;
			printf("Mean\n");
		}
		//ordinary edge
		if (waitKey(1) == 101) {
			c = 3;
			printf("Ordinary Edge\n");
		}
		//vertical edge - sobel filter
		if (waitKey(1) == 118) {
			c = 4;
			printf("VEdge\n");
		}
		//horizontal edge - sobel filter
		if (waitKey(1) == 104) {
			c = 5;
			printf("HEdge\n");
		}
		//sharpen effect
		if (waitKey(1) == 115) {
			c = 6;
			printf("Sharpen\n");
		}
		//grey out
		if (waitKey(1) == 97) {
			c = 7;
			printf("Grey\n");
		}

		/*int d = waitKey(0);
		if (d == 27) {
			break;
		}
*/
	}


	return 0;


}


