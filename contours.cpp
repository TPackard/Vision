#include "contours.h"

Contours * Contours::weedBySize(int min, int max) {
	erase(std::remove_if(begin(), end(), [&](Contour c) {
		return cv::contourArea(c) < min || cv::contourArea(c) > max; // Remove if it's outside of the size bounds
	}), end());

	return this;
}
