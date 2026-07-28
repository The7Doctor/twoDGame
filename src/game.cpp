#include <SDL3/SDL.h>
#include <cstdio>
#include <cstring>

#include "game.hpp"

#include <algorithm>

bool Game::init() {
	if (!renderer_.init(kWindowWidth, kWindowHeight, "twoDGame")) {
		return false;
	}

	if (!biome_.initFlatFloor(kWindowWidth, kWorldHeight, kBlockSize, kFloorRows, kBreakableBlockHp)) {
		return false;
	}

	float spawnX = 0.0f;
	float spawnY = 0.0f;
	const float preferredX = (static_cast<float>(kWindowWidth) * 0.5f) - (static_cast<float>(kPlayerSize) * 0.5f);
	if (!biome_.computeSpawn(static_cast<float>(kPlayerSize), static_cast<float>(kPlayerSize), preferredX, spawnX, spawnY)) {
		return false;
	}

	player_.init(spawnX, spawnY, static_cast<float>(kPlayerSize), static_cast<float>(kPlayerSize));
	camera_.init(static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), static_cast<float>(kWindowWidth), static_cast<float>(kWorldHeight));
	camera_.follow(player_.x(), player_.y(), player_.width(), player_.height());

	if (!renderer_.loadTextureFromPng("assets/textures/Textures-16.png", biomeTexture_)) {
		renderer_.loadTextureFromPng("../assets/textures/Textures-16.png", biomeTexture_);
	}

	if (!biomeTexture_.isValid()) {
		std::fprintf(stderr, "Biome texture not available. Falling back to rectangle tiles.\n");
	}

	running_ = true;
	return true;
}

void Game::run() {
	Uint64 lastTick = SDL_GetTicks();

	while (running_) {
		const Uint64 now = SDL_GetTicks();
		float deltaTime = static_cast<float>(now - lastTick) / 1000.0f;
		lastTick = now;

		// Clamp large spikes so movement does not jump after stalls.
		deltaTime = std::min(deltaTime, 0.05f);

		bool shouldQuit = false;
		input_.beginFrame();
		input_.poll(shouldQuit);
		if (shouldQuit) {
			running_ = false;
		}

		update(deltaTime);
		render();
	}
}

void Game::shutdown() {
	biomeTexture_.unload();
	renderer_.shutdown();
}

void Game::update(float deltaTime) {
	player_.update(input_, deltaTime, biome_, kWindowWidth, kWorldHeight);
	camera_.follow(player_.x(), player_.y(), player_.width(), player_.height());
}

void Game::render() {
	renderer_.beginFrame(17, 17, 24, 255);
	renderer_.setCameraOffset(camera_.x(), camera_.y());
	const Texture* biomeTexture = biomeTexture_.isValid() ? &biomeTexture_ : nullptr;
	const SDL_FRect* tileSrcRect = biomeTexture ? &biomeTileSrcRect_ : nullptr;
	biome_.renderProto(renderer_, biomeTexture, tileSrcRect);
	renderer_.drawFilledRect(player_.x(), player_.y(), player_.width(), player_.height(), 0, 220, 120, 255);
	renderer_.endFrame();
}

