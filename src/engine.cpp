#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <stdio.h>
#include "engine.hpp"
#include <glm/gtx/transform.hpp>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl_gl3.h>
#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX          0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX    0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX  0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX            0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX            0x904B



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
	double deltaTime = 0.0;

	bool quit = false;
	bool octreeVisible = false;
	float timeStep = 1.f;
	float maxStep = 150.f;
	double accumulator = 0.f;
	int counter = 0;
	float dt = 1.0f / 100.f;
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
				if (event.key.keysym.sym == SDLK_o)
					octreeVisible = !octreeVisible;
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
			// Bind FBO for gBuffers output
			_deferredFBO->bind();
			_renderer->render(_screen.get(), _models, _geometryPass);
		}

		{ // Reflection pass
			_reflectionPass->useProgram();
			_reflectionPass->setValue(1, _camera.getReflectedView());
			_reflectionPass->setValue(2, _camera.getProj());
			_reflectedFBO->bind();
			_renderer->render(_screen.get(), _models, _reflectionPass);
		}

		{ // Lighting pass to reconstruct scene.
			_lightingPass->useProgram();
			_lightingPass->setValue(17, _camera.position);
			_lightingPass->setValue(18, _camera.zNear);
			_lightingPass->setValue(19, _camera.zFar);
			_lightingPass->setValue(20, 0);
			_lightingPass->setValue(21, 1);
			_lightingPass->setValue(22, 2);
			_lightingPass->setValue(23, 3);
			_lightingPass->setValue(24, (int)_pointLights.size());
			int shaderPos = 30;
			for (int i = 0; i < _pointLights.size(); i++) {
				_lightingPass->setValue(shaderPos++, _pointLights[i].position);
				_lightingPass->setValue(shaderPos++, _pointLights[i].color);
				_lightingPass->setValue(shaderPos++, _pointLights[i].constant);
				_lightingPass->setValue(shaderPos++, _pointLights[i].linear);
				_lightingPass->setValue(shaderPos++, _pointLights[i].quadratic);
			}
			(*_deferredFBO)[0]->bind(0);
			(*_deferredFBO)[1]->bind(1);
			(*_deferredFBO)[2]->bind(2);
			_deferredFBO->bindDepth(3);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			_renderer->render(_screen.get(), _lightingPass);
		}

		{ // Deciding on collision particle method.
			_computeShader->useProgram();
			switch (_particleSystem->getMethod()) {
			case ParticleSystem::Octree3DCollision: {
				_computeShader->setValue(15, _triangleCount);
				_computeShader->setValue(16, _nrOfNodes);
				_computeShader->setValue(17, 5);
				break;
			}
			case ParticleSystem::ScreeSpaceParticleCollision: {
				_computeShader->setValue(8, _camera.getView());
				_computeShader->setValue(9, _camera.getProj());
				_computeShader->setValue(10, _camera.getReflectedView());
				_computeShader->setValue(20, 0);
				_computeShader->setValue(21, 1);
				_computeShader->setValue(22, 2);
				_computeShader->setValue(23, 3);
				_deferredFBO->bindDepth(0);
				(*_deferredFBO)[1]->bind(1);
				_reflectedFBO->bindDepth(2);
				(*_reflectedFBO)[0]->bind(3);
				break;
			}
			}
		}

		while (accumulator >= dt) {
			accumulator -= dt;
			_particleSystem->update(dt, _computeShader);
			counter++;
		}
		accumulator += deltaTime;

		{ // Particle pass.
			glm::mat4 v = _camera.getView();
			glm::vec3 forward = glm::vec3(v[0][2], v[1][2], v[2][2]);
			auto ssbos = _particleSystem->getSSBuffers();
			_particlePass->useProgram();
			_particlePass->setValue(6, _camera.getView());
			_particlePass->setValue(7, _camera.getProj());
			_particlePass->setValue(21, forward);
			//_particlePass->setValue(22, 0);
			//_testTexture->bind(0);
			for (int i = 0; i < 3; i++) {
				ssbos[i]->bindBase(i);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			_renderer->renderParticles(_screen.get(), _particlePass, _particleSystem->getParticles());
		}

		{	// Skybox
			_skyboxPass->useProgram();
			glm::mat4 v = _camera.getView();
			v[3][0] = 0;
			v[3][1] = 0;
			v[3][2] = 0;
			_skyboxPass->setValue(1, v);
			_skyboxPass->setValue(2, _camera.getProj());
			_skyboxPass->setValue(10, 0);
			//glBindTexture(GL_TEXTURE_CUBE_MAP, _skyboxCubeMap);
			

			_renderSkybox();
			//_renderer->renderSkybox(_screen.get(), _skyboxPass);

		}

		// Uncomment when screen-space.
		{ // Octree renderer.
			if (octreeVisible) {
				_octreePass->useProgram();
				_octreePass->setValue(6, _camera.getView());
				_octreePass->setValue(7, _camera.getProj());
				_renderer->renderOctree(_screen.get(), _octreePass, _octree);
			}
		}

		{
			//GLint total_mem_kb = 0;
			//glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
			//	&total_mem_kb);
			//GLint cur_avail_mem_kb = 0;
			//glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
			//	&cur_avail_mem_kb);
		     
			GLint dedvram = 0;
			GLint tavram = 0;
			GLint cavram = 0;
			GLint eviccount = 0;
			GLint evicmem = 0;
			
			glGetIntegerv(GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &dedvram);
			glGetIntegerv(GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &tavram);
			glGetIntegerv(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &cavram);
			glGetIntegerv(GPU_MEMORY_INFO_EVICTION_COUNT_NVX, &eviccount);
			glGetIntegerv(GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &evicmem);
			
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			//ImGui::Text("VRAM: %d MB", (total_mem_kb - cur_avail_mem_kb) / 1000);
			ImGui::Text("Dedicated vram: %d MB", dedvram / 1000);
			ImGui::Text("Total available vram: %d MB", tavram / 1000);
			ImGui::Text("Current available vram: %d MB", cavram / 1000);
			ImGui::Text("Eviction count: %d MB", eviccount);
			ImGui::Text("Eviction memory: %d MB", evicmem / 1000);
			//static float f = 0.0f;
			//static int counter = 0;
			//ImGui::Text("Hello world!");
			//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
			//ImGui::ColorEdit3("clear_color", (float*)&clear_color);
			//
			//ImGui::Checkbox("Demo window", &show_demo_window);
			//ImGui::Checkbox("Another window", &show_another_window);
			//
			//if (ImGui::Button("Button"))
			//	counter++;
			//ImGui::SameLine();
			//ImGui::Text("counter = %d", counter);

			
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

	_reflectionPass = new ShaderProgram("Reflection Pass");
	_reflectionPass->attachShader(ShaderProgram::ShaderType::VertexShader, "assets/shaders/reflectionPass.vert")
		.attachShader(ShaderProgram::ShaderType::FragmentShader, "assets/shaders/reflectionPass.frag")
		.finalize();
	
	_lightingPass = new ShaderProgram("Lighting Pass");
	_lightingPass->attachShader(ShaderProgram::ShaderType::VertexShader, "assets/shaders/lightingPass.vert")
		.attachShader(ShaderProgram::ShaderType::FragmentShader, "assets/shaders/lightingPass.frag")
		.finalize();

	/*_computeShader = new ShaderProgram("Compute Shader - Updating Particles w/e Octree");
	_computeShader->attachShader(ShaderProgram::ShaderType::ComputeShader, "assets/shaders/particlesOctreeCollision.comp")
		.finalize();*/

	_computeShader = new ShaderProgram("Compute Shader - Updating Particles w/e SSPC");
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

	_skyboxPass = new ShaderProgram("Skybox Pass");
	_skyboxPass->attachShader(ShaderProgram::ShaderType::VertexShader, "assets/shaders/skyboxPass.vert")
		.attachShader(ShaderProgram::ShaderType::FragmentShader, "assets/shaders/skyboxPass.frag")
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
	_testTexture = _textureLoader->loadTexture("Ollie.jpg");
	
	_initSkybox();
	//std::string skyboxFiles[6] = { "right", "left", "top", "bottom", "back", "front" };
	std::vector<std::string> skyboxFiles;
	skyboxFiles.push_back("right.png");
	skyboxFiles.push_back("left.png");
	skyboxFiles.push_back("top.png");
	skyboxFiles.push_back("bottom.jpg");
	skyboxFiles.push_back("back.png");
	skyboxFiles.push_back("front.png");
	_skyboxCubeMap = _textureLoader->loadCubeMap(skyboxFiles);

	_deferredFBO = std::shared_ptr<GLFrameBuffer>(new GLFrameBuffer());
	_deferredFBO->bind()
		.addTexture(0, Texture::TextureFormat::RGB32f, _screen->getWidth(), _screen->getHeight())
		.addTexture(1, Texture::TextureFormat::RGB32f, _screen->getWidth(), _screen->getHeight())
		.addTexture(2, Texture::TextureFormat::RGBA32f, _screen->getWidth(), _screen->getHeight())
		.addDepth(3, _screen->getWidth(), _screen->getHeight())
		.finalize();
	_reflectedFBO = std::shared_ptr<GLFrameBuffer>(new GLFrameBuffer());
	_reflectedFBO->bind()
		.addTexture(0, Texture::TextureFormat::RGBA32f, _screen->getWidth(), _screen->getHeight())
		.addDepth(1, _screen->getWidth(), _screen->getHeight())
		.finalize();

	_models.push_back(_meshLoader->loadMesh("assets/models/bunny.obj", true));
	_models.back().updateModelMatrix(glm::vec3(0, 0, 0), glm::vec3(1.0f));

	_pointLights.push_back(PointLight(glm::vec3(0.25f, 3.0f, 0.25f), glm::vec3(1,1,1)));
	_camera.position = glm::vec3(3, 1.5f, 3);

	/*glm::vec3 min = {100, 100, 100};
	glm::vec3 max = {-100, -100, -100};
	std::vector<Mesh::Triangle> allTriangles;
	int id = 0;
	for (auto model : _models) {
		for (auto mesh : model.meshes) {
			mesh->setMin(model.model * glm::vec4(mesh->getMin(), 1));
			mesh->setMax(model.model * glm::vec4(mesh->getMax(), 1));
			for (int i = 0; i < mesh->getTriangles().size(); i++) {
				Mesh::Triangle temp = mesh->getTriangles()[i].mul(model.model);
				temp.id.x = id;
				allTriangles.push_back(temp);
				id++;
			}
		}
		model.boundingBox.min = model.model * model.boundingBox.min;
		model.boundingBox.max = model.model * model.boundingBox.max;
		min = glm::min(min, glm::vec3(model.boundingBox.min));
		max = glm::max(max, glm::vec3(model.boundingBox.max));
	}
	printf("(%f, %f, %f), (%f, %f, %f)\n", min.x, min.y, min.z,
	max.x, max.y, max.z);

	printf("NUMBER OF MODELS: %zu\n", _models.size());
	_octree = new Octree(Box(min, max), allTriangles, 0, 0);

	_triangleCount = allTriangles.size();
	_particleSystem = new ParticleSystem(ParticleSystem::ParticleMethod::Octree3DCollision, allTriangles, _octree);*/
	

	_particleSystem = new ParticleSystem(ParticleSystem::ParticleMethod::ScreeSpaceParticleCollision);
}

void Engine::_initSkybox() {
	float SIZE = 500.f;
	GLfloat skyboxVertices[] = {
		-SIZE,  SIZE, -SIZE,
		-SIZE, -SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,
		SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,

		-SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,

		SIZE, -SIZE, -SIZE,
		SIZE, -SIZE,  SIZE,
		SIZE,  SIZE,  SIZE,
		SIZE,  SIZE,  SIZE,
		SIZE,  SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,

		-SIZE, -SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		SIZE,  SIZE,  SIZE,
		SIZE,  SIZE,  SIZE,
		SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,

		-SIZE,  SIZE, -SIZE,
		SIZE,  SIZE, -SIZE,
		SIZE,  SIZE,  SIZE,
		SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE, -SIZE,

		-SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		SIZE, -SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		SIZE, -SIZE,  SIZE
	};

	glGenVertexArrays(1, &_skyboxVAO);
	glGenBuffers(1, &_skyboxVBO);
	glBindVertexArray(_skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glBindVertexArray(0);
}

void Engine::_renderSkybox() {
	glViewport(0, 0, _screen->getWidth(), _screen->getHeight());
	glClearColor(0, 0, 0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);

	glBindVertexArray(_skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _skyboxCubeMap);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
}

