#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transformation {
public:
	Transformation();

	void setTranslation(const glm::vec3& translation);
	void setScale(const glm::vec3& scale);
	void setRotation(glm::vec3& angles);

	glm::mat4 getModelMatrix() const;

private:
	glm::vec3 translation;
	glm::vec3 scale;
	glm::vec3 rotationAngles;
};