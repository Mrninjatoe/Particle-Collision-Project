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

	GLRenderer* getRenderer() { return _renderer.get(); };
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
	std::shared_ptr<GLFrameBuffer> _deferredFBO;
	Camera _camera;
	std::vector<Model> _models;
	ParticleSystem* _particleSystem;
	ShaderProgram* _geometryPass;
	ShaderProgram* _lightingPass;
	ShaderProgram* _particlePass;
	ShaderProgram* _computeShader;
	ShaderProgram* _octreePass;
	Octree* _octree;

	void _init();
	void _initializeSDL();
	void _initializeGL();
	void _initWorld();
};