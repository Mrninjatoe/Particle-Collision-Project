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
			case SDLK_LALT:
				if (_camera.counter >= 1.f) {
					_camera.enableMouse = !_camera.enableMouse;
					SDL_ShowCursor(_camera.enableMouse);
					_camera.counter = 0.f;
				}
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
		{ // Geometry pass for information.
			_geometryPass->useProgram();
			// Texture.
			_geometryPass->setValue(1, _camera.getView());
			_geometryPass->setValue(2, _camera.getProj());
			_geometryPass->setValue(22, 0);
			_testTexture->bind(0);
			// Bind FBO for gBuffers output
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
		
			_particleSystem->update(deltaTime, _computeShader);
		{ // Particle pass.
			auto ssbos = _particleSystem->getSSBuffers();
			_particlePass->useProgram();
			_particlePass->setValue(6, _camera.getView());
			_particlePass->setValue(7, _camera.getProj());
			_particlePass->setValue(20, 0);
			_testTexture->bind(0);
			for (int i = 0; i < ssbos.size(); i++) {
				//ssbos[i]->bindBase(i);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			_renderer->renderParticles(_screen.get(), _particlePass, _particleSystem->getParticles());
		}

		{ // Octree renderer.
			_octreePass->useProgram();
			_octreePass->setValue(6, _camera.getView());
			_octreePass->setValue(7, _camera.getProj());
			_renderer->renderOctree(_screen.get(), _octreePass, _octree);
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

	_computeShader = new ShaderProgram("Compute Shader - Updating Particles");
	_computeShader->attachShader(ShaderProgram::ShaderType::ComputeShader, "assets/shaders/particlesUpdate.comp")
		.finalize();

	_particlePass = new ShaderProgram("Particle Pass");
	_particlePass->attachShader(ShaderProgram::ShaderType::VertexShader, "assets/shaders/particlePass.vert")
		.attachShader(ShaderProgram::ShaderType::GeometryShader, "assets/shaders/particlePass.geom")
		.attachShader(ShaderProgram::ShaderType::FragmentShader, "assets/shaders/particlePass.frag")
		.finalize();

	_octreePass = new ShaderProgram("Octree Pass");
	_octreePass->attachShader(ShaderProgram::ShaderType::VertexShader, "assets/shaders/octreePass.vert")
		.attachShader(ShaderProgram::ShaderType::FragmentShader, "assets/shaders/octreePass.frag")
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
	_testTexture = _textureLoader->loadTexture("assets/textures/bonparticles.png");
	_deferredFBO = std::shared_ptr<GLFrameBuffer>(new GLFrameBuffer());
	_deferredFBO->bind()
		.addTexture(0, Texture::TextureFormat::RGB32f, _screen->getWidth(), _screen->getHeight())
		.addTexture(1, Texture::TextureFormat::RGB32f, _screen->getWidth(), _screen->getHeight())
		.addTexture(2, Texture::TextureFormat::RGBA32f, _screen->getWidth(), _screen->getHeight())
		.addDepth(3, _screen->getWidth(), _screen->getHeight())
		.finalize();
	_models.push_back(_meshLoader->loadMesh("assets/models/bb8.fbx", true));
	_models.back().updateModelMatrix(glm::vec3(0, 0, 0), glm::vec3(1));
	_models.push_back(_meshLoader->loadMesh("assets/models/bb8.fbx", true));
	_models.back().updateModelMatrix(glm::vec3(2.5, 0, 0), glm::vec3(1));
	_models.push_back(_meshLoader->loadMesh("assets/models/bb8.fbx", true));
	_models.back().updateModelMatrix(glm::vec3(2.5, 0, 2.5), glm::vec3(1));
	_models.push_back(_meshLoader->loadMesh("assets/models/bb8.fbx", true));
	_models.back().updateModelMatrix(glm::vec3(0, 0, 2.5), glm::vec3(1));
	_models.push_back(_meshLoader->loadMesh("assets/models/plane.fbx", true));
	_models.back().updateModelMatrix(glm::vec3(0, 0, 0), glm::vec3(6));
	_particleSystem = new ParticleSystem(ParticleSystem::ParticleMethod::Octree3DCollision);
	_camera.position = glm::vec3(0, 0, 0);

	
	glm::vec3 min = glm::vec3(0);
	glm::vec3 max = glm::vec3(0);
	for (auto model : _models) {
		for (auto mesh : model.meshes) {
			mesh->setMin(model.model * glm::vec4(mesh->getMin(), 1));
			mesh->setMax(model.model * glm::vec4(mesh->getMax(), 1));
		}
		model.boundingBox.min = glm::vec3(model.model * glm::vec4(model.boundingBox.min, 1));
		model.boundingBox.max = glm::vec3(model.model * glm::vec4(model.boundingBox.max, 1));
		min = glm::min(min, model.boundingBox.min);
		max = glm::max(max, model.boundingBox.max);
	}

	std::vector<glm::vec3> centers;
	centers.resize(_models.size());
	//glm::vec3 furCenter;
	//glm::vec3 closeCenter;
	//for (int i = 0; i < _models.size(); i++) {
	//	glm::vec3 dimensions = _models[i].boundingBox.max - _models[i].boundingBox.min;
	//	centers[i] = _models[i].boundingBox.min + (dimensions * 0.5f);
	//	glm::length(centers[i]);
	//}

	printf("NUMBER OF MODELS: %zu\n", _models.size());
	//_octree = new Octree(new Box(glm::vec3(-5, 0, -5), glm::vec3(5, 5, 5)), _models, 0, 0);
	_octree = new Octree(new Box(min, max), _models, 0, 0);
	_octree->getNrOfNodes(_octree, _nrOfNodes);
	printf("%i\n", _nrOfNodes);
}