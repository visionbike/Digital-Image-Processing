#pragma once
#ifndef _GHE_H_
#define _GHE_H_

#include <iostream>
#include <opencv2/core/core.hpp>
#include "util.h"

// Global histgram equalization function
cv::Mat equalizeHist(const cv::Mat& src);
#endif // !_GHE_H_

