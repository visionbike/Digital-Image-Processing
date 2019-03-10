#pragma once
#ifndef _MEDIAN_FILTER_H_
#define _MEDIAN_FILTER_H_

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

/* Median filter */
cv::Mat medianFilter(const cv::Mat& src, int kernelSize);
#endif // !MEDIAN_FILTER
