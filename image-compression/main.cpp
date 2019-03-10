#include <iostream>
#include<ctime>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "dct.h"

int main() {
	std::cout << "\t*** Discrete Cosine Transform ***\n";
	std::cout << "***NOTICE***: Fast DCT is only for 8-point size\n\n";
	std::cout << "***OPTION***:\n";
	std::cout << "\t0: 2D DCT using Fast DCT\n";
	std::cout << "\t1: 2D DCT using DCT 1D\n";
	std::cout << "\t2: Basic 2D DCT\n";

	// get image path
	std::string filename = "";
	std::cout << "\nInput image: ";
	std::getline(std::cin, filename);

	cv::Mat img = cv::imread(filename, cv::IMREAD_GRAYSCALE);
	if (img.empty()) {
		std::cout << "Invalid filename!\n";
		return -1;
	}
	if (img.rows != img.cols) {
		std::cout << "Width and Height shoulde be equal!\n";
		return -1;
	}

	// choose mode
	int mode = 0;
	std::cout << "\nChoose option: ";
	std::cin >> mode;
	if (mode != 0 && mode != 1 && mode != 2) {
		std::cout << "Invalid option!\n";
		return -1;
	}

	// choose block size
	int block_size = 8;
	std::cout << "\nChoose block size: ";
	std::cin >> block_size;
	if (mode == 0 && block_size != 8) {
		std::cout << "Fast DCT only support 8-point block!\n";
		return -1;
	}

	// DCT
	std::cout << "Excecuting DCT...\n";
	std::clock_t tStart = std::clock();
	cv::Mat dct = DCT2D::transform(img, block_size, mode);
	printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	// IDCT
	std::cout << "Excecuting DCT...\n";
	tStart = std::clock();
	cv::Mat idct = DCT2D::inverseTransform(dct, block_size, mode);
	printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	dct.convertTo(dct, CV_8UC1);

	// save results
	std::string smode = "FDCT";
	if (mode == 1) smode = "1DCT";
	if (mode == 2) smode = "2DCT";
	std::string dctFile = "result_dct_" + smode + "_" + std::to_string(block_size) + ".png";
	std::string idctFile = "result_idct_" + smode + "_" + std::to_string(block_size) + ".png";
	cv::imwrite(dctFile, dct);
	cv::imwrite(idctFile, idct);

	// display result
	cv::imshow("Image", img);
	cv::imshow("DCT", dct);
	cv::imshow("IDCT", idct);
	cv::waitKey(0);

	return 0;
}
