#include "gaussian_filter.h"

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
	cv::Mat tmp = cv::Mat::zeros(cv::Size(src.cols + 2 * pad, src.rows + 2 * pad), src.type());
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
