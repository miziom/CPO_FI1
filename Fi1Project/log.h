double countKernelCell(double sigma, int row, int col);

double** createKernel(int size, double sigma);

cv::Vec3b** returnRegionOfPixel(cv::Mat img, int kernelSize, int pixelRow, int pixelCol);

double sumOfKernel(double** kernel, int kernelSize);

double sumOfProducts(cv::Vec3b** regionOfPixel, double** kernel, int kernelSize, int pixelComponent);

int pixelComponentValue(double pixelComponent);

void LaplacianOfGaussian(cv::Mat& img, cv::Mat imgCopy, double sigma, int kernelSize);


