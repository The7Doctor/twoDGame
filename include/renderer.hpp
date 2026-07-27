#pragma once

#include <SDL3/SDL.h>

class Renderer {
public:
	bool init(int width, int height, const char* title);
	void shutdown();

	void beginFrame(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void setCameraOffset(float x, float y);
	void drawFilledRect(float x, float y, float w, float h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void endFrame();

	bool isReady() const;

private:
	SDL_Window* window_ = nullptr;
	SDL_Renderer* renderer_ = nullptr;
	float cameraX_ = 0.0f;
	float cameraY_ = 0.0f;
};
