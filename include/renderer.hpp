#pragma once

#include <SDL3/SDL.h>

#include "texture.hpp"

class Renderer {
public:
	bool init(int width, int height, const char* title);
	void shutdown();

	void beginFrame(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void setCameraOffset(float x, float y);
	void drawFilledRect(float x, float y, float w, float h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void drawFilledRectScreen(float x, float y, float w, float h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	bool loadTextureFromPng(const char* filePath, Texture& outTexture, bool whiteColorKey = false);
	bool drawTexture(const Texture& texture, const SDL_FRect* srcRect, float x, float y, float w, float h);
	void endFrame();

	bool isReady() const;

private:
	SDL_Window* window_ = nullptr;
	SDL_Renderer* renderer_ = nullptr;
	float cameraX_ = 0.0f;
	float cameraY_ = 0.0f;
};
