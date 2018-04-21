#include "camera.hpp"
#include "SDL2\SDL.h"
#include "engine.hpp"
#include <algorithm>

Camera::Camera() {
	fov = 70.f;
	zNear = 0.1;
	zFar = 100.f;
	aspect = 4.f / 3.f;
	sensitivity = 0.006f;
	pitch = 0.f;
	yaw = 0.f;
	cameraOrientation = glm::quat();

	cameraTarget = glm::vec3(0);
	cameraRight = glm::vec3(1, 0, 0);
	cameraUp = glm::vec3(0, 1, 0);
	cameraDirection = glm::vec3(0, 0, -1);
	cameraPos = glm::vec3(0);
	mousePos = glm::ivec2(0, 0);

	//cameraDirection = glm::normalize(_getPlayerPos() - cameraTarget);
	//SDL_SetRelativeMouseMode(SDL_TRUE);
	freeCamera = false;
	SDL_ShowCursor(!freeCamera);
}

Camera::~Camera() {
	
}

void Camera::update() {
	if (freeCamera) {
		glm::ivec2 newMousePos;
		SDL_GetMouseState(&newMousePos.x, &newMousePos.y);
		_updateMouseMovement(newMousePos);
	}
	_updateView();
}

void Camera::_updateMouseMovement(glm::ivec2& newMousePos) {
	auto window = Engine::getInstance()->getWindow();
	glm::ivec2 sizes = window->getSizes();
	mousePos = newMousePos;
	newMousePos -= sizes / 2;

	yaw = yaw + sensitivity * newMousePos.x;
	pitch = pitch + sensitivity * newMousePos.y;

	SDL_WarpMouseInWindow(window->getView(), sizes.x / 2, sizes.y / 2);
}

void Camera::_updateView() {
	glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
	cameraOrientation = glm::normalize(qPitch * qYaw);

	view = glm::mat4_cast(cameraOrientation) * glm::translate(glm::mat4(1.f), -cameraPos);
}


void Camera::enableFreeCamera() {
	freeCamera = !freeCamera;
	SDL_ShowCursor(!freeCamera);
}