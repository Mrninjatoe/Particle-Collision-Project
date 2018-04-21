#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera {
public:
	Camera();
	~Camera();

	//glm::mat4 getViewMatrix() { 
	//	return glm::lookAt(cameraPos, cameraOrientation * cameraDirection, cameraUp);
	//}


	glm::mat4 getViewMatrix() {
		return view;
	}

	glm::mat4 getProjectionMatrix() {
		return glm::perspective(fov, aspect, zNear, zFar);
	}

	void update();
	glm::vec3 getUpVector() { return cameraUp; }
	glm::vec3 getRightVector() { return cameraRight; }
	glm::vec3 getDirection() { return cameraDirection; }
	void enableFreeCamera();

	glm::vec3 cameraTarget{ 0 }; // Origin atm
	glm::vec3 cameraDirection;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	glm::vec3 cameraPos;
	glm::ivec2 mousePos;
	glm::mat4 view;
	glm::quat cameraOrientation;

	float fov;
	float zNear;
	float zFar;
	float aspect;
	float sensitivity;
	float yaw;
	float pitch;

	bool freeCamera;
private:
	inline void _updateMouseMovement(glm::ivec2& newMousePos);
	inline void _updateView();
};