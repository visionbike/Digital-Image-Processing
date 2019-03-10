#include "median_filter.h"

cv::Mat medianFilter(const cv::Mat& src, int kernelSize) {
	// kernelSize should be odd number
	assert(kernelSize % 2 == 1);

	// Get padding size
	int pad = kernelSize / 2;

	// Define result image
	cv::Mat dst = cv::Mat::zeros(src.size(), src.type());
	src.copyTo(dst);

	// Get number of pixel in kernel
	int nPixel = kernelSize * kernelSize;

	// Define kernel window
	std::vector<uchar> kernel(kernelSize, 0);

	// Add border to image
	int extendSize = 2 * pad;
	cv::Mat tmp = cv::Mat::zeros(cv::Size(src.cols + extendSize, src.rows + extendSize), src.type());
	cv::copyMakeBorder(src, tmp, pad, pad, pad, pad, cv::BORDER_REPLICATE);

	for (int y = pad; y < tmp.rows - pad; ++y) {
		for (int x = pad; x < tmp.cols - pad; ++x) {
			int k = 0;
			for (int j = -pad; j <= pad; ++j) {
				for (int i = -pad; i <= pad; ++i) {
					// Get value in kernel
					kernel[k] = tmp.at<uchar>(y + j, x + i);
					++k;
				}
			}
			// Sort kernel's values
			std::sort(kernel.begin(), kernel.end());

			// Get median value
			dst.at<uchar>(y - pad, x - pad) = kernel[nPixel / 2];
		}
	}

	return dst;
}
