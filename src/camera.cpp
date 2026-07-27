#include "camera.hpp"

#include <algorithm>

void Camera::init(float viewportWidth, float viewportHeight, float worldWidth, float worldHeight) {
	viewportWidth_ = viewportWidth;
	viewportHeight_ = viewportHeight;
	worldWidth_ = worldWidth;
	worldHeight_ = worldHeight;
	x_ = 0.0f;
	y_ = 0.0f;
}

void Camera::follow(float targetX, float targetY, float targetWidth, float targetHeight) {
	const float targetCenterX = targetX + (targetWidth * 0.5f);
	const float targetCenterY = targetY + (targetHeight * 0.5f);

	const float desiredX = targetCenterX - (viewportWidth_ * 0.5f);
	const float desiredY = targetCenterY - (viewportHeight_ * 0.5f);

	const float maxX = std::max(0.0f, worldWidth_ - viewportWidth_);
	const float maxY = std::max(0.0f, worldHeight_ - viewportHeight_);

	x_ = std::clamp(desiredX, 0.0f, maxX);
	y_ = std::clamp(desiredY, 0.0f, maxY);
}

float Camera::x() const {
	return x_;
}

float Camera::y() const {
	return y_;
}
