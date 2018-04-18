#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <glm/glm.hpp>
class Window {
public:
	Window(const std::string& caption = "Rolling Bogdans, two fast four you!");
	~Window();
	SDL_Window* getView() { return _window; }
	int getWidth() { return _screenWidth; }
	int getHeight() { return _screenHeight; }
	glm::ivec2 getSizes() { return glm::ivec2(_screenWidth, _screenHeight); }
private:
	int _screenWidth = 1280;
	int _screenHeight = 720;
	bool _fullscreen = 0;
	SDL_Window* _window;
	std::string caption;
};