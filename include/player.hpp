#pragma once

class Input;
class Biome;

class Player {
public:
	void init(float x, float y, float w = 64.0f, float h = 64.0f);
	void update(const Input& input, float deltaTime, Biome& biome, int worldWidth, int worldHeight);

	float x() const;
	float y() const;
	float width() const;
	float height() const;
	bool isGrounded() const;

private:
	float x_ = 0.0f;
	float y_ = 0.0f;
	float width_ = 48.0f;
	float height_ = 48.0f;
	float runSpeed_ = 260.0f;
	float velocityY_ = 0.0f;
	float gravity_ = 1800.0f;
	float diveAcceleration_ = 2600.0f;
	float maxFallSpeed_ = 2200.0f;
	float jumpVelocity_ = -720.0f;
	float minBreakSpeed_ = 700.0f;
	float breakDamageScale_ = 0.008f;
	int maxBreakDamagePerFrame_ = 6;
	bool grounded_ = false;
};
