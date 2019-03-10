#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "median_filter.h"
#include "average_filter.h"
#include "gaussian_filter.h"

int main(int argc, char** argv) {
	// Get parameters
	char* imageName = nullptr;
	int mode = 1;	// 1: average filter, 2: gaussian filter, 3: median filter
	int kernelSize = 3;

	std::string instruction = "spatial-image-filters.exe <imageName> <mode> <kernelSize> <sigma - optional>\n\n";
	instruction += "<mode>: 1 for average filter; 2 for gaussian filter; 3 for median filter\n";
	instruction += "<kernelSize>: default: 3\n";
	instruction += "sigma: default: 1.0 - option for mode 2 - gaussian filter\n";

	if (argc == 3) {
		imageName = argv[1];
		mode = atoi(argv[2]);
	}
	else if (argc == 4) {
		imageName = argv[1];
		mode = atoi(argv[2]);
		kernelSize = atoi(argv[3]);
	}
	else if (argc == 5) {
		imageName = argv[1];
		mode = atoi(argv[2]);
		kernelSize = atoi(argv[3]);
		sigma = atof(argv[4]);
	}
	else {
		std::cout << "Wrong syntax!\n\n";
		std::cout << instruction;
		return -1;
	}

	// Load image
	cv::Mat img = cv::imread(imageName, cv::IMREAD_GRAYSCALE);
	cv::Mat dst;

	std::string windowName = "";

	if (!img.data) {
		std::cout << "Invalid image name!\n";
		return -1;
	}

	switch (mode)
	{
	case 1: {
		// Average Filter
		windowName = "Average Filter - k = " + std::to_string(kernelSize);
		dst = averageFilter(img, kernelSize);
		break;
	}
	case 2: {
		// Gaussian Filter
		windowName = "Gaussian Filter - k = " + std::to_string(kernelSize) + ", sigma = " + std::to_string(sigma);
		int* kernel = int[9];
		kernel[0] = kernel[2] = kernel[6] = kernel[8] = 0;
		kernel[1] = kernel[3] = kernel[5] = kernel[7] = 2;
		kernel[4] = 4;
		kernelSize = 3
			dst = gaussianFilter(img, kernelSize, kernel);
		break;
	}
	case 3: {
		// Median Filter
		windowName = "Median Filter - k = " + std::to_string(kernelSize);
		dst = medianFilter(img, kernelSize);
		break;
	}
	default:
		break;
	}

	// Save result
	cv::imwrite("result.png", dst);

	cv::imshow("Original Image", img);
	cv::imshow(windowName, dst);
	cv::waitKey(0);
	return 0;
}
