#include "dct.h"

double DCT1D::C(int i, int vec_size) {
	if (i == 0) return 1. / sqrt(vec_size);
	return sqrt(2. / vec_size);
}

std::vector<double> DCT1D::transform(const std::vector<double>& src, int vec_size) {
	std::vector<double> dct(vec_size, 0.);

	for (int i = 0; i < vec_size; ++i) {
		double tmp = 0.;
		for (int j = 0; j < vec_size; ++j) {
			tmp += src[j] * cos(((2 * j + 1) * i * CV_PI) / (2 * vec_size));
		}
		dct[i] = DCT1D::C(i, vec_size) * tmp;
	}
	return dct;
}
std::vector<double> DCT1D::inverseTransform(const std::vector<double>& dct, int vec_size) {
	std::vector<double> idct(vec_size, 0.);

	for (int i = 0; i < vec_size; ++i) {
		for (int j = 0; j < vec_size; ++j) {
			idct[i] += DCT1D::C(j, vec_size) * dct[j] * cos(((2 * i + 1) * j * CV_PI) / (2 * vec_size));
		}
	}
	return idct;
}

std::vector<std::vector<double>> DCT2D::createCosineMat(int block_size) {
	std::vector<std::vector<double>> cosineMat(block_size, std::vector<double>(block_size, 1.));

	for (int i = 0; i < block_size; ++i) {
		for (int j = 0; j < block_size; ++j) {
			cosineMat[i][j] = std::cos(((2. * i + 1.) * j * CV_PI) / (2. * block_size));
		}
	}

	// test
	/*for (int i = 0; i < block_size; ++i) {
		for (int j = 0; j < block_size; ++j) {
			std::cout << cosineMat[i][j] << " ";
		}
		std::cout << std::endl;
	}*/

	return cosineMat;
}

std::vector<std::vector<double>> DCT2D::createCoefficientMat(int block_size) {
	std::vector<std::vector<double>> coefficientMat(block_size, std::vector<double>(block_size, 1.));

	coefficientMat[0][0] = 0.5;
	for (int i = 1; i < block_size; ++i) {
		coefficientMat[0][i] = coefficientMat[i][0] = 1. / sqrt(2.);
	}

	// test
	/*for (int i = 0; i < block_size; ++i) {
		for (int j = 0; j < block_size; ++j) {
			std::cout << coefficientMat[i][j] << " ";
		}
		std::cout << std::endl;
	}*/

	return coefficientMat;
}

cv::Mat DCT2D::transform(const cv::Mat& src, int block_size, int mode) {
	// output
	cv::Mat dct = cv::Mat::zeros(src.size(), CV_64FC1);

	if (mode == 0) { // 2D DCT using Fast DCT
		// 1D DCT by rows
		for (int y = 0; y < src.rows; ++y) {
			const uchar* p = src.ptr<uchar>(y);
			for (int x = 0; x < src.cols; x += block_size) {
				std::vector<double> vec(p + x, p + x + block_size);
				std::vector<double> dctVec = FDCT::transform(vec.data());
				for (int i = 0; i < block_size; ++i) {
					dct.at<double>(y, x + i) = dctVec[i];
				}
			}
		}
		// 1D DCT by columns
		for (int x = 0; x < dct.cols; ++x) {
			cv::Mat column = dct.col(x);
			cv::transpose(column, column);
			const double* p = column.ptr<double>(0);
			for (int y = 0; y < dct.rows; y += block_size) {
				std::vector<double> vec(p + y, p + y + block_size);
				std::vector<double> dctVec = FDCT::transform(vec.data());
				for (int i = 0; i < block_size; ++i) {
					dct.at<double>(y + i, x) = dctVec[i];
				}
			}
		}
	}
	else if (mode == 1) { // 2D DCT using 1D DCT
		// 1D DCT by rows
		for (int y = 0; y < src.rows; ++y) {
			const uchar* p = src.ptr<uchar>(y);
			for (int x = 0; x < src.cols; x += block_size) {
				std::vector<double> vec(p + x, p + x + block_size);
				std::vector<double> dctVec = DCT1D::transform(vec, block_size);
				for (int i = 0; i < block_size; ++i) {
					dct.at<double>(y, x + i) = dctVec[i];
				}
			}
		}
		// 1D DCT by columns
		for (int x = 0; x < dct.cols; ++x) {
			cv::Mat column = dct.col(x);
			cv::transpose(column, column);
			const double* p = column.ptr<double>(0);
			for (int y = 0; y < dct.rows; y += block_size) {
				std::vector<double> vec(p + y, p + y + block_size);
				std::vector<double> dctVec = DCT1D::transform(vec, block_size);
				for (int i = 0; i < block_size; ++i) {
					dct.at<double>(y + i, x) = dctVec[i];
				}
			}
		}
	}
	else if (mode == 2) { // 2D DCT
		// create cosine matrix
		std::vector<std::vector<double>> cosine = DCT2D::createCosineMat(block_size);

		// creat coefficient matrix
		std::vector<std::vector<double>> coeff = DCT2D::createCoefficientMat(block_size);

		double determinator = 2. / block_size;
		for (int y = 0; y < src.rows; y += block_size) {
			for (int x = 0; x < src.cols; x += block_size) {
				for (int i = 0; i < block_size; ++i) {
					for (int j = 0; j < block_size; ++j) {
						double tmp = 0.;
						for (int t = 0; t < block_size; ++t) {
							for (int k = 0; k < block_size; ++k) {
								tmp += (double)(src.at<uchar>(y + t, x + k)) * cosine[t][i] * cosine[k][j];
							}
						}
						dct.at<double>(y + i, x + j) = determinator * tmp * coeff[i][j];
					}
				}
			}
		}
	}
	return dct;
}

cv::Mat DCT2D::inverseTransform(const cv::Mat& dct, int block_size, int mode) {
	// output
	cv::Mat idct = cv::Mat::zeros(dct.size(), CV_64FC1);

	if (mode == 0) { // Fast IDCT
		// 1D IDCT by columns
		for (int x = 0; x < dct.cols; ++x) {
			cv::Mat column = dct.col(x);
			cv::transpose(column, column);
			const double* p = column.ptr<double>(0);
			for (int y = 0; y < dct.rows; y += block_size) {
				std::vector<double> vec(p + y, p + y + block_size);
				std::vector<double> idctVec = FDCT::inverseTransform(vec.data());
				for (int i = 0; i < block_size; ++i) {
					idct.at<double>(y + i, x) = idctVec[i];
				}
			}
		}
		// 1D IDCT by rows
		for (int y = 0; y < idct.rows; ++y) {
			const double* p = idct.ptr<double>(y);
			for (int x = 0; x < idct.cols; x += block_size) {
				std::vector<double> vec(p + x, p + x + block_size);
				std::vector<double> idctVec = FDCT::inverseTransform(vec.data());
				for (int i = 0; i < block_size; ++i) {
					idct.at<double>(y, x + i) = idctVec[i];
				}
			}
		}
	}
	if (mode == 1) { // 1D IDCT
		// 1D IDCT by columns
		for (int x = 0; x < dct.cols; ++x) {
			cv::Mat column = dct.col(x);
			cv::transpose(column, column);
			const double* p = column.ptr<double>(0);
			for (int y = 0; y < dct.rows; y += block_size) {
				std::vector<double> vec(p + y, p + y + block_size);
				std::vector<double> idctVec = DCT1D::inverseTransform(vec, block_size);
				for (int i = 0; i < block_size; ++i) {
					idct.at<double>(y + i, x) = idctVec[i];
				}
			}
		}
		// 1D IDCT by rows
		for (int y = 0; y < idct.rows; ++y) {
			const double* p = idct.ptr<double>(y);
			for (int x = 0; x < idct.cols; x += block_size) {
				std::vector<double> vec(p + x, p + x + block_size);
				std::vector<double> idctVec = DCT1D::inverseTransform(vec, block_size);
				for (int i = 0; i < block_size; ++i) {
					idct.at<double>(y, x + i) = idctVec[i];
				}
			}
		}
	}
	else if (mode == 2) { // 2D IDCT
		// create cosine matrix
		std::vector<std::vector<double>> cosine = DCT2D::createCosineMat(block_size);

		// creat coefficient matrix
		std::vector<std::vector<double>> coeff = DCT2D::createCoefficientMat(block_size);

		double determinator = 2. / block_size;

		for (int y = 0; y < dct.rows; y += block_size) {
			for (int x = 0; x < dct.cols; x += block_size) {
				for (int i = 0; i < block_size; ++i) {
					for (int j = 0; j < block_size; ++j) {
						double tmp = 0.;
						for (int t = 0; t < block_size; ++t) {
							for (int k = 0; k < block_size; ++k) {
								tmp += coeff[t][k] * dct.at<double>(y + t, x + k) * cosine[i][t] * cosine[j][k];
							}
						}
						idct.at<double>(y + i, x + j) = determinator * tmp;
					}
				}
			}
		}
	}
	idct.convertTo(idct, CV_8UC1);
	return idct;
}

std::vector<double> FDCT::transform(const double src[8]) {
	std::vector<double> dct(8, 0.);

	const double v0 = src[0] + src[7];
	const double v1 = src[1] + src[6];
	const double v2 = src[2] + src[5];
	const double v3 = src[3] + src[4];
	const double v4 = src[3] - src[4];
	const double v5 = src[2] - src[5];
	const double v6 = src[1] - src[6];
	const double v7 = src[0] - src[7];

	const double v8 = v0 + v3;
	const double v9 = v1 + v2;
	const double v10 = v1 - v2;
	const double v11 = v0 - v3;
	const double v12 = -v4 - v5;
	const double v13 = (v5 + v6) * A[3];
	const double v14 = v6 + v7;

	const double v15 = v8 + v9;
	const double v16 = v8 - v9;
	const double v17 = (v10 + v11) * A[1];
	const double v18 = (v12 + v14) * A[5];

	const double v19 = -v12 * A[2] - v18;
	const double v20 = v14 * A[4] - v18;

	const double v21 = v17 + v11;
	const double v22 = v11 - v17;
	const double v23 = v13 + v7;
	const double v24 = v7 - v13;

	const double v25 = v19 + v24;
	const double v26 = v23 + v20;
	const double v27 = v23 - v20;
	const double v28 = v24 - v19;

	dct[0] = S[0] * v15;
	dct[1] = S[1] * v26;
	dct[2] = S[2] * v21;
	dct[3] = S[3] * v28;
	dct[4] = S[4] * v16;
	dct[5] = S[5] * v25;
	dct[6] = S[6] * v22;
	dct[7] = S[7] * v27;

	return dct;
}

std::vector<double> FDCT::inverseTransform(const double dct[8]) {
	std::vector<double> idct(8, 0.);

	const double v15 = dct[0] / S[0];
	const double v26 = dct[1] / S[1];
	const double v21 = dct[2] / S[2];
	const double v28 = dct[3] / S[3];
	const double v16 = dct[4] / S[4];
	const double v25 = dct[5] / S[5];
	const double v22 = dct[6] / S[6];
	const double v27 = dct[7] / S[7];

	const double v19 = (v25 - v28) / 2;
	const double v20 = (v26 - v27) / 2;
	const double v23 = (v26 + v27) / 2;
	const double v24 = (v25 + v28) / 2;

	const double v7 = (v23 + v24) / 2;
	const double v11 = (v21 + v22) / 2;
	const double v13 = (v23 - v24) / 2;
	const double v17 = (v21 - v22) / 2;

	const double v8 = (v15 + v16) / 2;
	const double v9 = (v15 - v16) / 2;

	const double v18 = (v19 - v20) * A[5];  // Different from original
	const double v12 = (v19 * A[4] - v18) / (A[2] * A[5] - A[2] * A[4] - A[4] * A[5]);
	const double v14 = (v18 - v20 * A[2]) / (A[2] * A[5] - A[2] * A[4] - A[4] * A[5]);

	const double v6 = v14 - v7;
	const double v5 = v13 / A[3] - v6;
	const double v4 = -v5 - v12;
	const double v10 = v17 / A[1] - v11;

	const double v0 = (v8 + v11) / 2;
	const double v1 = (v9 + v10) / 2;
	const double v2 = (v9 - v10) / 2;
	const double v3 = (v8 - v11) / 2;

	idct[0] = (v0 + v7) / 2;
	idct[1] = (v1 + v6) / 2;
	idct[2] = (v2 + v5) / 2;
	idct[3] = (v3 + v4) / 2;
	idct[4] = (v3 - v4) / 2;
	idct[5] = (v2 - v5) / 2;
	idct[6] = (v1 - v6) / 2;
	idct[7] = (v0 - v7) / 2;

	return idct;
}
