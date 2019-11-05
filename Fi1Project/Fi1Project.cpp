#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/world.hpp>
#include <opencv2/imgproc.hpp>
#include <iomanip>
#include <cmath>
#include <math.h>
#include <iostream>
#include "log.h"

using namespace std;
using namespace cv;

int main()
{
	std::string imageName = "image.jpg";
	cv::Mat img = imread(imageName);
	cv::Mat imgCopy = imread(imageName);
	cv::Mat imgGray;
	cv::Mat imgGrayCopy;

	cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
	cv::cvtColor(imgGray, imgGray, cv::COLOR_GRAY2BGR);

	cv::cvtColor(imgCopy, imgGrayCopy, cv::COLOR_BGR2GRAY);
	cv::cvtColor(imgGrayCopy, imgGrayCopy, cv::COLOR_GRAY2BGR);


	double sigma = 1.4;
	int kernelSize = 9;

	//LaplacianOfGaussian(img, imgCopy, sigma, kernelSize);
	LaplacianOfGaussian(imgGray, imgGrayCopy, sigma, kernelSize);

	cv::namedWindow("image", cv::WINDOW_NORMAL);
	//cv::imshow("image", img);
	cv::imshow("image", imgGray);
	cv::waitKey(0);	

	return 0;
}