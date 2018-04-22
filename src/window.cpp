#include "window.hpp"
#include <SDL2/SDL_image.h>

Window::Window(const std::string& caption) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		printf("Couldn't initalize SDL");
	else{
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}

	SDL_GL_LoadLibrary(NULL);

	if (_fullscreen) {
		_window = SDL_CreateWindow(
			caption.c_str(),
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
		);
	}else{
		_window = SDL_CreateWindow(
			caption.c_str(),
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			_screenWidth, _screenHeight, SDL_WINDOW_OPENGL
		);
	}

	if (_window == NULL)
		printf("SDL failed to create window!\n");
	else
		printf("SDL successfully created a window!\n");

	SDL_ShowCursor(0);
}

Window::~Window() {
	
}