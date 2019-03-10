#include "ghe.h"

cv::Mat equalizeHist(const cv::Mat& src) {
	// Input image should be gray-scale image
	assert(src.type() == CV_8UC1);

	// Define equalized image
	cv::Mat dst = cv::Mat::zeros(src.size(), src.type());

	// Calculate src image histgoram
	int* hist = calcHist(src);

	// Calculate cumulative histogram
	double* cumHist = calcCumulativeHist(hist);

	// Number of src image
	int nPixel = src.total();

	// Compute equalized image
	// Slidding data pointer in case gray-scale image
	// because gray-scale image is in continuous memory zone
	uchar* pSrcData = src.data;
	uchar* pDstData = dst.data;
	for (int i = 0; i < nPixel; ++i, pSrcData++, pDstData++) {
		double newVal = (cumHist[(int)*(pSrcData)] / nPixel * MAX_INTENSITY + 0.5);
		*pDstData = cv::saturate_cast<uchar>(newVal);
	}

	// release memory
	free(hist);
	free(cumHist);
	hist = nullptr;
	cumHist = nullptr;

	return dst;
}
