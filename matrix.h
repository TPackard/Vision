#ifndef VISION_MATRIX_H
#define VISION_MATRIX_H

#include "vision.h"

class Contours; // Forward declaration

/// A wrapper class for OpenCV Mats that provides extra functionality
class Matrix: public cv::Mat {
public:
	/// Default constructor
	Matrix();
	/// Clone constructor
	Matrix(cv::Mat mat);

	// Clones the matrix
	Matrix clone();

	/// Changes the brightness and contrast
	Matrix * alphaBeta(double alpha, double beta);

	/// Blur the matrix using a Gaussian filter
	Matrix * blur(int strength);

	// Masks the areas that are not the contours with black pixels
	Matrix mask(Contours contours);

	// Crops the matrix to only contain the contours
	Matrix crop(Contours contours);

	// Finds contours based off of an HSV range
	Contours findContours(cv::Scalar min, cv::Scalar max);
};

#endif //VISION_MATRIX_H
