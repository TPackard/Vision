#ifndef VISION_CONTOURS_H
#define VISION_CONTOURS_H

#include "vision.h"

using Contour = std::vector<cv::Point>;

class Contours: public std::vector<Contour> {
public:
	Contours * weedBySize(int min, int max);
};

#endif //VISION_CONTOURS_H
