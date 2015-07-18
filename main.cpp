#include <opencv2/opencv.hpp>
#include "error.h"

/* ___ ___  ___  ______  ___  ______  __  ___      ______  ______  ___  ______  ___
 * |  /  / /  / /  ___/ /  / /     / /  |/  /     /___  / /     / /  / /  ___/ /  /
 * |    / /  / /___  / /  / /  /  / /      /     /  ___/ /  /  / /  / /  /  / /__/
 * |___/ /__/ /_____/ /__/ /_____/ /__/|__/     /_____/ /_____/ /__/ /_____/ /__/
 * NEW AND IMPROVED!
 *
 * NOW INCLUDES:
 *  • OBJECT FILTERING BY COLOR!
 *  • OBJECT FILTERING BY SIZE!
 *  • CROPPING & MASKING ROI!
 *  • AND MORE!
 *
 * WRITTEN BY TYLER PACKARD FOR TEAM 8 (PALY ROBOTICS)
 */

// Define what a Contour is
using Contour = std::vector<cv::Point>;

int main() {
	// Set up video camera
	cv::namedWindow("Camera Feed");

	cv::VideoCapture vCap;
	vCap.open(0);

	if (!vCap.isOpened()) {
		ERROR("No camera");
		return 1;
	}

	// Perform vision processes
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

		// Crop to ROI
		cv::Mat roi = cv::Mat::zeros(10, 10, source.type());

		if (contours.size() > 0) {
			// Create mask
			cv::Mat mask = cv::Mat::zeros(source.size(), CV_8UC1);

			// Draw contours onto the mask so they are not removed
			for (int i = 0; i < contours.size(); ++i) {
				cv::drawContours(mask, contours, i, cv::Scalar(255), CV_FILLED);
			}

			cv::Mat masked;
			source.copyTo(masked, mask);

			// Determine ROI
			int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
			for (int i = 0; i < contours.size(); ++i) {
				cv::Rect rect = cv::boundingRect(cv::Mat(contours[i]));
				if (rect.x < minX) minX = rect.x;
				if (rect.y < minY) minY = rect.y;
				if (rect.x + rect.width > maxX) maxX = rect.x + rect.width;
				if (rect.y + rect.height > maxY) maxY = rect.y + rect.height;
			}

			roi = masked(cv::Rect(minX, minY, maxX - minX, maxY - minY)).clone();
		}

		// Show the detected features
		cv::imshow("Camera Feed", roi);


		if (cv::waitKey(1) == 27) { // Wait for escape key press
			break;
		}
	}

	return 0;
}