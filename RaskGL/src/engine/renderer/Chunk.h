#pragma once

#include "GLTools.h"
#include "Mesh.h"
#include "Material.h"
#include "InstanceBuffer.h"
#include "Camera.h"
#include "Renderer.h"
#include "Frustrum.h"

class Chunk
{
public:
	Chunk(std::shared_ptr<Mesh> m, std::shared_ptr<Material> mat, const std::vector<glm::mat4>& mats, unsigned int chunkSize, glm::vec3 chunkPosition);
	void UpdateInstances();
	void Draw(const Camera& camera, const Frustum& frustum) const;

	std::vector<glm::mat4>& getTransforms() { return transforms; }

private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
	std::vector<glm::mat4> transforms;
	unsigned int m_RendererID;
	std::shared_ptr<InstanceBuffer> instanceBuffer;

	unsigned int chunkSize = 16;
	glm::vec3 chunkPosition = glm::vec3(0.0f);
};

