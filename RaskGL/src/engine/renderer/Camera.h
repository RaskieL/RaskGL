#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up, float fov, float aspect, float nearPlane, float farPlane);

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

	void setPosition(const glm::vec3& pos);
	void setTarget(const glm::vec3& target);
	void setAspectRatio(float aspect);
	void setFOV(float fov);

	float& getFOV() { return fov; }
	void MoveX(float delta);
	void MoveY(float delta);
	void MoveZ(float delta);
	void Rotate(float xoffset, float yoffset);

private:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
	float fov;
	float aspect;
	float nearPlane;
	float farPlane;

	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
};

