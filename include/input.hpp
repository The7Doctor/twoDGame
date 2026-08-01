#pragma once

#include <SDL3/SDL.h>

#include <vector>

class Input {
public:
	void beginFrame();
	void poll(bool& shouldQuit);

	bool isHeld(SDL_Scancode scancode) const;
	bool isPressed(SDL_Scancode scancode) const;
	bool isMouseLeftHeld() const;
	bool isMouseLeftPressed() const;
	float mouseX() const;
	float mouseY() const;

	int horizontalAxis() const;
	int verticalAxis() const;

private:
	const bool* currentKeys_ = nullptr;
	int numKeys_ = 0;
	std::vector<bool> previousKeys_;
	SDL_MouseButtonFlags currentMouseButtons_ = 0;
	SDL_MouseButtonFlags previousMouseButtons_ = 0;
	float mouseX_ = 0.0f;
	float mouseY_ = 0.0f;
};
