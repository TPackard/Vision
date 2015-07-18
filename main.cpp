// ___ ___  ___  ______  ___  ______  __  ___      ______  ______  ___  ______  ___
// |  /  / /  / /  ___/ /  / /     / /  |/  /     /___  / /     / /  / /  ___/ /  /
// |    / /  / /___  / /  / /  /  / /      /     /  ___/ /  /  / /  / /  /  / /__/
// |___/ /__/ /_____/ /__/ /_____/ /__/|__/     /_____/ /_____/ /__/ /_____/ /__/
// NEW AND IMPROVED!
//
// NOW INCLUDES:
//  • OBJECT FILTERING BY COLOR!
//  • OBJECT FILTERING BY SIZE!
//  • CROPPING & MASKING ROI!
//  • AND MORE!
//
// WRITTEN BY TYLER PACKARD FOR TEAM 8 (PALY ROBOTICS)
// SUMMER 2015
//

#include "vision.h"

int main() {
	// Set up video camera
	cv::namedWindow("Camera Feed");

	Camera camera;

	// Perform vision processes
	while(camera.isOpened()) {
		// Load image
		Matrix source = camera.read();

		// Copy source into a matrix to transform
		Matrix transformed(source);

		// Apply the following transformations:
		//   • Gaussian blur so that only notable features are detected
		transformed.blur(9);

		// Finds and stores large yellow areas
		Contours contours = *transformed.findContours(cv::Scalar(20, 125, 50), cv::Scalar(41, 243, 255)).weedBySize(2000, INT_MAX);

		// Mask and crop to ROI
		Matrix roi = source.mask(contours).crop(contours);

		// Show the detected features
		cv::imshow("Camera Feed", roi);

		if (cv::waitKey(1) == 27) { // Wait for escape key press
			break;
		}
	}

	return 0;
}