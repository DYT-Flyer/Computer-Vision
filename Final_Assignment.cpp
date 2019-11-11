#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/core/cvstd_wrapper.hpp>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

int notmain(int argc, char** argv)
{
	cv::Mat image01a;
	cv::Mat image01b;
	cv::Mat image02a;
	cv::Mat image02b;
	cv::Mat image03a;
	cv::Mat image03b;
	cv::Mat image04a;
	cv::Mat image04b;
	cv::Mat image05a;
	cv::Mat image05b;
	image01a = cv::imread("C:/Users/thoma/Desktop/imgs/image01a.jpg");
	image01b = cv::imread("C:/Users/thoma/Desktop/imgs/image01b.jpg");
	image02a = cv::imread("C:/Users/thoma/Desktop/imgs/image02a.jpg");
	image02b = cv::imread("C:/Users/thoma/Desktop/imgs/image02b.jpg");
	image03a = cv::imread("C:/Users/thoma/Desktop/imgs/image03a.jpg");
	image03b = cv::imread("C:/Users/thoma/Desktop/imgs/image03b.jpg");
	image04a = cv::imread("C:/Users/thoma/Desktop/imgs/image04a.jpg");
	image04b = cv::imread("C:/Users/thoma/Desktop/imgs/image04b.jpg");
	image05a = cv::imread("C:/Users/thoma/Desktop/imgs/image05a.jpg");
	image05b = cv::imread("C:/Users/thoma/Desktop/imgs/image05b.jpg");

	cv::resize(image01a, image01a,cv::Size(),.15,.15);
	cv::resize(image01b, image01b,cv::Size(),.15,.15);
	cv::resize(image02a, image02a,cv::Size(),.15,.15);
	cv::resize(image02b, image02b,cv::Size(),.15,.15);
	cv::resize(image03a, image03a,cv::Size(),.15,.15);
	cv::resize(image03b, image03b,cv::Size(),.15,.15);
	cv::resize(image04a, image04a,cv::Size(),.15,.15);
	cv::resize(image04b, image04b,cv::Size(),.15,.15);
	cv::resize(image05a, image05a,cv::Size(),.15,.15);
	cv::resize(image05b, image05b,cv::Size(),.15,.15);
	int Key = 1;
	Mat imga;
	Mat imgb;
	int drawsift = 0;
	int drawsiftt = 0;
	Mat imgc;
	int ckey = 0;
	std::vector<KeyPoint> keypoints_1, keypoints_2;
	std::vector<DMatch> matches;
	
	for (;;)
	{
		if (Key == 1) {
			imga = image01a.clone();
			imgb = image01b.clone();
		}

		if (Key == 2) {
			imga = image02a.clone();
			imgb = image02b.clone();
		}

		if (Key == 3) {
			imga = image03a.clone();
			imgb = image03b.clone();
		}

		if (Key == 4) {
			imga = image04a.clone();
			imgb = image04b.clone();
		}

		if (Key == 5) {
			imga = image05a.clone();
			imgb = image05b.clone();
		}

		if (drawsift == 1) {
			if (drawsiftt == 1){
				//cv::xfeatures2d::SiftFeatureDetector f2d;
				//FeatureDetector* detector =cv::xfeatures2d::SIFT:
				//Ptr<SIFT> f2d = SIFT::create();
				//Ptr<Feature2D> sift = SIFT::create();
				//f2d = cv::xfeatures2d::SIFT::create();
				/*std::vector<KeyPoint> keypoints_1, keypoints_2;
				f2d->detect(imga, keypoints_1);
				f2d->detect(imgb, keypoints_2);
				Mat descriptors_1, descriptors_2;
				f2d->compute(imga, keypoints_1, descriptors_1);
				f2d->compute(imgb, keypoints_2, descriptors_2);
				BFMatcher matcher;
				std::vector<DMatch> matches;
				matcher.match(descriptors_1, descriptors_2, matches);
				drawsiftt = 0;*/
			}
			//drawMatches(imga, keypoints_1, imgb, keypoints_2, matches, imgc, Scalar::all(-1), Scalar::all(-1));
		}

		if (ckey == 0) {
			cv::namedWindow("Display Window1", WINDOW_AUTOSIZE);
			cv::imshow("Display Window1", imga);
			cv::namedWindow("Display Window2", WINDOW_AUTOSIZE);
			cv::imshow("Display Window 2", imgb);
			cv::waitKey(0);
		}
		else {
			cv::namedWindow("Display Window1", WINDOW_AUTOSIZE);
			cv::imshow("Display Window1", imgc);
		}
		if ((char)27 == (char)waitKey(0))break;
		if ((char)110 == (char)waitKey(0)) {
			if (Key == 5) {
				Key = 1;
			}
			else {
				Key = Key + 1;
			}
		}

		if ((char)112 == (char)waitKey(0)) {
			if (Key == 1) {
				Key = 5;
			}
			else {
				Key = Key - 1;
			}
		}

		if ((char)100 == (char)waitKey(0)) {
			if (drawsift == 1) {
				drawsift = 0;
				ckey = 0;
			}
			else {
				drawsift = 1;
				drawsiftt = 1;
			}
			
		}



		


	}
	return 0;

}