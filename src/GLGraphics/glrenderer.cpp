#include <iostream>
#include "GLGraphics/glrenderer.hpp"
#include "engine.hpp"
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl_gl3.h>

GLRenderer::GLRenderer(SDL_Window* window) {
	// Request an OpenGL 4.6 context (should be core)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	// Also request a depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG
	);

	_context = SDL_GL_CreateContext(window);
	if (_context == NULL)
		printf("SDL failed to create a GL_Context!\n");
	else
		printf("SDL successfully created a GL_Context!\n");

	printf("OpenGL loaded\n");
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplSdlGL3_Init(window);
	ImGui::StyleColorsDark();

	// Enable the debug callback
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openglCallbackFunction, nullptr);
	glDebugMessageControl(
		GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true
	);


	SDL_GL_SetSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDisable(GL_CULL_FACE);

	_fullscreenQuad = Engine::getInstance()->getMeshLoader()->getQuad();
}

GLRenderer::~GLRenderer() {
	SDL_GL_DeleteContext(_context);
}

void GLRenderer::render(Window* window, ShaderProgram* shader) {
	// Refactor render data into a class so it acts like a renderpass with clear color, color/depth buffer bit etc... 
	glViewport(0, 0, window->getWidth(), window->getHeight());
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(_fullscreenQuad->getVAO());
	glDrawElements(GL_TRIANGLES, _fullscreenQuad->getIndices().size(), GL_UNSIGNED_SHORT, nullptr);
	//glDrawElementsInstanced(GL_TRIANGLES, _fullscreenQuad->getIndices().size(), GL_UNSIGNED_INT, NULL, 1);
}

void GLRenderer::render(Window * window, std::vector<Model>& models, ShaderProgram* shader){
	glViewport(0, 0, window->getWidth(), window->getHeight());
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//glBindVertexArray(_fullscreenQuad->getVAO());
	//glDrawElements(GL_TRIANGLES, _fullscreenQuad->getIndices().size(), GL_UNSIGNED_SHORT, nullptr);
	int offset = -3;
	for (auto model : models) {
		for (auto mesh : model.meshes) {
			glBindVertexArray(mesh.getVAO());
			glDrawElements(GL_TRIANGLES, mesh.getIndices().size(), GL_UNSIGNED_SHORT, nullptr);
		}
		offset += 3;
	}
}
