#pragma once

#include <SDL3/SDL_render.h>

class Texture {
public:
	Texture() = default;
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	bool isValid() const;
	float width() const;
	float height() const;

	void unload();

private:
	friend class Renderer;

	void takeOwnership(SDL_Texture* texture, float width, float height);

	SDL_Texture* texture_ = nullptr;
	float width_ = 0.0f;
	float height_ = 0.0f;
};
