#include "particlesystem.hpp"
#include "mesh.hpp"
#define NUMBER_OF_EMITTERS 1

ParticleSystem::ParticleSystem(ParticleMethod type) {
	_nrOfParticles = 1024;
	//_nrOfParticles = 65536;
	std::vector<glm::vec4> positions;
	std::vector<glm::vec4> directions;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec4> velocities;
	for (int i = 0; i < _nrOfParticles; i++) {
		Particle p;
		positions.push_back(p.pos = glm::vec4(0, 10, 0, _fRand(5, 20)));
		directions.push_back(p.dir = glm::vec4(_fRand(-1, 1), -1, _fRand(-1, 1), 0));
		//velocities.push_back(glm::vec4(_fRand(-1, 1), -1, _fRand(-1, 1), p.pos.a));
		velocities.push_back(glm::vec4(0, -1, 0, p.pos.a));
		colors.push_back(p.color = (glm::vec4(1, 0, 0, 1 /*_fRand(0.2f, 0.5f)*/)));
		_particles.push_back(p);
	}

	//_ssbos.push_back(std::make_shared<ShaderStorageBuffer>(_nrOfParticles * sizeof(glm::vec4))); // Positions
	//_ssbos.push_back(std::make_shared<ShaderStorageBuffer>(_nrOfParticles * sizeof(glm::vec4))); // Directions
	//_ssbos.push_back(std::make_shared<ShaderStorageBuffer>(_nrOfParticles * sizeof(glm::vec4))); // color and radius
	//_ssbos[position]->setData(positions);
	//_ssbos[direction]->setData(directions);
	//_ssbos[color]->setData(colors);

	_ssbos.push_back(new ShaderStorageBuffer(GL_DYNAMIC_DRAW, positions)); // Positions
	_ssbos.push_back(new ShaderStorageBuffer(GL_DYNAMIC_DRAW, directions)); // Directions
	_ssbos.push_back(new ShaderStorageBuffer(GL_DYNAMIC_DRAW, colors)); // color and radius
	_ssbos.push_back(new ShaderStorageBuffer(GL_DYNAMIC_DRAW, velocities)); // color and radius
																			//_setupBuffers();
}

ParticleSystem::ParticleSystem(ParticleMethod type, std::vector<Mesh::Triangle> triangles, Octree* rootOct) {
	_nrOfParticles = 1024;
	//_nrOfParticles = 65536;
	std::vector<glm::vec4> positions;
	std::vector<glm::vec4> directions;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec4> velocities;
	for (int i = 0; i < _nrOfParticles; i++) {
		Particle p;
		positions.push_back(p.pos = glm::vec4(0, 10, 0, _fRand(5, 10)));
		directions.push_back(p.dir = glm::vec4(_fRand(-1, 1), -1, _fRand(-1, 1), 0));
		//velocities.push_back(glm::vec4(_fRand(-1, 1), -1, _fRand(-1, 1), p.pos.a));
		velocities.push_back(glm::vec4(0, -1, 0, p.pos.a));
		colors.push_back(p.color = (glm::vec4(1, 0, 0, 0.3f /*_fRand(0.2f, 0.5f)*/)));
		_particles.push_back(p);
	}

	_ssbos.push_back(new ShaderStorageBuffer(GL_DYNAMIC_DRAW, positions)); // Positions
	_ssbos.push_back(new ShaderStorageBuffer(GL_DYNAMIC_DRAW, directions)); // Directions
	_ssbos.push_back(new ShaderStorageBuffer(GL_DYNAMIC_DRAW, colors)); // color and radius
	_ssbos.push_back(new ShaderStorageBuffer(GL_DYNAMIC_DRAW, velocities));
	_ssbos.push_back(new ShaderStorageBuffer(GL_DYNAMIC_DRAW, triangles));
	std::vector<Octree::Node> nodes;
	rootOct->getNodes(rootOct, nodes);
	//std::vector<Box> boxes;
	//for (int i = 0; i < nodes.size(); i++) {
	//	boxes.push_back(nodes[i].);
	//}
	printf("Size of nodes vector: %zu\n,", nodes.size());
	printf("%zu\n", sizeof(Octree::Node) * nodes.size());
	_ssbos.push_back(new ShaderStorageBuffer(GL_DYNAMIC_DRAW, nodes));
	//_ssbos.push_back(new ShaderStorageBuffer(GL_DYNAMIC_DRAW, boxes));
}


ParticleSystem::~ParticleSystem() {
	/*for (auto e : _emitters) {
		glDeleteBuffers(1, &e.vao);
		glDeleteBuffers(1, &e.vbo);
	}*/
}

void ParticleSystem::update(float delta, ShaderProgram* shader) {
	shader->setValue(6, 0.01f);
	shader->setValue(7, glm::vec3(0, 5, 0));
	switch (_collisionMethod) {
	case Octree3DCollision:
		for (int i = 0; i < _ssbos.size(); i++) {
			_ssbos[i]->bindBase(i);
		}
		glDispatchCompute((GLint)_nrOfParticles / 128, 1, 1);
		//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		break;
	case ScreeSpaceParticleCollision:
		for (int i = 0; i < _ssbos.size(); i++) {
			_ssbos[i]->bindBase(i);
		}
		glDispatchCompute((GLint)_nrOfParticles / 128, 1, 1);
		//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		break;
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ParticleSystem::fixOctreeBuffers(Octree* octree) {
		
}

void ParticleSystem::_setupAttributes(Emitter& e) {
	/*glBindVertexArray(e.vao);

	glEnableVertexAttribArray(ParticleBindingLocation::position);
	glVertexAttribPointer(ParticleBindingLocation::position, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)0);

	glEnableVertexAttribArray(ParticleBindingLocation::direction);
	glVertexAttribPointer(ParticleBindingLocation::direction, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)offsetof(Particle, Particle::dir));

	glEnableVertexAttribArray(ParticleBindingLocation::color);
	glVertexAttribPointer(ParticleBindingLocation::color, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)offsetof(Particle, Particle::color));

	glBindVertexArray(0);*/
}
