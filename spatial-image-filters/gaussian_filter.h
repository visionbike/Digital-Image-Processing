#pragma once
#ifndef _GAUSSIAN_FILTER_H_
#define _GAUSSIAN_FILTER_H_

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

const double PI = 3.1416;

/* Gaussian filter */
// Apply convolutional operator
cv::Mat gaussianFilter(const cv::Mat& src, int kernelSize, int* kernel);
#endif // !_GAUSSIAN_FILTER_H_
