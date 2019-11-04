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
	return 480 * result;
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
			guardCol++;
		}
		guardCol = 0;
		guardRow++;
	}
	return partOfImage;
}

double sumOfKernel(double** kernel, int kernelSize) {
	double sum = 0;
	for (int row = 0; row < kernelSize; row++) {
		for (int col = 0; col < kernelSize; col++) {
			sum += kernel[row][col];
		}
	}
	return sum;
}

double sumOfProducts(cv::Vec3b** regionOfPixel, double** kernel, int kernelSize, int pixelComponent) {
	double sumOfProd = 0;
	for (int row = 0; row < kernelSize; row++) {
		for (int col = 0; col < kernelSize; col++) {
			sumOfProd = sumOfProd + (regionOfPixel[row][col].val[pixelComponent] * kernel[row][col]);
		}
	}
	return sumOfProd;
}

int pixelComponentValue(double pixelComponent) {
	int pixelComponentValue;
	if (pixelComponent > 255) {
		return 255;
	}
	else if (pixelComponent < 0) {
		return 0;
	}
	else {
		pixelComponentValue = (int)round(pixelComponent);
	}
}

void LaplacianOfGaussian(cv::Mat &img, cv::Mat imgCopy, double sigma, int kernelSize) {

	int cols = imgCopy.size().width;
	int rows = imgCopy.size().height;
	cv::Vec3b** partOfImage;
	double** kernel;
	double kernelSum = 0;
	double sumOfProd = 0;
	int pixComponentValue = 0;
	//creating Kernel
	kernel = createKernel(kernelSize, sigma);
	kernelSum = sumOfKernel(kernel, kernelSize);

	for (int row = (kernelSize - 1) / 2; row < (rows - 1 - ((kernelSize - 1) / 2)); row++) {
		for (int col = (kernelSize - 1) / 2; col < (cols - 1 - ((kernelSize - 1) / 2)); col++) {
			partOfImage = returnRegionOfPixel(imgCopy, kernelSize, row, col);
			cv::Vec3b pixel;
			for (int i = 0; i < 3; i++) {
				double sum = sumOfProducts(partOfImage, kernel, kernelSize, i);
				double pixelComponentFirstValue = sum / kernelSum;
				int pixelComValue = pixelComponentValue(pixelComponentFirstValue);
				pixel.val[i] = pixelComValue;
			}
			img.at<cv::Vec3b>(row, col) = pixel;
		}
	}

}


int main()
{
	std::string imageName = "image.jpg";
	cv::Mat img = imread(imageName);
	cv::Mat imgCopy = imread(imageName);
	cv::Mat imgGray;
	cv::Mat imgGrayCopy;

	cv::Mat img3 = imread(imageName);

	cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
	cv::cvtColor(imgGray, imgGray, cv::COLOR_GRAY2BGR);

	cv::cvtColor(imgCopy, imgGrayCopy, cv::COLOR_BGR2GRAY);
	cv::cvtColor(imgGrayCopy, imgGrayCopy, cv::COLOR_GRAY2BGR);


	double sigma = 1.4;
	int kernelSize = 5;

	//LaplacianOfGaussian(img, imgCopy, sigma, kernelSize);
	//LaplacianOfGaussian(imgGray, imgGrayCopy, sigma, kernelSize);


	cv::GaussianBlur(img, img, Size(5, 5), 1.4, 0, BORDER_DEFAULT);
	//cv::GaussianBlur(imgGray, imgGray, Size(5, 5), 1.4, 0, BORDER_DEFAULT);

	cv::namedWindow("image", cv::WINDOW_NORMAL);
	cv::imshow("image", img);
	cv::waitKey(0);	

	return 0;
}
