#pragma once
#include "window.hpp"
#include "GLGraphics/glrenderer.hpp"
#include "meshloader.hpp"
#include "textureloader.hpp"
#include "shaderprogram.hpp"
#include "GLGraphics/glframebuffer.hpp"
#include "camera.hpp"
#include "particlesystem.hpp"
#include "octree.hpp"
#include "pointlight.hpp"
class Engine {
public:
	Engine();
	~Engine() {
		delete _instance;
	}

	static Engine* getInstance() {
		if (!_instance)
			_instance = new Engine();

		return _instance;
	}

	GLRenderer* getRenderer() { return _renderer.get(); }
	Window* getWindow() { return _screen.get(); }
	MeshLoader* getMeshLoader() { return _meshLoader.get(); }
	TextureLoader* getTextureLoader() { return _textureLoader.get(); }
	int run();
private:
	// Never changing variables
	static Engine* _instance;
	std::unique_ptr<GLRenderer> _renderer;
	std::unique_ptr<Window> _screen;
	std::unique_ptr<MeshLoader> _meshLoader;
	std::unique_ptr<TextureLoader> _textureLoader;
	std::shared_ptr<Texture> _testTexture;
	std::shared_ptr<Texture> _octreeData;
	std::shared_ptr<GLFrameBuffer> _deferredFBO;
	std::shared_ptr<GLFrameBuffer> _reflectedFBO;
	GLuint _skyboxCubeMap;
	Camera _camera;
	std::vector<Model> _models;
	std::vector<PointLight> _pointLights;
	ParticleSystem* _particleSystem;
	ShaderProgram* _geometryPass;
	ShaderProgram* _reflectionPass;
	ShaderProgram* _lightingPass;
	ShaderProgram* _particlePass;
	ShaderProgram* _computeShader;
	ShaderProgram* _octreePass;
	ShaderProgram* _skyboxPass;
	Octree* _octree;
	int _triangleCount = 0;
	int _nrOfNodes = 0;

	GLfloat _skyboxVertices[36];
	GLuint _skyboxVAO, _skyboxVBO;

	void _init();
	void _initializeSDL();
	void _initializeGL();
	void _initWorld();
	void _initSkybox();
	void _renderSkybox();
};