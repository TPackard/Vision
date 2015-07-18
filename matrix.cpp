#include "matrix.h"

Matrix::Matrix(): Mat() {
	// Don't actually need to do anything
}

Matrix::Matrix(cv::Mat mat): Mat(mat) {
	// Don't actually need to do anything
}

Matrix Matrix::clone() {
	return Matrix(*this);
}

Matrix * Matrix::alphaBeta(double alpha, double beta) {
	if (isContinuous()) {
		int end = rows * cols * channels();
		uchar *data = ptr<uchar>(0);

		for (int i = 0; i < end; ++i) {
			*data = cv::saturate_cast<uchar>(alpha * (*data) + beta);
			data++;
		}
	} else {
		std::cerr << "Make alpha beta for non continous Mats!" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	return this;
}

Matrix * Matrix::blur(int strength) {
	cv::Mat temp = this->clone();
	cv::GaussianBlur(temp, *this, cv::Size(strength, strength), 0, 0);

	return this;
}

Matrix Matrix::mask(Contours contours) {
	Matrix masked = clone();

	if (contours.size() > 0) {
		// Create mask
		cv::Mat mask = cv::Mat::zeros(size(), CV_8UC1);

		// Draw contours onto the mask so they are not removed
		for (int i = 0; i < contours.size(); ++i) {
			cv::drawContours(mask, contours, i, cv::Scalar(255), CV_FILLED);
		}

		masked = cv::Mat(cv::Mat::zeros(size(), type()));
		copyTo(masked, mask);
	}

	return masked;
}

Matrix Matrix::crop(Contours contours) {
	Matrix cropped = cv::Mat(cv::Mat::zeros(1, 1, type()));

	if (contours.size() > 0) {
		// Determine ROI
		int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
		for (int i = 0; i < contours.size(); ++i) {
			cv::Rect rect = cv::boundingRect(cv::Mat(contours[i]));
			if (rect.x < minX) minX = rect.x;
			if (rect.y < minY) minY = rect.y;
			if (rect.x + rect.width > maxX) maxX = rect.x + rect.width;
			if (rect.y + rect.height > maxY) maxY = rect.y + rect.height;
		}

		cropped = (*this)(cv::Rect(minX, minY, maxX - minX, maxY - minY)).clone();
	}

	return cropped;
}

Contours Matrix::findContours(cv::Scalar min, cv::Scalar max) {
	// Convert to HSV color space
	cv::Mat hsvVersion = cv::Mat::zeros(size(), type());
	cv::cvtColor(*this, hsvVersion, CV_BGR2HSV);

	// Mask out non-yellow pixels
	cv::Mat masked = cv::Mat::zeros(hsvVersion.size(), hsvVersion.type());
	cv::inRange(hsvVersion, min, max, masked);

	// Remove small discrepancies
	int strength = 5;
	erode(masked, masked, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(strength, strength)));
	dilate(masked, masked, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(strength, strength)));
	dilate(masked, masked, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(strength, strength)));
	erode(masked, masked, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(strength, strength)));

	// Detect blobs
	Contours contours;
	cv::findContours(masked, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	return contours;
}