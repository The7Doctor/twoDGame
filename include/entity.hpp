
#pragma once

class Biome;
class Renderer;

class Entity {
public:
	virtual ~Entity() = default;

	void setPosition(float x, float y) {
		x_ = x;
		y_ = y;
	}

	void setSize(float width, float height) {
		width_ = width;
		height_ = height;
	}

	float x() const {
		return x_;
	}

	float y() const {
		return y_;
	}

	float width() const {
		return width_;
	}

	float height() const {
		return height_;
	}

	float centerX() const {
		return x_ + (width_ * 0.5f);
	}

	float centerY() const {
		return y_ + (height_ * 0.5f);
	}

	bool isActive() const {
		return active_;
	}

	void setActive(bool active) {
		active_ = active;
	}

	virtual void update(float /*deltaTime*/, const Biome& /*biome*/) {
	}

	virtual void render(Renderer& /*renderer*/) const {
	}

protected:
	float x_ = 0.0f;
	float y_ = 0.0f;
	float width_ = 0.0f;
	float height_ = 0.0f;
	bool active_ = true;
};

