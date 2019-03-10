#include <iostream>>

#include "ghe.h"
#include "ahe.h"
#include "util.h"

int main(int argc, char** argv) {
	// Get parameter
	char* imageName;
	int kernel = 15;
	if (argc == 2) {
		imageName = argv[1];
	}
	else if (argc == 3) {
		imageName = argv[1];
		kernel = atoi(argv[2]);
	}
	else {
		std::cout << "Wrong syntax!\n";
		std::cout << "Syntax:\thistogram-equalisation.exe <image name>\n";
		std::cout << "       \thistogram-equalisation.exe <image name> <kernel size>\n";
		return -1;
	}

	// Load image
	cv::Mat img = cv::imread(imageName, cv::IMREAD_GRAYSCALE);

	if (!img.data) {
		std::cout << "Invalid image name!\n";
		return -1;
	}

	// Calculate input image histogram
	int* hist = calcHist(img);

	// Global histogram equalization
	cv::Mat ghe = equalizeHist(img);
	int* hist2 = calcHist(ghe);

	// Local histogram equalization with overlapping
	cv::Mat ahe_o = localEqualizeHist(img, kernel, true);
	int* hist3 = calcHist(ahe_o);

	// Local histogram equalization without overlapping
	cv::Mat ahe = localEqualizeHist(img, kernel, false);
	int* hist4 = calcHist(ahe);

	//Save results
	cv::imwrite("HE.png", ghe);
	cv::imwrite("AHE with overlapping.png", ahe_o);
	cv::imwrite("AHE without overlapping.png", ahe);

	// Display results
	cv::imshow("Image", img);
	displayHist(hist, "Image Histogram");
	cv::imshow("Global Histogram Equalization (HE)", ghe);
	displayHist(hist2, "HE Histogram");
	cv::imshow("Local Histogram Equalization (AHE) with overlapping", ahe_o);
	displayHist(hist3, "AHE Histogram with overlapping");
	cv::imshow("Local Histogram Equalization (AHE) without overlapping", ahe);
	displayHist(hist4, "AHE Histogram without overlapping");

	cv::waitKey(0);

	return 0;
}
