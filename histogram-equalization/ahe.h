#pragma once
#ifndef _LHE_H_
#define _LHE_H_

#include <iostream>
#include <opencv2/core/core.hpp>
#include "util.h"

// Local histogram equalization or adaptive histogram equalization function
// kernel: the size of sub-image to equalize histogram
// This function applies to overlaping and non-overlapping cases
cv::Mat localEqualizeHist(const cv::Mat& src, int kernel = 15, bool overlap = true);

#endif // !_LHE_H_

