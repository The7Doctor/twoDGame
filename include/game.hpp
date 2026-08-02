#pragma once

#include "biome.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "player.hpp"
#include "renderer.hpp"
#include "squarefriend.hpp"
#include "texture.hpp"
#include "weapon.hpp"

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
	static constexpr int kExtraHorizontalBlocksPerSide = 100;
	static constexpr int kBlocksPerPlayer = 3;
	static constexpr int kVisibleFloorRows = 2;
	static constexpr int kExtraDepthRows = 16;
	static constexpr int kFloorRows = kVisibleFloorRows + kExtraDepthRows;
	static constexpr int kBreakableBlockHp = 12;
	static constexpr int kWorldWidth = kWindowWidth + (kExtraHorizontalBlocksPerSide * 2 * kBlockSize);
	static constexpr int kWorldHeight = kWindowHeight + (kExtraDepthRows * kBlockSize);
	static constexpr float kWeaponSize = 14.0f;
	static constexpr float kPickupRadius = 56.0f;
	static constexpr float kWeaponDropOffsetY = 8.0f;
	static constexpr float kProjectileSpeed = 650.0f;
	static constexpr float kProjectileLifetime = 0.8f;
	static constexpr float kFireCooldown = 0.14f;
	static constexpr float kFriendSize = 16.0f;
	static constexpr float kFriendWalkRadius = 140.0f;

	static_assert(kPlayerSize == (kBlockSize * kBlocksPerPlayer), "Player size must be 3 blocks tall");

	bool running_ = false;
	Renderer renderer_;
	Input input_;
	Biome biome_;
	Player player_;
	Camera camera_;
	Texture biomeTexture_;
	SDL_FRect biomeTileSrcRect_ {0.0f, 0.0f, static_cast<float>(kBlockSize), static_cast<float>(kBlockSize)};
	Texture playerTexture_;
	SDL_FRect playerSpriteSrcRect_ {0.0f, 0.0f, 809.0f, 896.0f};
	SquareWeapon squareWeapon_;
	SquareFriend squareFriend_;
};
