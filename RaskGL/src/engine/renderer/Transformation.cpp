#include "Transformation.h"

Transformation::Transformation() : translation(0.0f), scale(1.0f), rotationAngles(0.0f) {}

void Transformation::setTranslation(const glm::vec3& translation) {
	Transformation::translation = translation;
}

void Transformation::setRotation(glm::vec3& angles) {
	Transformation::rotationAngles = angles;
}

void Transformation::setScale(const glm::vec3& scale) {
	Transformation::scale = scale;
}

glm::mat4 Transformation::getModelMatrix() const {
	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(modelMatrix, translation);

	modelMatrix = glm::rotate(modelMatrix, rotationAngles.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotationAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotationAngles.z, glm::vec3(0.0f, 0.0f, 1.0f));

	modelMatrix = glm::scale(modelMatrix, scale);
	return modelMatrix;
}