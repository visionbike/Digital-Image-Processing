#include "average_filter.h"

cv::Mat averageFilter(const cv::Mat& src, int kernelSize) {
	// kernelSize should be odd number
	assert(kernelSize % 2 == 1);

	// Get padding size
	int pad = kernelSize / 2;

	// Define result image
	cv::Mat dst = cv::Mat::zeros(src.size(), src.type());
	src.copyTo(dst);

	// Add border to image
	int extendSize = 2 * pad;
	cv::Mat tmp = cv::Mat::zeros(cv::Size(src.cols + extendSize, src.rows + extendSize), src.type());
	cv::copyMakeBorder(src, tmp, pad, pad, pad, pad, cv::BORDER_REPLICATE);
	cv::imshow("tmp", tmp);

	// Get number of pixels in kernel
	int nPixel = kernelSize * kernelSize;

	for (int y = pad; y < tmp.rows - pad; ++y) {
		for (int x = pad; x < tmp.cols - pad; ++x) {
			double sum = 0.0;
			for (int j = -pad; j <= pad; ++j) {
				for (int i = -pad; i <= pad; ++i) {
					sum += (double)(tmp.at<uchar>(y + j, x + i));
				}
			}
			// Compute average value of kernel
			dst.at<uchar>(y - pad, x - pad) = cv::saturate_cast<uchar>(std::round(sum / (double)(nPixel)));
		}
	}

	return dst;
}
