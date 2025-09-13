#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 trgt, glm::vec3 p_up, float p_fov, float aspct, float nPlane, float fPlane): position(pos), target(trgt), up(p_up), fov(p_fov), aspect(aspct), nearPlane(nPlane), farPlane(fPlane) {}

glm::mat4 Camera::getViewMatrix() const {
	return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
	return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}

void Camera::setPosition(const glm::vec3& pos)
{
	position = pos;
}

void Camera::setTarget(const glm::vec3& trgt)
{
	Camera::target = trgt;
}

void Camera::setAspectRatio(float aspct)
{
	Camera::aspect = aspct;
}

void Camera::setFOV(float p_fov)
{
	Camera::fov = p_fov;
}

void Camera::MoveX(float delta)
{
	glm::vec3 direction = glm::normalize(target - position);
	glm::vec3 right = glm::normalize(glm::cross(direction, up));
	position += right * delta;
	target += right * delta;
}

void Camera::MoveY(float delta)
{
	glm::vec3 direction = glm::normalize(target - position);
	glm::vec3 right = glm::normalize(glm::cross(direction, up));
	glm::vec3 cameraUp = glm::normalize(glm::cross(right, direction));
	position += cameraUp * delta;
	target += cameraUp * delta;
}

void Camera::MoveZ(float delta)
{
	glm::vec3 direction = glm::normalize(target - position);
	position += direction * delta;
	target += direction * delta;
}

void Camera::Rotate(float xoffset, float yoffset)
{
	horizontalAngle -= glm::radians(xoffset);
	verticalAngle += glm::radians(yoffset);
	if (verticalAngle > glm::radians(89.0f))
		verticalAngle = glm::radians(89.0f);
	if (verticalAngle < glm::radians(-89.0f))
		verticalAngle = glm::radians(-89.0f);
	glm::vec3 direction;
	direction.x = cos(verticalAngle) * sin(horizontalAngle);
	direction.y = sin(verticalAngle);
	direction.z = cos(verticalAngle) * cos(horizontalAngle);
	direction = glm::normalize(direction);
	target = position + direction;
}
