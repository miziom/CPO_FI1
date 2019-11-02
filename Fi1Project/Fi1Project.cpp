#define _USE_MATH_DEFINES

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/world.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <math.h>

using namespace std;
using namespace cv;

//double countKernelCell(double sigma, int row, int col) {
//
//	double root = sqrt(2 * M_PI * pow(sigma, 2));
//	double index = -( (pow(col, 2) * pow(row, 2)) / (2 * pow(sigma, 2)) );
//	double power = pow(M_E, index);
//	double result = power / root;
//	return result;
//}

double countKernelCell(double sigma, int row, int col) {
	double index = -((pow(col, 2) * pow(row, 2)) / (2 * pow(sigma, 2)));
	double power = pow(M_E, index);
	double squereBracket = 1 + index;
	double denominator = M_PI * pow(sigma, 4);
	double result = -(1 / denominator) * squereBracket * power;
	return result;
}

double **createKernel(int size, double sigma) {
	//create empty kernel
	double** kernel;
	kernel = new double*[size];
	for (int i = 0; i < size; i++) {
		kernel[i] = new double[size];
	}

	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			kernel[row][col] = countKernelCell(sigma, row, col);
		}
	}
	return kernel;
}

cv::Vec3b** returnRegionOfPixel(cv::Mat img, int kernelSize, int pixelRow, int pixelCol) {

	cv::Vec3b** partOfImage;
	partOfImage = new cv::Vec3b * [kernelSize];
	for (int i = 0; i < kernelSize; i++) {
		partOfImage[i] = new cv::Vec3b[kernelSize];
	}
	int maxMinNumber = (kernelSize - 1) / 2;
	int minPicselRow = pixelRow - maxMinNumber;
	int minPicselCol = pixelCol - maxMinNumber;
	int maxPicselRow = pixelRow + maxMinNumber;
	int maxPicselCol = pixelCol + maxMinNumber;
	int guardRow = 0;
	int guardCol = 0;
	for (int row = minPicselRow; row <= maxPicselRow; row++) {
		for (int col = minPicselCol; col <= maxPicselCol; col++) {
			partOfImage[guardRow][guardCol] = img.at<cv::Vec3b>(row, col);
		}
	}
	return partOfImage;
}


int main()
{
	cv::Mat img = imread("image.jpg");
	cv::Mat imgGray;
	cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
	int cols = imgGray.size().width;
	int rows = imgGray.size().height;
	double sigma = 1.4;
	int kernelSize = 3;
	cv::Vec3b** partOfImage;

	partOfImage = new cv::Vec3b*[kernelSize];
	for (int row = (kernelSize - 1) / 2; row < (rows - 1 - ((kernelSize - 1) / 2)); row++) {
		for (int col = (kernelSize - 1) / 2; col < (cols - 1 - ((kernelSize - 1) / 2)); col++) {

		}
	}

	cv::namedWindow("image", cv::WINDOW_NORMAL);
	cv::imshow("image", img);
	cv::waitKey(0);
	return 0;
}
