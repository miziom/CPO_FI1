#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/world.hpp>
#include <opencv2/imgproc.hpp>
#include <iomanip>
#include <cmath>
#include <math.h>
#include "gui.h"
#include "log.h"

//loading images
void loadImage(cv::Mat &img, cv::Mat &imgCopy)
{
	std::string imageName = "image.jpg";
	img = cv::imread(imageName);
	imgCopy = cv::imread(imageName);
}

//check size of input string, if size = 0 return false
bool checkInpuSize(std::string input) {
	if (input.size() == 0) {
		return false;
	}
	else {
		return true;
	}
}

//check can it be odd number from string 
bool checkIsOddNumber(std::string input) {
	int number = std::stoi(input);
	if (number % 2 == 0) {
		return false;
	}
	else return true;
}

//check can it be int number from string
bool checkIsIntNumber(std::string input) {
	for (int i = 0; i < input.size(); i++) {
		if (!(input[i] >= '0' && input[i] <= '9')) {
			return false;
		}
	}
	return true;
}

//check can it be double from string
bool checkIsDoubleNumber(std::string input) {
	for (int i = 0; i < input.size(); i++) {
		if (!((input[i] >= '0' && input[i] <= '9') || input[i] == '.')) {
			return false;
		}
	}
	return true;
}

bool checkNumberOfDot(std::string input) {
	int counter = 0;
	for (int i = 0; i < input.size(); i++) {
		if (input[i] == '.') {
			counter++;
			if (counter >= 2) {
				return false;
			}
		}
	}
	return true;
}

//remove white spaces from string 
//void removeWhiteSpaces(std::string& input) {
//	input.erase(remove(input.begin(), input.end(), ' '), input.end());
//}

//remove white spaces from string 
void removeWhiteSpaces(std::string& input) {
	for (int i = input.size() - 1; i >= 0; i--) {
		if (input[i] == ' ') {
			input.erase(i,1);
		}
	}
}

//check can it be int which is greater than X 
bool checkIsIntGreaterEqualsThanX(std::string &input, int X) {
	removeWhiteSpaces(input);
	if (checkInpuSize(input) == false) {
		return false;
	}
	if (checkIsIntNumber(input) == false) {
		return false;
	}
	if (std::stoi(input) < X) {
		return false;
	}
	return true;
}

//check can it begin or end with dot '.'
bool checkBeginEndWithDot(std::string input) {
	if (input[0] == '.' || input[input.size() - 1] == '.') {
		return true;
	}
	return false;
}

//check can it be double which is greater/equals 0
bool checkIsDoublePositive(std::string &input) {
	removeWhiteSpaces(input);
	if (checkInpuSize(input) == false) {
		return false;
	}
	if (checkIsDoubleNumber(input) == false) {
		return false;
	}
	if (checkNumberOfDot(input) == false) {
		return false;
	}
	if (checkBeginEndWithDot(input) == true) {
		return false;
	}
	if (std::stod(input) <= 0.0) {
		return false;
	}
	return true;
}

//print error message
void printError() {
	std::cout << "Bledna dana" << std::endl;
}

//method which start program again with error message after press any key
void errorHandling() {
	printError();
	system("pause");
	system("CLS");
	program();
}

//check int can be in image up to kernel
bool checkIsInCorrectField(int kernelSize, int imageSize, int number) {
	int min = ((kernelSize - 1) / 2);
	int max = imageSize - 1 - ((kernelSize - 1) / 2);
	if (number < min) {
		return false;
	}
	else if (number > max) {
		return false;;
	}
	return true;
}

//count margin of image up to kernel
int marginCounting(int kernelSize) {
	return (kernelSize - 1) / 2;
}

//print message up to situation
void infoPrint(int number, int kernelSize, int imageWidth, int imageHeight, int xLeftTop, int yLeftTop) {
	int margin = marginCounting(kernelSize);
	switch (number)
	{
	case 0: {
		std::cout << "Mateusz Mizio" << std::endl;
		std::cout << "Temat Fi 1" << std::endl;
		std::cout << "Laplacian Of Gaussian" << std::endl;
	}break;
	case 1: {
		std::cout << "Podaj rozmiar jadra N [NxN] (N - liczba nieparzysta): ";
	}break;
	case 2: {
		std::cout << "Podaj wartosc wspolczynnika maski (sigma): ";
	}break;
	case 3: {
		std::cout << "Okresl ROI." << std::endl;
	}break;
	case 4: {
		std::cout << "Wartosc X pozycji lewego gornego rogu (Wartosc od " << margin << " do " << (imageWidth - 1 - margin) << ": ";
	}break;
	case 5: {
		std::cout << "Wartosc Y pozycji lewego gornego rogu (Wartosc od " << margin << " do " << (imageHeight - 1 - margin) << ": ";
	}break;
	case 6: {
		std::cout << "Wartosc X pozycji prawego dolnego rogu (Wartosc od " << xLeftTop << " do " << (imageWidth - 1 - margin) << ": ";
	}break;
	case 7: {
		std::cout << "Wartosc Y pozycji prawego dolnego rogu (Wartosc od " << yLeftTop << " do " << (imageHeight - 1 - margin) << ": ";
	}break;
	case 8: {
		std::cout << "Wybierz typ obrazu" << std::endl;
		std::cout << "1. Skala szarosci." << std::endl;
		std::cout << "2. Obraz RGB" << std::endl;
		std::cout << "Podaj numer: ";
	}break;
	case 9: {
		std::cout << "Obliczanie, moze to potrwac kilka minut...";
	}break;
	}
}

//check int can be in range
bool checkIsFromRange(int number, int min, int max) {
	if (number >= min && number <= max) {
		return true;
	}
	else {
		return false;
	}
}


//create empty int table 
int* createPointsTable() {
	int* table;
	table = new int[4];
	return table;
}

//check it can be point of Region Of Interest up to previous point
void checkROI(int &printCounter, int kernelSize, int imageWidth, int imageHeight,
	int &willContinue, int *&pointsTable, int &counter, bool &isX, int& xGuard, int& yGuard, int xMax, int yMax) {
	infoPrint(printCounter, kernelSize, imageWidth, imageHeight, xGuard, yGuard);
	printCounter++;
	std::string input;
	std::getline(std::cin, input);
	switch (checkIsIntGreaterEqualsThanX(input, 0)) {
	case true: {
		int output = stoi(input);
		if (isX) {
			if (checkIsFromRange(output, xGuard, xMax)) {
				pointsTable[counter] = output;
				counter++;
				isX = !isX;
				xGuard = output;
				if (willContinue < 4) {
					willContinue++;
					checkROI(printCounter, kernelSize, imageWidth, imageHeight, willContinue, pointsTable, counter, isX, xGuard, yGuard, xMax, yMax);
				}
				else
				{
					break;
				}
			}
			else
			{
				errorHandling();
			}
		}
		else
		{
			if (checkIsFromRange(output, yGuard, yMax)) {
				pointsTable[counter] = output;
				counter++;
				isX = !isX;
				yGuard = output;
				if (willContinue < 3) {
					willContinue++;
					checkROI(printCounter, kernelSize, imageWidth, imageHeight, willContinue, pointsTable, counter, isX, xGuard, yGuard, xMax, yMax);
				}
				else
				{
					break;
				}
			}
			else
			{
				errorHandling();
			}
		}
	}break;
	case false: {
		errorHandling();
	}break;
	}
}

//chect user deciosion (grayscale or rgb)
int grayOrRgb(std::string decision) {
	if (checkIsIntGreaterEqualsThanX(decision, 0)) {
		int number = std::stoi(decision);
		if (number == 1) {
			return 1;
		}
		else if (number == 2) {
			return 2;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

//display image img
void displayImage(cv::Mat img) {
	cv::namedWindow("image", cv::WINDOW_NORMAL);
	cv::imshow("image", img);
	cv::waitKey(0);
}

//display image after Log after user decision (gray/rgb)
void displayLaplacian(std::string decision, cv::Mat img, cv::Mat imgCopy, double sigma, int kernelSize, int xTopLeft, int yTopLeft, int xBottomRight, int yBottomRight) {
	switch (grayOrRgb(decision))
	{
	case 0: {
		errorHandling();
	}break;
	case 1: {
		infoPrint(9, 0, 0, 0, 0, 0);
		bgrToGrayscaleWeighted(img, imgCopy);
		bgrToGrayscaleWeighted(img, img);
		LaplacianOfGaussian(img, imgCopy, sigma, kernelSize, xTopLeft, yTopLeft, xBottomRight, yBottomRight);
		displayImage(img);
	}break;
	case 2: {
		infoPrint(9, 0, 0, 0, 0, 0);
		LaplacianOfGaussian(img, imgCopy, sigma, kernelSize, xTopLeft, yTopLeft, xBottomRight, yBottomRight);
		displayImage(img);
	}break;
	}
}

//method which start program
void program() {
	cv::Mat img;
	cv::Mat imgCopy;
	loadImage(img, imgCopy);
	infoPrint(0, 0, 0, 0, 0, 0);
	infoPrint(1, 0, 0, 0, 0, 0);
	std::string inputKernelSize;
	std::getline(std::cin, inputKernelSize);
	//get kernel size and check is correct
	if (checkIsIntGreaterEqualsThanX(inputKernelSize, 3) == false) {
		errorHandling();
	}
	else {
		if (checkIsOddNumber(inputKernelSize) == false) {
			errorHandling();
		}
		else {
			int kernelSize = std::stoi(inputKernelSize);
			infoPrint(2, 0, 0, 0, 0, 0);
			std::string inputSigma;
			std::getline(std::cin, inputSigma);
			//get sigma value and check is correct
			if (checkIsDoublePositive(inputSigma) == false) {
				errorHandling();
			}
			else {
				double sigma = std::stod(inputSigma);
				int willContinue = 0;
				int* tablePos = createPointsTable();
				int counter = 0;
				int printCounter = 4;
				bool isX = true;
				int xMax = imgCopy.size().width - 1 - marginCounting(kernelSize);
				int yMax = imgCopy.size().height - 1 - marginCounting(kernelSize);
				int xGuard = marginCounting(kernelSize);
				int yGuard = marginCounting(kernelSize);
				infoPrint(3, 0, 0, 0, 0, 0);
				checkROI(printCounter, kernelSize, imgCopy.size().width, imgCopy.size().height,
					willContinue, tablePos, counter, isX, xGuard, yGuard, xMax, yMax);		
				infoPrint(8, 0, 0, 0, 0, 0);
				std::string grayOrRgbDecision;
				std::getline(std::cin, grayOrRgbDecision);
				displayLaplacian(grayOrRgbDecision, img, imgCopy, sigma, kernelSize,
					tablePos[0], tablePos[1], tablePos[2], tablePos[3]);
			}
		}
	}
}