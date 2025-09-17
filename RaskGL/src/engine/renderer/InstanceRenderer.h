#pragma once

#include "GLTools.h"
#include "Mesh.h"
#include "Material.h"
#include "InstanceBuffer.h"
#include "Camera.h"
#include "Renderer.h"
#include "Frustrum.h"

class InstanceRenderer
{
public:
	InstanceRenderer(std::shared_ptr<Mesh> m, std::shared_ptr<Material> mat, const std::vector<glm::mat4>& mats = std::vector<glm::mat4>({}));
	void UpdateInstances();
	void Draw(const Camera& camera) const;
	void Init() const;
	void Clear() const;

	std::vector<glm::mat4>& getTransforms() { return transforms; }
	void setTransforms(const std::vector<glm::mat4>& trans);

private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
	std::vector<glm::mat4> transforms;
	unsigned int m_RendererID;

	std::shared_ptr<InstanceBuffer> instanceBuffer;
};

