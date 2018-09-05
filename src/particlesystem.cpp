#include "particlesystem.hpp"
#include <map>
#define NUMBER_OF_EMITTERS 1

ParticleSystem::ParticleSystem(ParticleMethod type) {
	//_nrOfParticles = 1024;
	_nrOfParticles = 65536;
	//_nrOfParticles = 524288;
	std::vector<glm::vec4> positions;
	std::vector<glm::vec4> directions;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec4> velocities;
	for (int i = 0; i < _nrOfParticles; i++) {
		Particle p;
		positions.push_back(p.pos = glm::vec4(_fRand(0, 10), 5, _fRand(0, 10), _fRand(3, 9.5f)));
		directions.push_back(p.dir = glm::vec4(_fRand(-1, 1), -1, _fRand(-1, 1), 0));
		//velocities.push_back(glm::vec4(_fRand(-1, 1), -1, _fRand(-1, 1), p.pos.a));
		velocities.push_back(glm::vec4(0, -1, 0, p.pos.a));
		colors.push_back(p.color = (glm::vec4(1, 0.8431f, 0, 0.025 /*_fRand(0.2f, 0.5f)*/)));
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
	_collisionMethod = type;
}

ParticleSystem::ParticleSystem(ParticleMethod type, std::vector<Mesh::Triangle> triangles, Octree* rootOct) {
	//_nrOfParticles = 1024;
	_nrOfParticles = 65536;
	std::vector<glm::vec4> positions;
	std::vector<glm::vec4> directions;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec4> velocities;
	for (int i = 0; i < _nrOfParticles; i++) {
		Particle p;
		positions.push_back(p.pos = glm::vec4(0, 10, 0, _fRand(5, 10)));
		directions.push_back(p.dir = glm::vec4(_fRand(1, 1), 1, _fRand(1, 1), 0));
				//velocities.push_back(glm::vec4(_fRand(1, 1), 1, _fRand(1, 1), p.pos.a));
		velocities.push_back(glm::vec4(0, 1, 0, p.pos.a));
		colors.push_back(p.color = (glm::vec4(1, 0.8431f, 0, 0.05f /*_fRand(0.2f, 0.5f)*/)));
		_particles.push_back(p);
		
	}
	
	// Setting up particle buffers and triangles.
	_ssbos.push_back(new ShaderStorageBuffer(GL_DYNAMIC_DRAW, positions)); // Positions
	_ssbos.push_back(new ShaderStorageBuffer(GL_DYNAMIC_DRAW, directions)); // Directions
	_ssbos.push_back(new ShaderStorageBuffer(GL_DYNAMIC_DRAW, colors)); // color and radius
	_ssbos.push_back(new ShaderStorageBuffer(GL_DYNAMIC_DRAW, velocities));

	_offsetTexture = std::make_shared<Texture>(Texture::RGBA16f, glm::ivec2(250, 250));

	std::vector<Octree::Node> nodes;
	std::list<Octree*> toProcess;
	std::vector<Mesh::Triangle> trongls;
	toProcess.push_back(rootOct);
	Octree* curr;
	glm::ivec2 currPixel = glm::ivec2(0);
	glm::ivec2 childrenOffset = glm::ivec2(1, 0);
	glm::ivec2 maxOffset = glm::ivec2(250, 250);
	int oneDimensionalOffset = 1;
	glm::vec4 data = glm::vec4(0.f);
	int nodeCounter = 0;
	while (!toProcess.empty()) {
		curr = toProcess.front();
		if (currPixel.x != 0){
			currPixel.y += int(currPixel.x / maxOffset.x);
			currPixel.x = currPixel.x % maxOffset.x;
		}
		if (childrenOffset.x != 0) {
			childrenOffset.y += int(childrenOffset.x / maxOffset.x);
			childrenOffset.x = childrenOffset.x % maxOffset.x;
		}

		Octree::Node octToNode;
		octToNode.info.x = curr->triangleIDs.size();
		octToNode.info.y = curr->trisIndices.size();
		octToNode.info.z = curr->isLeaf;
		octToNode.info.w = curr->depth;
		for (int i = 0; i < curr->triangleIDs.size(); i++) {
			octToNode.triangleRefs[i].x = curr->triangleIDs[i];
		}
		octToNode.region = curr->region;
		nodes.push_back(octToNode);


		//printf("Curr pixel: (%i, %i), child offset: (%i, %i), Number of children: %i, 1D: %i.\nMin: (%f, %f, %f), Max: (%f, %f, %f,)\n\n",
		//	currPixel.x, currPixel.y, childrenOffset.x, 
		//	childrenOffset.y, octToNode.info.y, oneDimensionalOffset, 
		//	curr->region.min.x, curr->region.min.y, curr->region.min.z,
		//	curr->region.max.x, curr->region.max.y, curr->region.max.z);
		
		//if (curr->isLeaf) {
		//	printf("Min: (%f, %f, %f). Max: (%f, %f, %f,)\n", curr->region.min.x, curr->region.min.y, curr->region.min.z,
		//		curr->region.max.x, curr->region.max.y, curr->region.max.z);
		//}
		
		//printf("Node: %i. isLeaf: %i, depth: %i, stored at pixel: (%i, %i) with number of triangles: %i\n", 
		//	nodeCounter, octToNode.info.z, octToNode.info.w, currPixel.x, currPixel.y, octToNode.info.x);


		if (curr->trisIndices.size() > 0) {
			for (int i = 0; i < curr->trisIndices.size(); i++) {
				toProcess.push_back(curr->children[curr->trisIndices[i]]);
			}
			data = glm::vec4(childrenOffset.x, childrenOffset.y, curr->trisIndices.size(), oneDimensionalOffset); // Offset x, y, number of active children, and offset in the one-dimensional array.
			oneDimensionalOffset += curr->trisIndices.size();
			childrenOffset.x += curr->trisIndices.size();
		}
		else {
			data = glm::vec4(0,0,0,0);
			oneDimensionalOffset += 1;
		}
		_offsetTexture->setData(glm::ivec2(1, 1), currPixel, &data);
		currPixel.x += 1;
		toProcess.pop_front();
		nodeCounter++;
	}
	printf("Size of nodes vector: %zu\n,", nodes.size());
	printf("%zu\n", sizeof(Octree::Node) * nodes.size());
	_ssbos.push_back(new ShaderStorageBuffer(GL_STATIC_DRAW, nodes));
	_ssbos.push_back(new ShaderStorageBuffer(GL_STATIC_DRAW, triangles));
	//_ssbos.push_back(new ShaderStorageBuffer(GL_DYNAMIC_DRAW, boxes));
	_collisionMethod = type;
}

ParticleSystem::~ParticleSystem() {
	/*for (auto e : _emitters) {
		glDeleteBuffers(1, &e.vao);
		glDeleteBuffers(1, &e.vbo);
	}*/
}

void ParticleSystem::update(float delta, ShaderProgram* shader) {
	shader->setValue(6, delta);
	//shader->setValue(7, glm::vec3(0, 10, 0));
	shader->setValue(7, glm::vec3(_fRand(-1.8, 1.3), 5, _fRand(-1, 1)));

	switch (_collisionMethod) {
	case Octree3DCollision:
		shader->setValue(18, 0);
		_offsetTexture->bind(0);
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
}

void ParticleSystem::fixOctreeBuffers(Octree* octree) {
		
}
//void ParticleSystem::_setupBuffers() {
	/*for (int i = 0; i < _emitters.size(); i++) {
		glGenVertexArrays(1, &_emitters[i].vao);
		glBindVertexArray(_emitters[i].vao);

		glGenBuffers(1, &_emitters[i].vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _emitters[i].vbo);
		glBufferData(GL_ARRAY_BUFFER, _emitters[i].particles.size() * sizeof(Particle), &_emitters[i].particles[0], GL_STATIC_DRAW);

		_setupAttributes(_emitters[i]);
	}*/
//}

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
