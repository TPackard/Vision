#include <opencv2/opencv.hpp>
#include "error.h"

using Contour = std::vector<cv::Point>;

int main() {
	cv::namedWindow("Camera Feed");

	cv::VideoCapture vCap;
	vCap.open(0);

	if (!vCap.isOpened()) {
		ERROR("No camera");
		return 1;
	}

	while(vCap.isOpened()) {
		// Load image
		cv::Mat source;
		vCap.read(source);

		// Apply Gaussian blur so that only notable features are detected
		cv::Mat denoised = cv::Mat::zeros(source.size(), source.type());
		cv::GaussianBlur(source, denoised, cv::Size(9, 9), 0, 0);

		// Double contrast and decrease brightness by 100
		cv::Mat contrasted = cv::Mat::zeros(source.size(), source.type());
		denoised.convertTo(contrasted, -1, 3.0, -100.0);

		// Convert to HSV colorspace
		cv::Mat hsvSource = cv::Mat::zeros(source.size(), source.type());
		cv::cvtColor(denoised, hsvSource, CV_BGR2HSV);

		// Mask out non-yellow pixels
		cv::Mat masked = cv::Mat::zeros(hsvSource.size(), hsvSource.type());
		cv::inRange(hsvSource, cv::Scalar(20, 125, 50), cv::Scalar(41, 243, 255), masked);

		// Remove small discrepancies
		int strength = 5;
		erode(masked, masked, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(strength, strength)));
		dilate(masked, masked, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(strength, strength)));
		dilate(masked, masked, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(strength, strength)));
		erode(masked, masked, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(strength, strength)));

		// Detect blobs
		std::vector<Contour> contours;
		cv::findContours(masked, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

		// Remove small blobs
		contours.erase(std::remove_if(contours.begin(), contours.end(), [](Contour c) { return cv::contourArea(c) < 2000; }), contours.end());

		// Draw blobs onto the source image (for testing)
		for (int i = 0; i < contours.size(); ++i) {
			cv::drawContours(source, contours, i, cv::Scalar(100, 255, 0), -1, CV_AA);
		}

		// Show the detected features
		cv::imshow("Camera Feed", source);

		if (cv::waitKey(1) == 27) { // Wait for escape key press
			break;
		}
	}

	return 0;
}