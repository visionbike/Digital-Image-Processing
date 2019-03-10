#pragma once
#pragma once
#ifndef _DCT_H_
#define _DCT_H_

#include <iostream>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/* Notice:
 * The matrix after DCT transform is in 64FC1 format.
 * Therefore, if you want to display DCT matrix, please convert it into 8UC1.
 */

 /* DCT algorithm on 1D vetor */
namespace DCT1D {
	double C(int i, int vec_size);
	std::vector<double> transform(const std::vector<double>& src, int vec_size = 8);
	std::vector<double> inverseTransform(const std::vector<double>& dct, int vec_size = 8);
}

/* DCT algorithm on 2D matrix */
namespace DCT2D {
	std::vector<std::vector<double>> createCosineMat(int block_size = 8);
	std::vector<std::vector<double>> createCoefficientMat(int block_size = 8);
	cv::Mat transform(const cv::Mat& src, int block_size = 8, int mode = 1);
	cv::Mat inverseTransform(const cv::Mat& dct, int block_size = 8, int mode = 1);
};

/* Fast DCT algorithm on 8-point vector, by Arai, Agui, Nakajima, 1988 */
namespace FDCT {
	static const double S[] = {
		0.353553390593273762200422,
		0.254897789552079584470970,
		0.270598050073098492199862,
		0.300672443467522640271861,
		0.353553390593273762200422,
		0.449988111568207852319255,
		0.653281482438188263928322,
		1.281457723870753089398043,
	};

	static const double A[] = {
		NAN,
		0.707106781186547524400844,
		0.541196100146196984399723,
		0.707106781186547524400844,
		1.306562964876376527856643,
		0.382683432365089771728460,
	};

	// Fast DCT type II, scaled. Algorithm by Arai, Agui, Nakajima, 1988.
	// Ref: https://web.stanford.edu/class/ee398a/handouts/lectures/07-TransformCoding.pdf#page=30
	std::vector<double> transform(const double src[8]);

	// Fast IDCT type III, scaled. Algorithm by Arai, Agui, Nakajima, 1988.
	// Ref: https://web.stanford.edu/class/ee398a/handouts/lectures/07-TransformCoding.pdf#page=30
	std::vector<double> inverseTransform(const double dct[8]);
}
#endif // !_DCT_H_

