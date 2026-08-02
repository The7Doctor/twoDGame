#include "squarefriend.hpp"

#include "biome.hpp"
#include "renderer.hpp"

#include <algorithm>
#include <random>

namespace {
constexpr Uint8 kFriendR = 124;
constexpr Uint8 kFriendG = 214;
constexpr Uint8 kFriendB = 130;

float findSurfaceYAtX(const Biome& biome, float worldX) {
	const int tileSize = biome.tileSize();
	if (tileSize <= 0 || biome.rows() <= 0) {
		return 0.0f;
	}

	for (int row = 0; row < biome.rows(); ++row) {
		const float probeY = (static_cast<float>(row) * static_cast<float>(tileSize)) + 1.0f;
		if (biome.isSolidAtWorldPoint(worldX, probeY)) {
			return static_cast<float>(row * tileSize);
		}
	}

	return 0.0f;
}

float randomFloat(float minValue, float maxValue) {
	static std::mt19937 rng{std::random_device{}()};
	std::uniform_real_distribution<float> distribution(minValue, maxValue);
	return distribution(rng);
}

float randomDirection() {
	return (randomFloat(0.0f, 1.0f) < 0.5f) ? -1.0f : 1.0f;
}
}

void SquareFriend::spawnOnSurface(float x, float y, float size, float leftBound, float rightBound) {
	setPosition(x, y);
	setSize(size, size);
	setActive(true);
	health_ = 3;
	leftBound_ = std::min(leftBound, rightBound);
	rightBound_ = std::max(leftBound, rightBound);
	direction_ = randomDirection();
	walkFlipTimer_ = 0.0f;
	walkFlipInterval_ = randomFloat(1.1f, 2.7f);
	speed_ = randomFloat(70.0f, 120.0f);
}

void SquareFriend::update(float deltaTime, const Biome& biome) {
	if (!isActive()) {
		return;
	}

	walkFlipTimer_ += deltaTime;
	if (walkFlipTimer_ >= walkFlipInterval_) {
		walkFlipTimer_ = 0.0f;
		walkFlipInterval_ = randomFloat(1.1f, 2.7f);
		direction_ = -direction_;
	}

	const float nextX = x_ + (direction_ * speed_ * deltaTime);
	const float clampedX = std::clamp(nextX, leftBound_, rightBound_);
	const bool hitLeftEdge = (clampedX <= leftBound_ + 0.01f) && (direction_ < 0.0f);
	const bool hitRightEdge = (clampedX >= rightBound_ - 0.01f) && (direction_ > 0.0f);
	if (hitLeftEdge || hitRightEdge) {
		direction_ = -direction_;
	}

	x_ = clampedX;

	const float surfaceY = findSurfaceYAtX(biome, centerX());
	y_ = surfaceY - height_;
}

void SquareFriend::render(Renderer& renderer) const {
	if (!isActive()) {
		return;
	}

	renderer.drawFilledRect(x_, y_, width_, height_, kFriendR, kFriendG, kFriendB, 255);
}

bool SquareFriend::takeDamage(int amount) {
	if (!isActive() || amount <= 0) {
		return false;
	}

	health_ -= amount;
	if (health_ <= 0) {
		setActive(false);
		return true;
	}

	return false;
}