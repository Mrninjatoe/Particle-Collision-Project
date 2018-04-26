#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "GLGraphics/glshaderstoragebuffer.hpp"
#include "shaderprogram.hpp"

class ParticleSystem {
public:
	ParticleSystem();
	~ParticleSystem();

	const static enum ParticleBindingLocation : const int {
		position = 0,
		direction = 1,
		color = 2,
	};

	struct Particle {
		glm::vec4 pos;
		glm::vec4 dir;
		glm::vec4 color; // Stores radius in the back of color
	};
	
	struct Emitter {
		GLuint vao;
		GLuint vbo;
		std::vector<Particle> particles;
	};
	
	std::vector<Emitter>& getEmitters() { return _emitters; }
	std::vector<ShaderStorageBuffer*>& getSSBuffers() { return _ssbos; }
	std::vector<Particle>& getParticles() { return _particles; }
	void update(float delta, ShaderProgram* shader);

private:
	std::vector<Emitter> _emitters;
	std::vector<Particle> _particles;
	std::vector<ShaderStorageBuffer*> _ssbos;
	int _nrOfParticles;

	void _setupBuffers();
	void _setupAttributes(Emitter& e);
};