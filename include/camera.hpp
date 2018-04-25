#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>

class Camera{
public:
	glm::ivec2 mousePos = {0,0};
	glm::vec3 position = {0,0,0};
	glm::quat orientation;

	bool pressedShift = false;
	bool moveUp = false;
	bool moveDown = false;
	bool moveForward = false;
	bool moveBack = false;
	bool moveRight = false;
	bool moveLeft = false;
	
	float zNear = 0.1f;
	float zFar = 50.f;
	float pitch;
	float yaw;

	void update(float dt);

	glm::mat4 getView();
	glm::mat4 getProj();
};