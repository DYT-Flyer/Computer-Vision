#include <stdio.h> 
#include <iostream> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

void k_Means(const Mat img, int k, int num_sample, int iterations, Mat &km_img) {
	cout << "start";
	RNG rng(12345);
	//cout << "Start";
	//initializing number of sample pixels vector
	vector<vector<int>> sample(num_sample, vector<int>(2, 0));

	//need to update if k changes
	//centroids
	vector<vector<int>> ck(k, vector<int>(2, 0));
	cout << "Starting";
	//initializing sample points
	for (int a = 0; a < num_sample; a++) {
		int sx = rng.uniform(1, img.cols);
		int sy = rng.uniform(1, img.rows);

		//checking for duplicates, if so: regenerate and check list again
		for (int b = 0; b < num_sample; b++) {
			if (sx == sample[b][0] && sy == sample[b][1]) {
				int sx = rng.uniform(1, img.cols);
				int sy = rng.uniform(1, img.rows);
				b = 0;
			}
		}

		sample[a][0] = sx;
		sample[a][1] = sy;
		cout << sample[a][1];
	}
	cout << "initializing";
	//initializing centroids
	for (int a = 0; a < k; a++) {
		int cx = rng.uniform(1, img.cols);
		int cy = rng.uniform(1, img.rows);

		for (int b = 0; b < k; b++) {
			if (cx == sample[b][0] && cy == sample[b][1]) {
				cx = rng.uniform(1, img.cols);
				cy = rng.uniform(1, img.rows);
				b = 0;
			}
		}

		ck[a][0] = cx;
		ck[a][1] = cy;
	}

	int first = 0;
	vector<int> cluster(num_sample, 0);
	vector<int> clusterprev(num_sample, 0);

	//initializing array cmean, need to change with k
	vector<vector<int>> cmean(k, vector<int>(2, 1));
	vector<int> ccount(k, 1);
	vector<int> cmeanvalues(k, 0);

	

	int framecount = 1;
	int i;

	cout << "loop";
	for (i = 0; i < iterations; i++) {
		//cout << "loop";
		vector<vector<int>> csum(k, vector<int>(2, 1));

		//restarting count
		for (int b = 0; b < k; b++) {
			ccount[b] = 1;
		}

		//finding distance and assigning to each cluster
		

		for (int a = 0; a < num_sample; a++) {
			

			//initializing min dist
			int min = 0;

			double base1 = sample[a][0] - ck[0][0];
			double base2 = sample[a][1] - ck[0][1];
			double power = 2.0;
			double pow1 = pow(base1, power);
			double pow2 = pow(base2, power);
			double dist = sqrt(pow1 + pow2);

			

			//finding min dist through k
			for (int b = 1; b < k; b++) {
				base1 = sample[a][0] - ck[b][0];
				base2 = sample[a][1] - ck[b][1];
				power = 2.0;
				pow1 = pow(base1, power);
				pow2 = pow(base2, power);
				double distTemp = sqrt(pow1 + pow2);

				if (distTemp < dist) {
					dist = distTemp;
					min = b;
				}
				

			}

			
			cluster[a] = min;
		}

		/*if (first == 0) {
		for (int a = 0; a < num_sample; a++) {
		clusterprev[a] = cluster[a];
		}
		}*/

		if (first > 1) {

		int count = 0;
		
		
		

		/*for (int a = 0; a < num_sample; a++) {
		if (cluster[a] == clusterprev[a]) {
		count++;
		}
		if (count == num_sample) {
		cout << "Break 1";
		break;
		}
		}*/
		}

		//copying cluster assignment to cluster prev

		for (int a = 0; a < num_sample; a++) {

			clusterprev[a] = cluster[a];
		}



		//finding cluster counts, preparing averages
		for (int a = 0; a < num_sample; a++) {
			
			for (int b = 0; b < k; b++) {

				

				if (cluster[a] == b) {
					csum[b][0] = csum[b][0] + sample[a][0];
					csum[b][1] = csum[b][1] + sample[a][1];

					cmeanvalues[b] = cmeanvalues[b] + img.ptr<uchar>(sample[a][1])[sample[a][0]];

					
					ccount[b] = ccount[b] + 1;
					
				}
			}
		}

		//calculating means, updating centroids
		for (int b = 0; b < k; b++) {
			
			cmean[b][0] = csum[b][0] / ccount[b];
			ck[b][0] = cmean[b][0];
			cmean[b][1] = csum[b][1] / ccount[b];
			ck[b][1] = cmean[b][1];
			cmeanvalues[b] = cmeanvalues[b] / ccount[b];

			
		}


		first++;


		//updating km_img
		for (int x = 1; x < img.cols*3; x++) {
			for (int y = 1; y < img.rows; y++) {
				int min_pixel = abs(img.ptr<uchar>(y)[x] - cmeanvalues[0]);
				int min_num = 0;

				for (int b = 1; b < k; b++) {
					int min_pixel_temp = abs(img.ptr<uchar>(y)[x] - cmeanvalues[b]);
					if (min_pixel_temp < min_pixel) {
						min_pixel = min_pixel_temp;
						min_num = b;
					}
				}
				km_img.ptr<uchar>(y)[x] = cmeanvalues[min_num];
			}
		}


		/*namedWindow("", WINDOW_AUTOSIZE);
		imshow("Kmeans", km_img);*/

		if (waitKey(0) == 27) {
			cout << "\nBreak 2";
			break;
		}

		/*cout << "\nFramecount: ";
		cout << framecount;
		cout << "\n";*/
		framecount++;
	}
	vector<double>means(k, 0);
	for (int d = 0; d < k; d++) {
		means[d] = cmeanvalues[d];
	}
	cout << "finished";
	return;
}

void expMax(const Mat img, int k, int iterations, Mat &em_img) {
	cout << "ExP";
	vector<double>sums(k, 0);
	vector<double>weights(k, 0);
	vector<double>distances(k, 0);
	vector<double>weightacc(k, 0);
	vector<double>means(k, 50);
	vector<double>stddevs(k, 1);
	double n = img.rows*img.cols;

	cout << img.cols;

	//filling means and stddevs
	RNG rng(12345);
	for (int d = 0; d < k; d++) {
		means[d] = rng.uniform(0, 255);
		stddevs[d] = rng.uniform(1, 4);
	}

	for (int i = 0; i < iterations; i++) {

		//cout << "i";
		double sum = 0;
		for (int a = 0; a < k; a++) {
			sums[a] = 0.0;
			stddevs[a] = 0.0;
		}
		
		for (int y = 0; y < img.rows; y++) {
			for (int x = 0; x < img.cols; x++) {

				for (int b = 0; b < k; b++) {
					stddevs[b] += pow((double)img.ptr<uchar>(y)[x] - means[b], 2.0);
					distances[b] = abs((double)img.ptr<uchar>(y)[x] - means[b]);
					sum += distances[b];
				}

				for (int c = 0; c < k; c++) {
					weights[c] = distances[c] / sum;
					sums[c] += weights[c] * (double)img.ptr<uchar>(y)[x];
					weightacc[c] += weights[c];
				}



			}
		}
		//recalculating means
		for (int d = 0; d < k; d++) {
			means[d] = sums[d] / weightacc[d];
			/*cout << means[d];
			cout << "\n";*/
		}
	}

	//calculate final standard deviations
	for (int d = 0; d < k; d++) {
		//cout << "\nstddevs before: ";
		//cout << stddevs[d];
		stddevs[d] = sqrt(stddevs[d] / n);
		//cout << "\nafter: ";
		//cout << stddevs[d];
	}

	double e = 2.71828;
	double pi = 3.14159;
	vector<double> histprob(k,0);

	//calculating gaussian values, comparing values, applying values to image
	int x1 = 0;
	int y1 = 0;
	for (int x = 0; x < img.cols*3; x++) {
		for (int y = 0; y < img.rows; y++) {
			for (int d = 0; d < k; d++) {
				

				histprob[d] = (1 / (stddevs[d] * sqrt(2 * pi)))*pow(e, ((-1 * pow(img.ptr<uchar>(y)[x] - means[d], 2)) / (2 * pow(stddevs[d], 2))));
			}
			int max = 0;
			double maxprob = histprob[0];
			

			for (int d = 1; d < k; d++) {
				
				if (histprob[d] > maxprob) {
					maxprob = histprob[d];
					max = d;
				}
			}
			em_img.ptr<uchar>(y)[x] = (int) means[max];
			//em_img.ptr<uchar>(y)[x] = 10;
			x1 = x;
			y1 = y;
		}
		/*for (;;) {
			namedWindow("Assignment 2", WINDOW_AUTOSIZE);
			imshow("Expecation Maximization", em_img);
			
			if (cv::waitKey(0) == 27)
				break;
		}
		cout << x;*/
	}
	cout << x1;
	cout << ", ";
	cout << y1;
}


void cannyEdge(const Mat img, Mat &can_img) {
	double dGaus[3][3] = { { 1,2,1 },{ 2,4,2 },{ 1,2,1 } };
	Mat kGaus = Mat(3, 3, CV_64FC1, dGaus)*((double)1 / 16);
	double dVEdge[3][3] = { { -1,0,1 },{ -2,0,2 },{ -1,0,1 } };
	Mat kVEdge = Mat(3, 3, CV_64FC1, dVEdge);
	double dHEdge[3][3] = { { -1,-2,-1 },{ 0,0,0 },{ 1,2,1 } };
	Mat kHEdge = Mat(3, 3, CV_64FC1, dHEdge);
	
	Mat ix;
	Mat iy;
	Mat ii;
	Mat itheta;
	Mat is;
	Mat id;
	Mat ie;

	can_img.copyTo(ix);
	can_img.copyTo(iy);
	can_img.copyTo(ii);
	can_img.copyTo(itheta);
	can_img.copyTo(is);
	can_img.copyTo(id);
	can_img.copyTo(ie);


	//applying gaussian, sobel kernels
	filter2D(ix, ix, -1, kGaus, Point(-1, -1), 0, BORDER_DEFAULT);
	filter2D(iy, iy, -1, kGaus, Point(-1, -1), 0, BORDER_DEFAULT);
	filter2D(ix, ix, -1, kVEdge, Point(-1, -1), 0, BORDER_DEFAULT);
	filter2D(iy, iy, -1, kHEdge, Point(-1, -1), 0, BORDER_DEFAULT);

	int mean = 0;

	//rounding direction to itheta
	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols*3; x++) {
		
			//calculating intensities, gradients
			ii.ptr<uchar>(y)[x] = sqrt(pow(ix.ptr<uchar>(y)[x], 2) + pow(iy.ptr<uchar>(y)[x], 2));
			
			itheta.ptr<uchar>(y)[x] = atan2(iy.ptr<uchar>(y)[x],ix.ptr<uchar>(y)[x])*(180 / 3.14159);

			if (itheta.ptr<uchar>(y)[x] > 180) {
				itheta.ptr<uchar>(y)[x] = itheta.ptr<uchar>(y)[x] % 180;
			}
			
			vector<int> directions = { 0, 45, 90, 135 };
			
			int min = 0;
			int minvalue = abs(itheta.ptr<uchar>(y)[x] - directions[0]);

			for (int i = 1; i < 4; i++) {
				
				if (abs(itheta.ptr<uchar>(y)[x] - directions[i]) < minvalue) {
					
					minvalue = abs(itheta.ptr<uchar>(y)[x] - directions[i]);
					min = i;
				}
			}


			if (x != 0 && x != img.cols - 1 && y != 0 && y != img.rows - 1) {
				if (min == 0) {
					if (ii.ptr<uchar>(y)[x] > ii.ptr<uchar>(y)[x + 1] && ii.ptr<uchar>(y)[x] > ii.ptr<uchar>(y)[x - 1]) {
						is.ptr<uchar>(y)[x] = 1;
						
					}
					else {
						is.ptr<uchar>(y)[x] = 0;
						
					}
				}

				if (min == 1) {
					if (ii.ptr<uchar>(y)[x] > ii.ptr<uchar>(y - 1)[x + 1] && ii.ptr<uchar>(y)[x] > ii.ptr<uchar>(y + 1)[x - 1]) {
						is.ptr<uchar>(y)[x] = 1;
					}
					else {
						is.ptr<uchar>(y)[x] = 0;
					}
				}

				if (min == 2) {
					if (ii.ptr<uchar>(y)[x] > ii.ptr<uchar>(y - 1)[x] && ii.ptr<uchar>(y)[x] > ii.ptr<uchar>(y + 1)[x]) {
						is.ptr<uchar>(y)[x] = 1;
					}
					else {
						is.ptr<uchar>(y)[x] = 0;
					}
				}

				if (min == 3) {
					if (ii.ptr<uchar>(y)[x] > ii.ptr<uchar>(y + 1)[x + 1] && ii.ptr<uchar>(y)[x] > ii.ptr<uchar>(y - 1)[x - 1]) {
						is.ptr<uchar>(y)[x] = 1;
					}
					else {
						is.ptr<uchar>(y)[x] = 0;
					}
				}

			}
			mean += ii.ptr<uchar>(y)[x];
		}
		
	}
	//comparing to thresholds, updating image
	mean = mean / (img.cols * img.rows);
	int max_threshold = mean * 1.3;
	int low_threshold = mean * .6;

	for (int x = 1; x < img.cols*3-1; x++) {
		for (int y = 1; y < img.rows-1; y++) {
			if (is.ptr<uchar>(y)[x] == 1 && ii.ptr<uchar>(y)[x] >= max_threshold) {
				can_img.ptr<uchar>(y)[x] = 255;
			}
			else if (is.ptr<uchar>(y)[x] == 1) {
				
				for (int a = -1; a <= 1; a++) {
					for (int b = -1; b <= 1; b++) {
						if (a == 0 && b == 0) {
							can_img.ptr<uchar>(y)[x] = 0;
						}
						else {
							if (ii.ptr<uchar>(y + b)[x + a] >= max_threshold) {
								can_img.ptr<uchar>(y)[x] = 255;
							}
						}
					}
				}
			
			}
			else {
				can_img.ptr<uchar>(y)[x] = 0;
			}
		}
	}
}

void hough(const Mat img, Mat &hough_img) {
	
	Mat prac;
	img.copyTo(prac);
	cvtColor(prac, prac, COLOR_BGR2GRAY);
	GaussianBlur(prac, prac, Size(9, 9), 2, 2);
	vector<Vec3f> circles;
	HoughCircles(prac, circles, HOUGH_GRADIENT, 1, prac.rows / 8, 200, 100, 0, 0);
	
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		 //draw the circle center
		circle(prac, center, 3, Scalar(0,255,0), -1, 8, 0);
		 //draw the circle outline
		circle(prac, center, radius, Scalar(0,0,255), 3, 8, 0);
	}
	prac.copyTo(hough_img);
}

int main(int argc, char** argv)
{
	printf("Project Assignment2 by Thomas Sherk");
	
	cv::Mat img;

	img = cv::imread("C:\\Users\\Thoma\\Dropbox\\Isidore\\Computer Vision\\openCV\\openCV\\Original_Image.jpg",1);
	resize(img, img,cv::Size(600, 600));
	cout << "resized";
	cv::Mat km_img;
	cv::Mat em_img;
	cv::Mat can_img;
	cv::Mat hough_img;

	img.copyTo(km_img);
	img.copyTo(em_img);
	img.copyTo(can_img);
	img.copyTo(hough_img);

	int iterations = 1;
	int k = 199;
	cout << "kmeans";
	k_Means(img, k, 200, iterations, km_img);
	cout << "ExpMax";
	expMax(img, k, iterations, em_img);
	cout << "CannyEdge";
	cannyEdge(img, can_img);
	hough(img, hough_img);

	for (;;) {
		
		namedWindow("Assignment 2", WINDOW_AUTOSIZE);
		imshow("Original", img);
		imshow("K-Means", km_img);
		imshow("Expecation Maximization", em_img);
		imshow("Canny Edges", can_img);
		imshow("Hough Detection", hough_img);

		if (cv::waitKey(0) == 27)
			break;
	}
	return 0;
}


