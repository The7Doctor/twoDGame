#include "texture.hpp"

Texture::~Texture() {
	unload();
}

Texture::Texture(Texture&& other) noexcept
	: texture_(other.texture_), width_(other.width_), height_(other.height_) {
	other.texture_ = nullptr;
	other.width_ = 0.0f;
	other.height_ = 0.0f;
}

Texture& Texture::operator=(Texture&& other) noexcept {
	if (this == &other) {
		return *this;
	}

	unload();
	texture_ = other.texture_;
	width_ = other.width_;
	height_ = other.height_;

	other.texture_ = nullptr;
	other.width_ = 0.0f;
	other.height_ = 0.0f;

	return *this;
}

bool Texture::isValid() const {
	return (texture_ != nullptr);
}

float Texture::width() const {
	return width_;
}

float Texture::height() const {
	return height_;
}

void Texture::unload() {
	if (texture_) {
		SDL_DestroyTexture(texture_);
		texture_ = nullptr;
	}

	width_ = 0.0f;
	height_ = 0.0f;
}

void Texture::takeOwnership(SDL_Texture* texture, float width, float height) {
	unload();
	texture_ = texture;
	width_ = width;
	height_ = height;
}