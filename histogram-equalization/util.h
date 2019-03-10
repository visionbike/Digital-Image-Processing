#pragma once
#ifndef _UTIL_H_
#define _UTIL_H_

#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

/* Define constants */
const double BIN = 256.0;
const double MIN_INTENSITY = 0.0;
const double MAX_INTENSITY = 255.0;

// Histogram calculation function
int* calcHist(const cv::Mat& src);

// Cummulatve histogram calculation
double* calcCumulativeHist(int* hist);

// Histogram displaying function
void displayHist(int hist[256], const char* name);
#endif // !_UTIL_H_
