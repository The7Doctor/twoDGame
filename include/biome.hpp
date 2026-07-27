#pragma once

#include "block.hpp"

#include <vector>

class Renderer;

class Biome {
public:
	bool initFlatFloor(int worldWidth, int worldHeight, int tileSize, int floorRows, int breakableBlockHp);
	bool computeSpawn(float playerWidth, float playerHeight, float preferredX, float& outX, float& outY) const;

	bool isSolidAtWorldPoint(float x, float y) const;
	bool findSolidTopAtSpan(float leftX, float rightX, float worldY, float& outTopY) const;
	int damageSolidTilesInAabb(float leftX, float topY, float rightX, float bottomY, int damageAmount);

	void renderProto(Renderer& renderer) const;

	int tileSize() const;
	int columns() const;
	int rows() const;

private:
	int indexAt(int col, int row) const;
	bool isSolidAtTile(int col, int row) const;
	bool damageTile(int col, int row, int damageAmount);

	int tileSize_ = 16;
	int columns_ = 0;
	int rows_ = 0;
	int breakableBlockHp_ = 1;
	std::vector<Block> blocks_;
};
