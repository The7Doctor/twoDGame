#include "biome.hpp"

#include "renderer.hpp"
#include "texture.hpp"

#include <algorithm>
#include <cmath>

bool Biome::initFlatFloor(int worldWidth, int worldHeight, int tileSize, int floorRows, int breakableBlockHp) {
	if (worldWidth <= 0 || worldHeight <= 0 || tileSize <= 0 || floorRows <= 0 || breakableBlockHp <= 0) {
		return false;
	}

	tileSize_ = tileSize;
	breakableBlockHp_ = breakableBlockHp;
	columns_ = (worldWidth + tileSize_ - 1) / tileSize_;
	rows_ = (worldHeight + tileSize_ - 1) / tileSize_;

	if (columns_ <= 0 || rows_ <= 0) {
		return false;
	}

	blocks_.assign(static_cast<size_t>(columns_ * rows_), Block{});

	const int firstSolidRow = std::max(0, rows_ - floorRows);
	for (int row = firstSolidRow; row < rows_; ++row) {
		for (int col = 0; col < columns_; ++col) {
			Block& block = blocks_[indexAt(col, row)];
			block.type = BlockType::SolidProto;
			block.hp = breakableBlockHp_;
			block.unbreakable = (row == (rows_ - 1));
		}
	}

	return true;
}

bool Biome::computeSpawn(float playerWidth, float playerHeight, float preferredX, float& outX, float& outY) const {
	if (columns_ <= 0 || rows_ <= 0 || tileSize_ <= 0 || blocks_.empty()) {
		return false;
	}

	const float maxX = static_cast<float>(columns_ * tileSize_) - playerWidth;
	const float clampedX = std::clamp(preferredX, 0.0f, std::max(0.0f, maxX));
	const float centerX = clampedX + (playerWidth * 0.5f);
	const int targetCol = static_cast<int>(std::floor(centerX / static_cast<float>(tileSize_)));
	const int clampedCol = std::clamp(targetCol, 0, columns_ - 1);

	for (int row = 0; row < rows_; ++row) {
		if (isSolidAtTile(clampedCol, row)) {
			outX = clampedX;
			outY = static_cast<float>(row * tileSize_) - playerHeight;
			return true;
		}
	}

	return false;
}

bool Biome::isSolidAtWorldPoint(float x, float y) const {
	if (columns_ <= 0 || rows_ <= 0 || tileSize_ <= 0) {
		return false;
	}

	const int col = static_cast<int>(std::floor(x / static_cast<float>(tileSize_)));
	const int row = static_cast<int>(std::floor(y / static_cast<float>(tileSize_)));
	return isSolidAtTile(col, row);
}

bool Biome::findSolidTopAtSpan(float leftX, float rightX, float worldY, float& outTopY) const {
	if (columns_ <= 0 || rows_ <= 0 || tileSize_ <= 0) {
		return false;
	}

	if (rightX < leftX) {
		std::swap(leftX, rightX);
	}

	const int row = static_cast<int>(std::floor(worldY / static_cast<float>(tileSize_)));
	if (row < 0 || row >= rows_) {
		return false;
	}

	int leftCol = static_cast<int>(std::floor(leftX / static_cast<float>(tileSize_)));
	int rightCol = static_cast<int>(std::floor(rightX / static_cast<float>(tileSize_)));

	leftCol = std::clamp(leftCol, 0, columns_ - 1);
	rightCol = std::clamp(rightCol, 0, columns_ - 1);

	for (int col = leftCol; col <= rightCol; ++col) {
		if (isSolidAtTile(col, row)) {
			outTopY = static_cast<float>(row * tileSize_);
			return true;
		}
	}

	return false;
}

int Biome::damageSolidTilesInAabb(float leftX, float topY, float rightX, float bottomY, int damageAmount) {
	if (columns_ <= 0 || rows_ <= 0 || tileSize_ <= 0 || damageAmount <= 0) {
		return 0;
	}

	if (rightX < leftX) {
		std::swap(leftX, rightX);
	}
	if (bottomY < topY) {
		std::swap(topY, bottomY);
	}

	int leftCol = static_cast<int>(std::floor(leftX / static_cast<float>(tileSize_)));
	int rightCol = static_cast<int>(std::floor(rightX / static_cast<float>(tileSize_)));
	int topRow = static_cast<int>(std::floor(topY / static_cast<float>(tileSize_)));
	int bottomRow = static_cast<int>(std::floor(bottomY / static_cast<float>(tileSize_)));

	leftCol = std::clamp(leftCol, 0, columns_ - 1);
	rightCol = std::clamp(rightCol, 0, columns_ - 1);
	topRow = std::clamp(topRow, 0, rows_ - 1);
	bottomRow = std::clamp(bottomRow, 0, rows_ - 1);

	int destroyedCount = 0;
	for (int row = topRow; row <= bottomRow; ++row) {
		for (int col = leftCol; col <= rightCol; ++col) {
			if (damageTile(col, row, damageAmount)) {
				++destroyedCount;
			}
		}
	}

	return destroyedCount;
}

void Biome::renderProto(Renderer& renderer, const Texture* tileTexture, const SDL_FRect* tileSrcRect) const {
	if (columns_ <= 0 || rows_ <= 0 || tileSize_ <= 0) {
		return;
	}

	for (int row = 0; row < rows_; ++row) {
		for (int col = 0; col < columns_; ++col) {
			if (!isSolidAtTile(col, row)) {
				continue;
			}

			const float worldX = static_cast<float>(col * tileSize_);
			const float worldY = static_cast<float>(row * tileSize_);
			const float tileSize = static_cast<float>(tileSize_);

			if (tileTexture && tileSrcRect && renderer.drawTexture(*tileTexture, tileSrcRect, worldX, worldY, tileSize, tileSize)) {
				continue;
			}

			renderer.drawFilledRect(
				worldX,
				worldY,
				tileSize,
				tileSize,
				102,
				92,
				81,
				255
			);
		}
	}
}

int Biome::tileSize() const {
	return tileSize_;
}

int Biome::columns() const {
	return columns_;
}

int Biome::rows() const {
	return rows_;
}

int Biome::indexAt(int col, int row) const {
	return (row * columns_) + col;
}

bool Biome::isSolidAtTile(int col, int row) const {
	if (col < 0 || col >= columns_ || row < 0 || row >= rows_) {
		return false;
	}

	return blocks_[indexAt(col, row)].isSolid();
}

bool Biome::damageTile(int col, int row, int damageAmount) {
	if (col < 0 || col >= columns_ || row < 0 || row >= rows_) {
		return false;
	}

	Block& block = blocks_[indexAt(col, row)];
	return block.applyDamage(damageAmount);
}
