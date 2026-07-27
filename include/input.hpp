#pragma once

#include <SDL3/SDL.h>

#include <vector>

class Input {
public:
	void beginFrame();
	void poll(bool& shouldQuit);

	bool isHeld(SDL_Scancode scancode) const;
	bool isPressed(SDL_Scancode scancode) const;

	int horizontalAxis() const;
	int verticalAxis() const;

private:
	const bool* currentKeys_ = nullptr;
	int numKeys_ = 0;
	std::vector<bool> previousKeys_;
};
