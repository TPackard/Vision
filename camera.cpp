#include "camera.h"

Camera::Camera(int device) {
	videoCapture.open(device);

	// Checks to make sure the connection to the camera is working
	if (!videoCapture.isOpened()) {
		std::cerr << "No camera!" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

bool Camera::isOpened() {
	return videoCapture.isOpened();
}

Matrix Camera::read() {
	Matrix image;
	videoCapture.read(image);
	return image;
}