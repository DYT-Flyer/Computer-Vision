#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

void detectAndDisplay(Mat &frame, int k, int r);

String face_cascade_name = "C:/Users/Arik/Desktop/opencv/sources/data/haarcascades_cuda/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "C:/Users/Arik/Desktop/opencv/sources/data/haarcascades_cuda/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Capture - Face detection";
RNG rng(12345);

int main(int argc, const char** argv)
{
	VideoCapture cap;
	if (!cap.open(0))
		return 0;


	cout << "hey";
	//Loading cascades
	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading\n"); return -1; };
	if (!eyes_cascade.load(eyes_cascade_name)) { printf("--(!)Error loading\n"); return -1; };

	int k = 5;
	int r = 1;
	//reading video stream
	namedWindow("", WINDOW_AUTOSIZE);
		for(;;)
		{
			Mat frame;
			cap >> frame;

			//applying classifier
			if (!frame.empty() )

			{
				detectAndDisplay(frame, k, r);
			}

			else

			{
				printf(" --(!) No captured frame -- Break!"); break;
			}

			cout << "1";

			imshow(window_name, frame);
			waitKey(100);
			int c = waitKey(25);
			if (c == 27) {
				break;
			}



			//I or i increases by 1, unless k >= 5, then it increases by 5;
			//D or d decreases by 1, unless k > 5, then it decreases by 5;

			if (c == 68 || c == 100) {
				if (k == 0) {
					cout << "\nk = 0, can't decrease if there is not blur";
				}

				if (k < 6 && k > 0) {
					k = k - 1;
				}

				if (k > 5) {
					k = k - 5;
				}
			}

			if (c == 73 || c == 105) {
				if (k >= 5) {
					k = k + 5;
				}

				if (k < 5) {
					k = k + 1;
				}
			}
			//pressing r or R turns on/off filter
			if (c == 82 || c == 114) {
				if (r == 1) {
					r = 0;
				}
				else {
					r = 1;
				}
			}

			
		}


	return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay(Mat &frame, int k, int r)
{
	std::vector<Rect> faces;
	Mat frame_gray;

	//cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++)
	{
		//Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		//ellipse(frame, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

		//rectangle(frame, faces[i], Scalar(255, 55, 0), 1, 8, 0);

		int x = faces[i].x;
		int y = faces[i].y;
		//int mean = 0;
		if (r == 1) {
			if (k != 0) {
				int width = faces[i].width;
				width = width / k;
				int height = faces[i].height;
				height = height / k;

				for (int a = 0; a < k; a++) {
					for (int b = 0; b < k; b++) {

						//x,y,w,h
						Rect krect(faces[i].x + width * a, faces[i].y + height * b, width, height);

						Mat mosaic = frame(krect);
						Scalar mosaicmean = mean(mosaic, noArray());

						frame(krect).setTo(mosaicmean);

						//rectangle(frame, krect, Scalar(255, 0, 0), 1, 8, 0);
					}

				}
			}
		}

		
	}
		
	//-- Show what you got
	/*namedWindow("", WINDOW_AUTOSIZE);
	imshow(window_name, frame);
	waitKey(1);*/
}