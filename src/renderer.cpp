#include "renderer.hpp"

#include <cstdio>

bool Renderer::init(int width, int height, const char* title) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
		return false;
	}

	window_ = SDL_CreateWindow(title, width, height, 0);
	if (!window_) {
		std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
		SDL_Quit();
		return false;
	}

	renderer_ = SDL_CreateRenderer(window_, nullptr);
	if (!renderer_) {
		std::fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
		SDL_DestroyWindow(window_);
		window_ = nullptr;
		SDL_Quit();
		return false;
	}

	return true;
}

void Renderer::shutdown() {
	if (renderer_) {
		SDL_DestroyRenderer(renderer_);
		renderer_ = nullptr;
	}

	if (window_) {
		SDL_DestroyWindow(window_);
		window_ = nullptr;
	}

	SDL_Quit();
}

void Renderer::beginFrame(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	if (!renderer_) {
		return;
	}

	SDL_SetRenderDrawColor(renderer_, r, g, b, a);
	SDL_RenderClear(renderer_);
}

void Renderer::setCameraOffset(float x, float y) {
	cameraX_ = x;
	cameraY_ = y;
}

void Renderer::drawFilledRect(float x, float y, float w, float h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	if (!renderer_) {
		return;
	}

	SDL_SetRenderDrawColor(renderer_, r, g, b, a);

	SDL_FRect rect;
	rect.x = x - cameraX_;
	rect.y = y - cameraY_;
	rect.w = w;
	rect.h = h;

	SDL_RenderFillRect(renderer_, &rect);
}

void Renderer::endFrame() {
	if (!renderer_) {
		return;
	}

	SDL_RenderPresent(renderer_);
}

bool Renderer::isReady() const {
	return (window_ != nullptr && renderer_ != nullptr);
}
