#include "util.h"

int* calcHist(const cv::Mat& src) {
	// Input image should be gray-scale image
	assert(src.type() == CV_8UC1);

	// Initialize histgram as zero array
	int* hist = new int[BIN];
	for (int i = 0; i < BIN; i++)
		hist[i] = 0;

	// Compute no of pixels for each intensity values
	// Slidding data pointer in case gray-scale image
	// because gray-scale image is in continuous memory zone
	uchar* pData = src.data;
	for (int i = 0; i < src.total(); ++i, ++pData) {
		++hist[int(*(pData))];
	}
	return hist;
}

double* calcCumulativeHist(int* hist) {
	double* cumHist = new double[BIN];

	// Compute cummulative distribution
	// cumHist[i] = hist[0] + ... + hist[i - 1] + hist[i]
	// cumHist[i] = cumHist[i - 1] + hist[i]
	cumHist[0] = hist[0];
	for (int i = 1; i < BIN; ++i) {
		cumHist[i] = cumHist[i - 1] + hist[i];
	}
	return cumHist;
}

void displayHist(int hist[256], const char* name) {
	// Define size of histogram image
	int histWidth = 512;
	int histHeight = 400;
	int binWidth = cvRound(histWidth / BIN);

	// Define histogram image
	cv::Mat histImage(histHeight, histWidth, CV_8UC3, cv::Scalar(255, 255, 255));

	// Find maximum element of histogram
	int max = hist[0];
	for (int i = 1; i < BIN; ++i) {
		if (max < hist[i]) max = hist[i];
	}

	// Normalize the histogram between 0 and 90% histImage.rows
	for (int i = 0; i < BIN; ++i) {
		hist[i] = ((double)hist[i] / max)*(0.9*histHeight);
	}

	// Draw the intensity lin for histogram
	for (int i = 0; i < 256; ++i) {
		cv::line(histImage, cv::Point(binWidth*(i), histHeight), cv::Point(binWidth*(i), histHeight - hist[i]), cv::Scalar(0, 0, 255), 1, 8, 0);
	}
	// Save histogram image
	std::string saveFileName(name);
	saveFileName += ".png";
	cv::imwrite(saveFileName, histImage);

	// Display histogram image
	cv::imshow(name, histImage);
}