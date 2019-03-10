#include "ahe.h"

cv::Mat localEqualizeHist(const cv::Mat& src, int kernel, bool overlap) {
	// Input image should be gray-scale image
	assert(src.type() == CV_8UC1);

	// kernel value shoulf be odd number
	assert(kernel % 2 == 1);

	// Define padding size of image
	int pad = kernel / 2;

	// Number of sub-image (src is gray-scale image)
	int nSubPixel = kernel * kernel;

	// Define equalized image
	cv::Mat dst = cv::Mat::zeros(src.size(), src.type());
	src.copyTo(dst);

	// Make padding for src image using prior compensation
	cv::Mat tmp = cv::Mat::zeros(cv::Size(src.cols + pad, src.rows + pad), src.type());
	cv::copyMakeBorder(src, tmp, pad, pad, pad, pad, cv::BORDER_REPLICATE);

	if (overlap) {
		// Apply overlapping sub-image
		// Equalize centers of sub-image, which are pixels in src image
		for (int y = 0; y < src.rows; ++y) {
			for (int x = 0; x < src.cols; ++x) {
				// Get sub-image
				cv::Mat roi = tmp(cv::Range(y, y + kernel), cv::Range(x, x + kernel));

				// Calculate src image histgoram
				int* hist = calcHist(roi);

				// Calculate cumulative histogram
				double* cumHist = calcCumulativeHist(hist);

				//// Find non-zero min of cumulative histogram
				//double minVal = 0;
				//for (int i = 0; i < BIN; ++i) {
				//	if (cumHist[i] == 0) continue;
				//	minVal = cumHist[i];
				//	break;
				//}

				// Map new value
				double newVal = (cumHist[(int)src.at<uchar>(y, x)] / nSubPixel * MAX_INTENSITY + 0.5);
				dst.at<uchar>(y, x) = cv::saturate_cast<uchar>(newVal);

				// Release memory
				free(hist);
				free(cumHist);
				hist = nullptr;
				cumHist = nullptr;
			}
		}
	}
	else {
		// apply non-overlapping regions
		// equalize centers of ROIs, which are pixels in src image.
		for (int y = 0; y < src.rows; y += kernel) {
			for (int x = 0; x < src.cols; x += kernel) {
				cv::Mat roi = tmp(cv::Range(y, y + kernel), cv::Range(x, x + kernel));

				// Calculate src image histgoram
				int* hist = calcHist(roi);

				// Calculate cumulative histogram
				double* cumHist = calcCumulativeHist(hist);

				for (int i = -pad; i <= pad; ++i) {
					for (int j = -pad; j <= pad; ++j) {
						if ((y + i < 0) || (y + i >= src.rows) || (x + j < 0) || (x + j >= src.cols)) continue;
						// Map new value
						double newVal = (cumHist[(int)src.at<uchar>(y + i, x + j)] / nSubPixel * MAX_INTENSITY + 0.5);
						dst.at<uchar>(y + i, x + j) = cv::saturate_cast<uchar>(newVal);
					}
				}

				free(hist);
				free(cumHist);
				hist = nullptr;
				cumHist = nullptr;
				roi.release();
			}
		}
	}
	return dst;
}
