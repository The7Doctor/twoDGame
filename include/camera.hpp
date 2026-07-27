#pragma once

class Camera {
public:
	void init(float viewportWidth, float viewportHeight, float worldWidth, float worldHeight);
	void follow(float targetX, float targetY, float targetWidth, float targetHeight);

	float x() const;
	float y() const;

private:
	float x_ = 0.0f;
	float y_ = 0.0f;
	float viewportWidth_ = 0.0f;
	float viewportHeight_ = 0.0f;
	float worldWidth_ = 0.0f;
	float worldHeight_ = 0.0f;
};
