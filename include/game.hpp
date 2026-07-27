#pragma once

#include "biome.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "player.hpp"
#include "renderer.hpp"

class Game {
public:
	bool init();
	void run();
	void shutdown();

private:
	void update(float deltaTime);
	void render();

	static constexpr int kWindowWidth = 800;
	static constexpr int kWindowHeight = 600;
	static constexpr int kPlayerSize = 48;
	static constexpr int kBlockSize = 16;
	static constexpr int kBlocksPerPlayer = 3;
	static constexpr int kVisibleFloorRows = 2;
	static constexpr int kExtraDepthRows = 16;
	static constexpr int kFloorRows = kVisibleFloorRows + kExtraDepthRows;
	static constexpr int kBreakableBlockHp = 12;
	static constexpr int kWorldHeight = kWindowHeight + (kExtraDepthRows * kBlockSize);

	static_assert(kPlayerSize == (kBlockSize * kBlocksPerPlayer), "Player size must be 3 blocks tall");

	bool running_ = false;
	Renderer renderer_;
	Input input_;
	Biome biome_;
	Player player_;
	Camera camera_;
};
