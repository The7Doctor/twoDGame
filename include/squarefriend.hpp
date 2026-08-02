#pragma once

#include "entity.hpp"

class Biome;
class Renderer;

class SquareFriend : public Entity {
public:
	void spawnOnSurface(float x, float y, float size, float leftBound, float rightBound);
	void update(float deltaTime, const Biome& biome) override;
	void render(Renderer& renderer) const override;

private:
	float speed_ = 90.0f;
	float direction_ = 1.0f;
	float leftBound_ = 0.0f;
	float rightBound_ = 0.0f;
	float walkFlipTimer_ = 0.0f;
	float walkFlipInterval_ = 0.0f;
};