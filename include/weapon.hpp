#pragma once

#include "entity.hpp"

#include <vector>

class Biome;
class SquareFriend;
class Renderer;

struct SquareProjectile {
	float x = 0.0f;
	float y = 0.0f;
	float vx = 0.0f;
	float vy = 0.0f;
	float size = 6.0f;
	float remainingLife = 0.0f;
	bool active = false;
};

class SquareWeapon {
public:
	void spawnOnFloor(float x, float y, float size);

	bool isHeld() const;
	bool isOnFloor() const;
	float size() const;

	bool canPickup(float playerCenterX, float playerCenterY, float pickupRadius) const;
	bool tryPickup(float playerCenterX, float playerCenterY, float pickupRadius);
	void dropTo(float worldX, float worldY);

	void getHeldAnchor(float playerX, float playerY, float playerWidth, float playerHeight, float& outX, float& outY) const;
	void tryFire(
		float originX,
		float originY,
		float targetX,
		float targetY,
		float projectileSpeed,
		float projectileLifetime,
		float fireCooldownSeconds
	);

	void update(float deltaTime, Biome& biome, SquareFriend& squareFriend);
	void renderFloor(Renderer& renderer) const;
	void renderHeld(Renderer& renderer, float playerX, float playerY, float playerWidth, float playerHeight) const;
	void renderProjectiles(Renderer& renderer) const;

private:
	bool collidesWithSolid(const Biome& biome, float x, float y, float size) const;

	Entity floorSquare_;
	bool held_ = false;
	float fireCooldownRemaining_ = 0.0f;
	std::vector<SquareProjectile> projectiles_;
};
