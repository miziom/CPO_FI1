#define _USE_MATH_DEFINES

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/world.hpp>
#include <opencv2/imgproc.hpp>
#include <iomanip>
#include <cmath>
#include <math.h>
#include "log.h"

//count cell for kernel
double countKernelCell(double sigma, int x, int y) {
	double index = -((pow(x, 2) + pow(y, 2)) / (2 * pow(sigma, 2)));
	double power = pow(M_E, index);
	double squareBracket = 1 + index;
	double denominator = M_PI * pow(sigma, 4);
	double result = -(1 / denominator) * squareBracket * power;
	return result;
}

//count x-x0
int normalizeXY(int rowOrCol, int size) {
	return (rowOrCol - ((size - 1) / 2));
}

//create and count kernel
double** createKernel(int size, double sigma) {
	//create empty kernel
	double** kernel;
	kernel = new double* [size];
	for (int i = 0; i < size; i++) {
		kernel[i] = new double[size];
	}
	//count values for kernel
	for (int row = 0; row < size; row++) {
		int y = normalizeXY(row, size);
		for (int col = 0; col < size; col++) {
			int x = normalizeXY(col, size);
			kernel[row][col] = countKernelCell(sigma, x, y);
		}
	}
	//print(kernel, size);
	return kernel;
}

//return part of image(values of pixels), which kernel will cover
cv::Vec3b** returnRegionOfPixel(cv::Mat img, int kernelSize, int pixelRow, int pixelCol) {
	//create empty region
	cv::Vec3b** partOfImage;
	partOfImage = new cv::Vec3b * [kernelSize];
	for (int i = 0; i < kernelSize; i++) {
		partOfImage[i] = new cv::Vec3b[kernelSize];
	}
	//set min and max position of region
	int maxMinNumber = (kernelSize - 1) / 2;
	int minPicselRow = pixelRow - maxMinNumber;
	int minPicselCol = pixelCol - maxMinNumber;
	int maxPicselRow = pixelRow + maxMinNumber;
	int maxPicselCol = pixelCol + maxMinNumber;
	//guards for set pos in partoOfImage
	int guardRow = 0;
	int guardCol = 0;
	//set partOfImage with pixel values of region
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

//count sum of kernel components
double sumOfKernel(double** kernel, int kernelSize) {
	double sum = 0;
	for (int row = 0; row < kernelSize; row++) {
		for (int col = 0; col < kernelSize; col++) {
			sum += kernel[row][col];
		}
	}
	return sum;
}

//count sum of pixel component values of RegionOfPixel multiplied by matched kernel pixel values 
double sumOfProducts(cv::Vec3b** regionOfPixel, double** kernel, int kernelSize, int pixelComponent) {
	double sumOfProd = 0;
	for (int row = 0; row < kernelSize; row++) {
		for (int col = 0; col < kernelSize; col++) {
			sumOfProd = sumOfProd + (regionOfPixel[row][col].val[pixelComponent] * kernel[row][col]);
		}
	}
	return sumOfProd;
}

//normalise value of pictures
int pixelComponentValue(double pixelComponent) {
	int pixelComponentValue;
	//if pixel component after counting is greater than 255, pixel equals 255
	if (pixelComponent > 255) {
		return 255;
	}
	//if pixel component after counting is smaller then 0, pixel equals 0
	if (pixelComponent < 0) {
		return 0;
	}
	////if pixel component after counting is between 0 and 255, pixel is rounded
	else {
		pixelComponentValue = (int)round(pixelComponent);
	}
}

//make from rgb image grayscale image use weighted method 
void bgrToGrayscaleWeighted(const cv::Mat inputImage, cv::Mat &outputImage) {
	//check is it 3 channels imag (RGB)
	if (inputImage.channels() != 3) {
		throw(std::invalid_argument("Obraz wejsciowy powinien byc 3 kanalowy"));
	}
	//check is it CV_8UC3 type image
	if (inputImage.type() != CV_8UC3) {
		throw(std::invalid_argument("Obrazek wejsciowy powinien byc 8 bitowy"));
	}
	//create output image (empty)
	outputImage = cv::Mat(inputImage.rows, inputImage.cols, CV_8UC3);
	for (int row = 0; row < inputImage.rows; row++) {
		for (int col = 0; col < inputImage.cols; col++) {
			//save pixel
			cv::Vec3b pixelValue = inputImage.at<cv::Vec3b>(row, col);
			//count grayscale value
			int value = (int)round(pixelValue[0] * 0.11 + pixelValue[1] * 0.59 + pixelValue[2] * 0.3);
			cv::Vec3b outputPixel;
			//create outpu 3 channels grayscale pixel
			for (int i = 0; i < 3; i++) {
				outputPixel[i] = value;
			}
			//add pixel to output image
			outputImage.at<cv::Vec3b>(row, col) = outputPixel;
		}
	}
}

//laplacian algorithm working on ROI    , int minXroi, int maxXroi, int minYroi, int maxYroi
void LaplacianOfGaussian(cv::Mat& img, cv::Mat imgCopy, double sigma, int kernelSize, int xTopLeft, int yTopLeft, int xBottomRight, int yBottomRight) {

	//get size of image
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

	for (int row = yTopLeft; row <= yBottomRight; row++) {
		for (int col = xTopLeft; col <= xBottomRight; col++) {
			//return part of image, which is needed to LoG
			partOfImage = returnRegionOfPixel(imgCopy, kernelSize, row, col);
			cv::Vec3b pixel;
			//couting for 3 channels
			for (int i = 0; i < 3; i++) {
				//counting sum of products (sum of products kernel and match pixel value)
				double sum = sumOfProducts(partOfImage, kernel, kernelSize, i);
				//counting value of pixel in double
				double pixelComponentFirstValue = sum / kernelSum;
				//convert double to int
				int pixelComValue = pixelComponentValue(pixelComponentFirstValue);
				//set pixel channel
				pixel.val[i] = pixelComValue;
			}
			//change pixel in image to new pixel
			img.at<cv::Vec3b>(row, col) = pixel;
		}
	}
}