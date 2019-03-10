#pragma once
#ifndef _GAUSSIAN_FILTER_H_
#define _GAUSSIAN_FILTER_H_

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

const double PI = 3.1416;

/* Create Gaussian kernel */
// Each value in kernel follow Gaussian distribution: 
// kernel(x, y) = (1.0 / (2 * PI * sigma^2)) * (exp(-(x^2 + y^2) / (2 * sigma^2)))
// With index format of kernel as:
// [(-1,-1) (0,-1) (1,-1)
//	 (-1,0)  (0,0)	(1,0)
//	 (-1,1)  (0,1)	(1,1)]
int* createGaussianKernel(int kernelSize, double sigma = 1.0);

/* Gaussian ilter */
// Apply convolutional operator
cv::Mat gaussianFilter(const cv::Mat& src, int kernelSize, int* kernel);
#endif // !_GAUSSIAN_FILTER_H_
