#include <SDL3/SDL.h>
#include <array>
#include <cstdio>
#include <cstring>

#include "game.hpp"

#include <algorithm>

namespace {
const std::array<unsigned char, 5>* glyphRowsFor(char c) {
	static const std::array<unsigned char, 5> kSpace {{0b000, 0b000, 0b000, 0b000, 0b000}};
	static const std::array<unsigned char, 5> kZero {{0b111, 0b101, 0b101, 0b101, 0b111}};
	static const std::array<unsigned char, 5> kOne {{0b010, 0b110, 0b010, 0b010, 0b111}};
	static const std::array<unsigned char, 5> kTwo {{0b111, 0b001, 0b111, 0b100, 0b111}};
	static const std::array<unsigned char, 5> kThree {{0b111, 0b001, 0b111, 0b001, 0b111}};
	static const std::array<unsigned char, 5> kFour {{0b101, 0b101, 0b111, 0b001, 0b001}};
	static const std::array<unsigned char, 5> kFive {{0b111, 0b100, 0b111, 0b001, 0b111}};
	static const std::array<unsigned char, 5> kSix {{0b111, 0b100, 0b111, 0b101, 0b111}};
	static const std::array<unsigned char, 5> kSeven {{0b111, 0b001, 0b010, 0b010, 0b010}};
	static const std::array<unsigned char, 5> kEight {{0b111, 0b101, 0b111, 0b101, 0b111}};
	static const std::array<unsigned char, 5> kNine {{0b111, 0b101, 0b111, 0b001, 0b111}};
	static const std::array<unsigned char, 5> kMinus {{0b000, 0b000, 0b111, 0b000, 0b000}};
	static const std::array<unsigned char, 5> kColon {{0b000, 0b010, 0b000, 0b010, 0b000}};
	static const std::array<unsigned char, 5> kComma {{0b000, 0b000, 0b000, 0b010, 0b100}};
	static const std::array<unsigned char, 5> kX {{0b101, 0b101, 0b010, 0b101, 0b101}};
	static const std::array<unsigned char, 5> kY {{0b101, 0b101, 0b010, 0b010, 0b010}};

	switch (c) {
	case '0': return &kZero;
	case '1': return &kOne;
	case '2': return &kTwo;
	case '3': return &kThree;
	case '4': return &kFour;
	case '5': return &kFive;
	case '6': return &kSix;
	case '7': return &kSeven;
	case '8': return &kEight;
	case '9': return &kNine;
	case '-': return &kMinus;
	case ':': return &kColon;
	case ',': return &kComma;
	case 'X': return &kX;
	case 'Y': return &kY;
	case ' ': return &kSpace;
	default: return &kSpace;
	}
}

void drawHudGlyph(Renderer& renderer, float x, float y, char glyph, float pixelSize) {
	const auto* rows = glyphRowsFor(glyph);
	for (int row = 0; row < 5; ++row) {
		for (int col = 0; col < 3; ++col) {
			const unsigned char bitMask = static_cast<unsigned char>(1 << (2 - col));
			if (((*rows)[row] & bitMask) == 0) {
				continue;
			}

			renderer.drawFilledRectScreen(
				x + (static_cast<float>(col) * pixelSize),
				y + (static_cast<float>(row) * pixelSize),
				pixelSize,
				pixelSize,
				236,
				231,
				201,
				255
			);
		}
	}
}

void drawHudTextTopRight(Renderer& renderer, int screenWidth, const char* text) {
	if (!text) {
		return;
	}

	const float pixelSize = 3.0f;
	const float glyphWidth = 3.0f * pixelSize;
	const float glyphHeight = 5.0f * pixelSize;
	const float glyphSpacing = pixelSize;
	const float margin = 10.0f;

	const std::size_t textLength = std::strlen(text);
	const float textWidth = (static_cast<float>(textLength) * glyphWidth)
		+ (std::max(0.0f, static_cast<float>(textLength) - 1.0f) * glyphSpacing);

	const float backgroundPadding = 6.0f;
	const float startX = static_cast<float>(screenWidth) - margin - textWidth;
	const float startY = margin;

	renderer.drawFilledRectScreen(
		startX - backgroundPadding,
		startY - backgroundPadding,
		textWidth + (backgroundPadding * 2.0f),
		glyphHeight + (backgroundPadding * 2.0f),
		12,
		16,
		22,
		220
	);

	for (std::size_t i = 0; i < textLength; ++i) {
		const float glyphX = startX + (static_cast<float>(i) * (glyphWidth + glyphSpacing));
		drawHudGlyph(renderer, glyphX, startY, text[i], pixelSize);
	}
}
}

bool Game::init() {
	if (!renderer_.init(kWindowWidth, kWindowHeight, "twoDGame")) {
		return false;
	}

	if (!biome_.initFlatFloor(kWorldWidth, kWorldHeight, kBlockSize, kFloorRows, kBreakableBlockHp)) {
		return false;
	}

	float spawnX = 0.0f;
	float spawnY = 0.0f;
	constexpr float kPlayerHitboxWidth = 30.0f;
	constexpr float kPlayerHitboxHeight = 42.0f;
	constexpr float kPlayerHitboxOffsetX = 9.0f;
	constexpr float kPlayerHitboxOffsetY = 3.0f;
	const float preferredX = (static_cast<float>(kWorldWidth) * 0.5f) - (kPlayerHitboxWidth * 0.5f);
	if (!biome_.computeSpawn(kPlayerHitboxWidth, kPlayerHitboxHeight, preferredX, spawnX, spawnY)) {
		return false;
	}

	player_.init(
		spawnX - kPlayerHitboxOffsetX,
		spawnY - kPlayerHitboxOffsetY,
		static_cast<float>(kPlayerSize),
		static_cast<float>(kPlayerSize)
	);
	camera_.init(static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), static_cast<float>(kWorldWidth), static_cast<float>(kWorldHeight));
	camera_.follow(player_.x(), player_.y(), player_.width(), player_.height());

	if (!renderer_.loadTextureFromPng("assets/textures/Textures-16.png", biomeTexture_)) {
		renderer_.loadTextureFromPng("../assets/textures/Textures-16.png", biomeTexture_);
	}

	if (!biomeTexture_.isValid()) {
		std::fprintf(stderr, "Biome texture not available. Falling back to rectangle tiles.\n");
	}

	if (!renderer_.loadTextureFromPng("assets/skins/Sprites.png", playerTexture_)) {
		renderer_.loadTextureFromPng("../assets/skins/Sprites.png", playerTexture_);
	}

	if (!playerTexture_.isValid()) {
		std::fprintf(stderr, "Player sprite sheet not available. Falling back to rectangle player.\n");
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
	playerTexture_.unload();
	biomeTexture_.unload();
	renderer_.shutdown();
}

void Game::update(float deltaTime) {
	player_.update(input_, deltaTime, biome_, kWorldWidth, kWorldHeight);
	camera_.follow(player_.x(), player_.y(), player_.width(), player_.height());
}

void Game::render() {
	renderer_.beginFrame(17, 17, 24, 255);
	renderer_.setCameraOffset(camera_.x(), camera_.y());
	const Texture* biomeTexture = biomeTexture_.isValid() ? &biomeTexture_ : nullptr;
	const SDL_FRect* tileSrcRect = biomeTexture ? &biomeTileSrcRect_ : nullptr;
	biome_.renderProto(renderer_, biomeTexture, tileSrcRect);

	const bool drewPlayerSprite = playerTexture_.isValid() && renderer_.drawTexture(
		playerTexture_,
		&playerSpriteSrcRect_,
		player_.x(),
		player_.y(),
		player_.width(),
		player_.height()
	);

	if (!drewPlayerSprite) {
		renderer_.drawFilledRect(player_.x(), player_.y(), player_.width(), player_.height(), 0, 220, 120, 255);
	}

	char positionText[64];
	std::snprintf(
		positionText,
		sizeof(positionText),
		"X:%d Y:%d",
		static_cast<int>(player_.x()),
		static_cast<int>(player_.y())
	);
	drawHudTextTopRight(renderer_, kWindowWidth, positionText);
	renderer_.endFrame();
}

