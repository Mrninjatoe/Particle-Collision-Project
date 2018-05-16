#include "camera.hpp"
#include "window.hpp"
#include "engine.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

void Camera::update(float dt) {
	// DEBUG FPS CAMERA
	if (!enableMouse){
		glm::ivec2 dm = mousePos;
		SDL_GetMouseState(&mousePos.x, &mousePos.y);
		if (dm != mousePos) {
			auto sizes = Engine::getInstance()->getWindow()->getSizes();
			mousePos -= sizes / 2;

			pitch += dm.y * 0.001f;
			yaw -= dm.x * 0.001f;
			float hpi = glm::half_pi<float>() - 0.001;
			pitch = glm::clamp(pitch, -hpi, hpi);
			SDL_WarpMouseInWindow(Engine::getInstance()->getWindow()->getView(), sizes.x / 2, sizes.y / 2);
		}
	}
	float speed = 350.f;
	if (pressedShift)
		speed *= 10;

	glm::vec3 walk_dir;
	if (moveForward)
		walk_dir += glm::vec3(0, 0, 1);

	if (moveBack)
		walk_dir += glm::vec3(0, 0, -1);

	if (moveLeft)
		walk_dir += glm::vec3(1, 0, 0);

	if (moveRight)
		walk_dir += glm::vec3(-1, 0, 0);

	orientation = glm::quat(glm::vec3(0, yaw, 0)) * glm::quat(glm::vec3(pitch, 0, 0));

	walk_dir = orientation * walk_dir;

	position += walk_dir * dt * speed * 0.001f;
	if (moveUp)
		position.y += speed * dt;

	if (moveDown)
		position.y -= speed * dt;
	
	counter += 1 * dt;
}

glm::mat4 Camera::getView() {
	return glm::inverse(glm::translate(position) * glm::mat4_cast(glm::rotate(orientation, glm::pi<float>(), glm::vec3(0, 1, 0))));
}

glm::mat4 Camera::getReflectedView() {
	glm::mat4 view = getView();
	glm::vec3 forward = glm::vec3(-view[0][2], -view[1][2], -view[2][2]);
	glm::vec3 up = glm::vec3(-view[0][1], view[1][1], -view[2][1]);
	glm::quat temp = glm::quat(glm::vec3(0, yaw + 3.1415, 0)) * glm::quat(glm::vec3(-pitch, 0, 0));
	glm::vec3 pos = position + (5.f * forward);
	return glm::inverse(glm::translate(pos) * glm::mat4_cast(glm::rotate(temp, glm::pi<float>(), glm::vec3(0, 1, 0))));
}

glm::mat4 Camera::getProj() {
	auto size = Engine::getInstance()->getWindow()->getSizes();
	float aspect = size.x / (float)size.y;

	return glm::perspective(glm::radians(70.f), aspect, zNear, zFar);
}