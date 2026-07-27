#include "player.hpp"

#include "biome.hpp"
#include "input.hpp"

#include <algorithm>
#include <SDL3/SDL.h>

void Player::init(float x, float y, float w, float h) {
	x_ = x;
	y_ = y;
	width_ = w;
	height_ = h;
	velocityY_ = 0.0f;
	grounded_ = false;
}

void Player::update(const Input& input, float deltaTime, Biome& biome, int worldWidth, int worldHeight) {
	const float moveX = static_cast<float>(input.horizontalAxis());
	float nextX = x_ + (moveX * runSpeed_ * deltaTime);

	if (moveX != 0.0f) {
		const float topProbeY = y_ + 2.0f;
		const float bottomProbeY = y_ + height_ - 2.0f;
		const float tileSize = static_cast<float>(biome.tileSize());

		if (moveX > 0.0f) {
			const float rightProbeX = nextX + width_;
			if (biome.isSolidAtWorldPoint(rightProbeX, topProbeY) || biome.isSolidAtWorldPoint(rightProbeX, bottomProbeY)) {
				const int hitCol = static_cast<int>(rightProbeX / tileSize);
				nextX = (static_cast<float>(hitCol) * tileSize) - width_ - 0.01f;
			}
		} else {
			const float leftProbeX = nextX;
			if (biome.isSolidAtWorldPoint(leftProbeX, topProbeY) || biome.isSolidAtWorldPoint(leftProbeX, bottomProbeY)) {
				const int hitCol = static_cast<int>(leftProbeX / tileSize);
				nextX = (static_cast<float>(hitCol + 1) * tileSize) + 0.01f;
			}
		}
	}

	x_ = nextX;

	const float maxX = std::max(0.0f, static_cast<float>(worldWidth) - width_);
	x_ = std::clamp(x_, 0.0f, maxX);

	if (grounded_ && input.isPressed(SDL_SCANCODE_SPACE)) {
		velocityY_ = jumpVelocity_;
		grounded_ = false;
	}

	const bool wantsDive = (!grounded_ && input.verticalAxis() > 0);
	velocityY_ += gravity_ * deltaTime;
	if (wantsDive) {
		velocityY_ += diveAcceleration_ * deltaTime;
	}
	velocityY_ = std::min(velocityY_, maxFallSpeed_);

	const float previousBottom = y_ + height_;
	float nextY = y_ + (velocityY_ * deltaTime);

	if (velocityY_ >= 0.0f) {
		const float leftProbe = x_ + 2.0f;
		const float rightProbe = x_ + width_ - 2.0f;
		const float nextBottom = nextY + height_;

		if (wantsDive && velocityY_ > minBreakSpeed_) {
			const float rawDamage = (velocityY_ - minBreakSpeed_) * breakDamageScale_;
			const int scaledDamage = static_cast<int>(rawDamage);
			const int damageAmount = std::clamp(scaledDamage, 1, maxBreakDamagePerFrame_);
			biome.damageSolidTilesInAabb(leftProbe, previousBottom, rightProbe, nextBottom, damageAmount);
		}

		float topY = 0.0f;
		const float probeY = nextBottom;

		if (biome.findSolidTopAtSpan(leftProbe, rightProbe, probeY, topY)) {
			nextY = topY - height_;
			velocityY_ = 0.0f;
			grounded_ = true;
		} else {
			grounded_ = false;
		}
	}

	if (nextY < 0.0f) {
		nextY = 0.0f;
		if (velocityY_ < 0.0f) {
			velocityY_ = 0.0f;
		}
	}

	const float maxY = std::max(0.0f, static_cast<float>(worldHeight) - height_);
	if (nextY > maxY) {
		nextY = maxY;
		velocityY_ = 0.0f;
		grounded_ = true;
	}

	y_ = nextY;

	// Keep grounded state stable when standing exactly on block tops.
	if (velocityY_ == 0.0f) {
		float topY = 0.0f;
		const float standProbeY = y_ + height_ + 1.0f;
		const float leftProbe = x_ + 2.0f;
		const float rightProbe = x_ + width_ - 2.0f;
		grounded_ = biome.findSolidTopAtSpan(leftProbe, rightProbe, standProbeY, topY);
	}
}

float Player::x() const {
	return x_;
}

float Player::y() const {
	return y_;
}

float Player::width() const {
	return width_;
}

float Player::height() const {
	return height_;
}

bool Player::isGrounded() const {
	return grounded_;
}
