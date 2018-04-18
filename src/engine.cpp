#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <stdio.h>
#include "engine.hpp"

Engine* Engine::_instance;

Engine::Engine() {

}

int Engine::run() {
	_init();

	SDL_Event event;

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;

	bool quit = false;

	while (!quit) {
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = ((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency()) * 0.001;
		SDL_GL_SwapWindow(_screen->getView());

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				quit = true;
				break;
			default:
				break;
			}
			switch (event.button.button) {
			case SDL_BUTTON_RIGHT:
				printf("Lmao\n");
				break;
			default:
				break;
			}
		}

		//_geometryPass->useProgram();
		//_geometryPass->setValue(22, 0);
		//_testTexture->bind(22);
		//_geometryPass->setValue(0, _camera->getComponent<CameraComponent>().getProjectionMatrix());
		//_geometryPass->setValue(1, _camera->getComponent<CameraComponent>().getViewMatrix());
		//_geometryPass->setValue(2, glm::mat4(1));
		//_renderer->render(_screen.get(), _geometryPass);
	}

	return 0;
}

void Engine::_init() {
	_initializeSDL();
	_initializeGL();

	_meshLoader = std::make_unique<MeshLoader>();
	_textureLoader = std::make_unique<TextureLoader>();

	_geometryPass = new ShaderProgram("Geometry Pass");
	_geometryPass->attachShader(ShaderProgram::ShaderType::VertexShader, "assets/shaders/geometryPass.vert")
		.attachShader(ShaderProgram::ShaderType::FragmentShader, "assets/shaders/geometryPass.frag")
		.finalize();

	_initWorld();
}

void Engine::_initializeSDL() {
	_screen = std::make_unique<Window>("Rolling Bogdans, two fast four you!");
}

void Engine::_initializeGL() {
	_renderer = std::make_unique<GLRenderer>(_screen->getView());
}

void Engine::_initWorld() {
	//_testTexture = _textureLoader->loadTexture("assets/textures/wawa.png");
}