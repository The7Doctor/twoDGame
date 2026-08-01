#include "weapon.hpp"

#include "biome.hpp"
#include "renderer.hpp"

#include <algorithm>
#include <cmath>

namespace {
constexpr float kDirectionEpsilon = 0.001f;
constexpr Uint8 kFloorItemR = 88;
constexpr Uint8 kFloorItemG = 180;
constexpr Uint8 kFloorItemB = 255;
constexpr Uint8 kHeldItemR = 255;
constexpr Uint8 kHeldItemG = 238;
constexpr Uint8 kHeldItemB = 120;
constexpr Uint8 kProjectileR = 255;
constexpr Uint8 kProjectileG = 255;
constexpr Uint8 kProjectileB = 170;
}

void SquareWeapon::spawnOnFloor(float x, float y, float size) {
	floorSquare_.setPosition(x, y);
	floorSquare_.setSize(size, size);
	floorSquare_.setActive(true);
	held_ = false;
	fireCooldownRemaining_ = 0.0f;
	projectiles_.clear();
}

bool SquareWeapon::isHeld() const {
	return held_;
}

bool SquareWeapon::isOnFloor() const {
	return floorSquare_.isActive();
}

float SquareWeapon::size() const {
	return floorSquare_.width();
}

bool SquareWeapon::canPickup(float playerCenterX, float playerCenterY, float pickupRadius) const {
	if (held_ || !floorSquare_.isActive() || pickupRadius <= 0.0f) {
		return false;
	}

	const float dx = floorSquare_.centerX() - playerCenterX;
	const float dy = floorSquare_.centerY() - playerCenterY;
	const float distanceSquared = (dx * dx) + (dy * dy);
	const float pickupSquared = pickupRadius * pickupRadius;
	return distanceSquared <= pickupSquared;
}

bool SquareWeapon::tryPickup(float playerCenterX, float playerCenterY, float pickupRadius) {
	if (!canPickup(playerCenterX, playerCenterY, pickupRadius)) {
		return false;
	}

	held_ = true;
	floorSquare_.setActive(false);
	return true;
}

void SquareWeapon::dropTo(float worldX, float worldY) {
	if (!held_) {
		return;
	}

	held_ = false;
	floorSquare_.setPosition(worldX, worldY);
	floorSquare_.setActive(true);
}

void SquareWeapon::getHeldAnchor(
	float playerX,
	float playerY,
	float playerWidth,
	float playerHeight,
	float& outX,
	float& outY
) const {
	const float itemSize = floorSquare_.width();
	outX = playerX + playerWidth - (itemSize * 0.3f);
	outY = playerY + (playerHeight * 0.5f) - (itemSize * 0.5f);
}

void SquareWeapon::tryFire(
	float originX,
	float originY,
	float targetX,
	float targetY,
	float projectileSpeed,
	float projectileLifetime,
	float fireCooldownSeconds
) {
	if (!held_ || fireCooldownRemaining_ > 0.0f || projectileSpeed <= 0.0f || projectileLifetime <= 0.0f) {
		return;
	}

	const float dirX = targetX - originX;
	const float dirY = targetY - originY;
	const float length = std::sqrt((dirX * dirX) + (dirY * dirY));
	if (length <= kDirectionEpsilon) {
		return;
	}

	const float normalizedX = dirX / length;
	const float normalizedY = dirY / length;

	SquareProjectile projectile;
	projectile.size = std::max(5.0f, floorSquare_.width() * 0.45f);
	projectile.x = originX - (projectile.size * 0.5f);
	projectile.y = originY - (projectile.size * 0.5f);
	projectile.vx = normalizedX * projectileSpeed;
	projectile.vy = normalizedY * projectileSpeed;
	projectile.remainingLife = projectileLifetime;
	projectile.active = true;
 projectiles_.push_back(projectile);

	fireCooldownRemaining_ = std::max(0.0f, fireCooldownSeconds);
}

void SquareWeapon::update(float deltaTime, const Biome& biome) {
	fireCooldownRemaining_ = std::max(0.0f, fireCooldownRemaining_ - deltaTime);

	for (SquareProjectile& projectile : projectiles_) {
		if (!projectile.active) {
			continue;
		}

		projectile.remainingLife -= deltaTime;
		if (projectile.remainingLife <= 0.0f) {
			projectile.active = false;
			continue;
		}

		const float nextX = projectile.x + (projectile.vx * deltaTime);
		const float nextY = projectile.y + (projectile.vy * deltaTime);

		if (collidesWithSolid(biome, nextX, nextY, projectile.size)) {
			projectile.active = false;
			continue;
		}

		projectile.x = nextX;
		projectile.y = nextY;
	}

	projectiles_.erase(
		std::remove_if(
			projectiles_.begin(),
			projectiles_.end(),
			[](const SquareProjectile& projectile) {
				return !projectile.active;
			}
		),
		projectiles_.end()
	);
}

void SquareWeapon::renderFloor(Renderer& renderer) const {
	if (!floorSquare_.isActive()) {
		return;
	}

	renderer.drawFilledRect(
		floorSquare_.x(),
		floorSquare_.y(),
		floorSquare_.width(),
		floorSquare_.height(),
		kFloorItemR,
		kFloorItemG,
		kFloorItemB,
		255
	);
}

void SquareWeapon::renderHeld(Renderer& renderer, float playerX, float playerY, float playerWidth, float playerHeight) const {
	if (!held_) {
		return;
	}

	float handX = 0.0f;
	float handY = 0.0f;
	getHeldAnchor(playerX, playerY, playerWidth, playerHeight, handX, handY);

	renderer.drawFilledRect(
		handX,
		handY,
		floorSquare_.width(),
		floorSquare_.height(),
		kHeldItemR,
		kHeldItemG,
		kHeldItemB,
		255
	);
}

void SquareWeapon::renderProjectiles(Renderer& renderer) const {
	for (const SquareProjectile& projectile : projectiles_) {
		if (!projectile.active) {
			continue;
		}

		renderer.drawFilledRect(
			projectile.x,
			projectile.y,
			projectile.size,
			projectile.size,
			kProjectileR,
			kProjectileG,
			kProjectileB,
			255
		);
	}
}

bool SquareWeapon::collidesWithSolid(const Biome& biome, float x, float y, float size) const {
	const float inset = std::min(2.0f, size * 0.25f);
	const float left = x + inset;
	const float right = x + size - inset;
	const float top = y + inset;
	const float bottom = y + size - inset;
	const float centerX = x + (size * 0.5f);
	const float centerY = y + (size * 0.5f);

	return biome.isSolidAtWorldPoint(left, top)
		|| biome.isSolidAtWorldPoint(right, top)
		|| biome.isSolidAtWorldPoint(left, bottom)
		|| biome.isSolidAtWorldPoint(right, bottom)
		|| biome.isSolidAtWorldPoint(centerX, centerY);
}
