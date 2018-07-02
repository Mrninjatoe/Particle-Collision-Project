#include "pointlight.hpp"

PointLight::PointLight(glm::vec3 pos, glm::vec3 color) {
	position = pos;
	this->color = color;
	constant = 1.0f;
	linear = 0.14f;
	quadratic = 0.07f;
}

PointLight::PointLight() {
	position = glm::vec3(0);
	color = glm::vec3(1);
	constant = 1.0f;
	linear = 0.14f;
	quadratic = 0.07f;
}