#pragma once
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <memory>
#include "window.hpp"
#include "mesh.hpp"
#include "shaderprogram.hpp"
#include "particlesystem.hpp"

class GLRenderer {
public:
	GLRenderer(SDL_Window* window);
	~GLRenderer();
	void render(Window* window, ShaderProgram* shader);
	void render(Window* window, std::vector<Model>& models, ShaderProgram* shader);
	//void renderParticles(Window* window, ShaderProgram* shader, std::vector<ParticleSystem::Emitter> emitters);
	void renderParticles(Window* window, ShaderProgram* shader, std::vector<ParticleSystem::Particle>& particles);
	static void APIENTRY openglCallbackFunction(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam
	) {
		(void)source; (void)type; (void)id;
		(void)severity; (void)length; (void)userParam;
		fprintf(stderr, "%s\n", message);

		printf("%s, ");
		if (severity == GL_DEBUG_SEVERITY_HIGH) {
			fprintf(stderr, "Aborting...\n");
			//abort();
		}
	}
private:
	SDL_GLContext _context;
	Mesh* _fullscreenQuad;
	GLuint _emptyVAO;
};