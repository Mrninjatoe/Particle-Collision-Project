#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "GLGraphics/glshaderstoragebuffer.hpp"
#include "shaderprogram.hpp"
#include "octree.hpp"

class ParticleSystem {
public:
	const static enum ParticleMethod : int { Octree3DCollision = 0, ScreeSpaceParticleCollision = 1 };
	ParticleSystem(ParticleMethod type = Octree3DCollision);
	ParticleSystem(ParticleMethod type, std::vector<Mesh::Triangle> triangles, Octree* rootOct);
	~ParticleSystem();

	const static enum ParticleBindingLocation : const int {
		position = 0,
		direction = 1,
		color = 2,
		velocities = 3
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
	void fixOctreeBuffers(Octree* octree);

private:
	std::vector<Emitter> _emitters;
	std::vector<Particle> _particles;
	std::vector<ShaderStorageBuffer*> _ssbos;
	int _nrOfParticles;
	ParticleMethod _collisionMethod;

	void _setupAttributes(Emitter& e);
	inline double _fRand(double fMin, double fMax)
	{
		double f = (double)rand() / RAND_MAX;
		return fMin + f * (fMax - fMin);
	}
};