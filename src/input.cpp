#include <SDL3/SDL.h>
#include <cstdio>
#include <cstring>

#include "input.hpp"

void Input::beginFrame() {
	if (!currentKeys_ || numKeys_ <= 0) {
		previousMouseButtons_ = currentMouseButtons_;
		return;
	}

	previousKeys_.assign(currentKeys_, currentKeys_ + numKeys_);
	previousMouseButtons_ = currentMouseButtons_;
}

void Input::poll(bool& shouldQuit) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			shouldQuit = true;
		}
	}

	currentKeys_ = SDL_GetKeyboardState(&numKeys_);
	if (numKeys_ > 0 && static_cast<int>(previousKeys_.size()) != numKeys_) {
		previousKeys_.assign(numKeys_, false);
	}

	currentMouseButtons_ = SDL_GetMouseState(&mouseX_, &mouseY_);
}

bool Input::isHeld(SDL_Scancode scancode) const {
	if (!currentKeys_ || scancode < 0 || scancode >= numKeys_) {
		return false;
	}

	return currentKeys_[scancode];
}

bool Input::isPressed(SDL_Scancode scancode) const {
	if (!currentKeys_ || scancode < 0 || scancode >= numKeys_) {
		return false;
	}

	const bool current = currentKeys_[scancode];
	const bool previous = (scancode < static_cast<int>(previousKeys_.size())) ? previousKeys_[scancode] : false;
	return current && !previous;
}

bool Input::isMouseLeftHeld() const {
	return (currentMouseButtons_ & SDL_BUTTON_LMASK) != 0;
}

bool Input::isMouseLeftPressed() const {
	const bool current = (currentMouseButtons_ & SDL_BUTTON_LMASK) != 0;
	const bool previous = (previousMouseButtons_ & SDL_BUTTON_LMASK) != 0;
	return current && !previous;
}

float Input::mouseX() const {
	return mouseX_;
}

float Input::mouseY() const {
	return mouseY_;
}

int Input::horizontalAxis() const {
	int axis = 0;
	if (isHeld(SDL_SCANCODE_A) || isHeld(SDL_SCANCODE_LEFT)) {
		axis -= 1;
	}
	if (isHeld(SDL_SCANCODE_D) || isHeld(SDL_SCANCODE_RIGHT)) {
		axis += 1;
	}
	return axis;
}

int Input::verticalAxis() const {
	int axis = 0;
	if (isHeld(SDL_SCANCODE_W) || isHeld(SDL_SCANCODE_UP)) {
		axis -= 1;
	}
	if (isHeld(SDL_SCANCODE_S) || isHeld(SDL_SCANCODE_DOWN)) {
		axis += 1;
	}
	return axis;
}

