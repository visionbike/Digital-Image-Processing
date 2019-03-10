#include "gaussian_filter.h"

int* createGaussianKernel(int kernelSize, double sigma) {
	// kernelSize should be odd number
	assert(kernelSize % 2 == 1);

	// Get padding size
	int pad = kernelSize / 2;

	// Get number of pixel in kernel
	int nPixel = kernelSize * kernelSize;

	// create kernel (double type)
	double * kernel = new double[nPixel];

	// Compute the values in kernel, following Gaussian distribution
	double a = 2.0 * sigma * sigma;
	for (int y = -pad; y <= pad; ++y) {
		for (int x = -pad; x <= pad; ++x) {
			double b = std::exp(-(double)(x * x + y * y) / a) / (a * PI);
			kernel[(y + pad) * kernelSize + (x + pad)] = b;
		}
	}

	// Normalize Gaussian kernel to integers
	double c = kernel[0];
	int * dstKernel = new int[nPixel];
	for (int i = 0; i < nPixel; ++i) {
		dstKernel[i] = static_cast<int>(std::round(dstKernel[i] / c));
	}

	std::free(kernel);
	kernel = nullptr;

	return dstKernel;
}

cv::Mat gaussianFilter(const cv::Mat& src, int kernelSize, int* kernel) {
	// kernelSize should be odd number
	assert(kernelSize % 2 == 1);
	// kernel should not be empty
	assert(kernel != nullptr);

	// Get padding size
	int pad = kernelSize / 2;

	// Get number of pixel in kernel
	int nPixel = kernelSize * kernelSize;

	// Add border to image
	int extendSize = 2 * pad;
	cv::Mat tmp = cv::Mat::zeros(cv::Size(src.cols + extendSize, src.rows + extendSize), src.type());
	cv::copyMakeBorder(src, tmp, pad, pad, pad, pad, cv::BORDER_REPLICATE);

	// Define result image
	cv::Mat dst = cv::Mat::zeros(src.size(), src.type());

	// Get sum of kernel's values
	double sum = 0.0;
	for (int i = 0; i < nPixel; ++i) {
		sum += kernel[i];
	}

	for (int y = pad; y < tmp.rows - pad; ++y) {
		for (int x = pad; x < tmp.cols - pad; ++x) {
			double p = 0.0;
			int k = 0;
			for (int j = -pad; j <= pad; ++j) {
				for (int i = -pad; i <= pad; ++i) {
					p += (double)(tmp.at<uchar>(y + j, x + i)) * (double)(kernel[k]);
					++k;
				}
			}
			// Compute value
			dst.at<uchar>(y - pad, x - pad) = cv::saturate_cast<uchar>(p / sum);
		}
	}
	return dst;
}
