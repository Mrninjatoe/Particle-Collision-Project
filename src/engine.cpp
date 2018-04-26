#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <stdio.h>
#include "engine.hpp"
#include <glm/gtx/transform.hpp>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl_gl3.h>

Engine* Engine::_instance;

Engine::Engine() {

}

int Engine::run() {
	_init();

	

	

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	
	SDL_Event event;

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;

	bool quit = false;

	while (!quit) {
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = ((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency()) * 0.001;
		//SDL_GL_SwapWindow(_screen->getView());

		while (SDL_PollEvent(&event)) {
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				quit = true;
				break;
			case SDLK_LSHIFT:
				_camera.pressedShift = true;
				break;
			case SDLK_SPACE:
				_camera.moveUp = true;
				break;
			case SDLK_w:
				_camera.moveForward = true;
				break;
			case SDLK_a:
				_camera.moveLeft = true;
				break;
			case SDLK_s:
				_camera.moveBack = true;
				break;
			case SDLK_d:
				_camera.moveRight = true;
				break;
			default:
				break;
			}
			switch (event.button.button) {
			case SDL_BUTTON_RIGHT:
				break;
			default:
				break;
			}
			switch (event.type) {
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_LSHIFT) {
					_camera.pressedShift = false;
				}
				if (event.key.keysym.sym == SDLK_SPACE) {
					_camera.moveUp = false;
				}
				if (event.key.keysym.sym == SDLK_LCTRL) {
					_camera.moveDown = false;
				}
				if (event.key.keysym.sym == SDLK_w) {
					_camera.moveForward = false;
				}
				if (event.key.keysym.sym == SDLK_a) {
					_camera.moveLeft = false;
				}
				if (event.key.keysym.sym == SDLK_s) {
					_camera.moveBack = false;
				}
				if (event.key.keysym.sym == SDLK_d) {
					_camera.moveRight = false;
				}
				break;
			default:
				break;
			}

		
		}
		ImGui_ImplSdlGL3_NewFrame(_screen->getView());
		_camera.update(deltaTime);

		glm::mat4 bog = glm::translate(glm::vec3(0, 0, 0)) * glm::scale(glm::vec3(0.01f));
		{ // Geometry pass for information.
			_geometryPass->useProgram();
			// Texture.
			_geometryPass->setValue(0, bog);
			_geometryPass->setValue(1, _camera.getViewProj());
			_geometryPass->setValue(22, 0);
			_testTexture->bind(0);
			// Bind FBO for gBuffers
			//_renderer->render(_screen.get(), _geometryPass);
			_deferredFBO->bind();
			_renderer->render(_screen.get(), _models, _geometryPass);
		}

		{ // Lighting pass to reconstruct scene.
			_lightingPass->useProgram();
			_lightingPass->setValue(20, 0);
			_lightingPass->setValue(21, 1);
			_lightingPass->setValue(22, 2);
			_lightingPass->setValue(23, 3);
			_lightingPass->setValue(18, _camera.zNear);
			_lightingPass->setValue(19, _camera.zFar);
			(*_deferredFBO)[0]->bind(0);
			(*_deferredFBO)[1]->bind(1);
			(*_deferredFBO)[2]->bind(2);
			_deferredFBO->bindDepth(3);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			_renderer->render(_screen.get(), _lightingPass);
		}

		

		{
			static float f = 0.0f;
			static int counter = 0;
			ImGui::Text("Hello world!");
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
			ImGui::ColorEdit3("clear_color", (float*)&clear_color);

			ImGui::Checkbox("Demo window", &show_demo_window);
			ImGui::Checkbox("Another window", &show_another_window);

			if (ImGui::Button("Button"))
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}


		if (show_another_window) {
			ImGui::Begin("Another window", &show_another_window);
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close"))
				show_another_window = false;
			ImGui::End();
		}

		if (show_demo_window) {
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
			ImGui::ShowDemoWindow(&show_demo_window);
		}

		//glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
		//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		//glClear(GL_COLOR_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		ImGui::Render();
		ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(_screen->getView());
		
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
	
	_lightingPass = new ShaderProgram("Lighting Pass");
	_lightingPass->attachShader(ShaderProgram::ShaderType::VertexShader, "assets/shaders/lightingPass.vert")
		.attachShader(ShaderProgram::ShaderType::FragmentShader, "assets/shaders/lightingPass.frag")
		.finalize();
	
	
	
	_initWorld();
}

void Engine::_initializeSDL() {
	_screen = std::make_unique<Window>("Particle Collision Test");
}

void Engine::_initializeGL() {
	_renderer = std::make_unique<GLRenderer>(_screen->getView());
}

void Engine::_initWorld() {
	_testTexture = _textureLoader->loadTexture("assets/textures/sleeping_pupper.png");
	_deferredFBO = std::make_shared<GLFrameBuffer>();
	_deferredFBO->bind()
		.addTexture(0, Texture::TextureFormat::RGB32f, _screen->getWidth(), _screen->getHeight())
		.addTexture(1, Texture::TextureFormat::RGB32f, _screen->getWidth(), _screen->getHeight())
		.addTexture(2, Texture::TextureFormat::RGBA32f, _screen->getWidth(), _screen->getHeight())
		.addDepth(3, _screen->getWidth(), _screen->getHeight())
		.finalize();
	_models.push_back(_meshLoader->loadMesh("assets/models/bb8.fbx"));
	_camera = Camera();
	_camera.position = glm::vec3(0, 0, -10);
}