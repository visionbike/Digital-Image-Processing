#pragma once
#ifndef _AVERAGE_FILTER_H_
#define _AVERAGE_FILTER_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

/* Average filter */
cv::Mat averageFilter(const cv::Mat& src, int kernelSize);
#endif // !_AVERAGE_FILTER_H_
