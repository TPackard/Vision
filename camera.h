#ifndef VISION_CAMERA_H
#define VISION_CAMERA_H

#include "vision.h"

class Matrix; // Forward declaration

/// The Camera class is a wrapper for OpenCV camera functions. It supports the creation of new cameras.
class Camera {
public:
	/// Creates a new camera feed from the specified device, defaulting to the computer's built in camera
	Camera(int device = 0);

	/// Returns whether or not the connection the camera is opened
	bool isOpened();

	/// Returns the camera's current image
	Matrix read();
private:
	/// The OpenCV camera feed object
	cv::VideoCapture videoCapture;
};

#endif //VISION_CAMERA_H
