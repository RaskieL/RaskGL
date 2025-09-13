#pragma once

#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "Transformation.h"

class GameObject {
public:
	GameObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
		: mesh(mesh), material(material) {}

	void setTransform(const Transformation& t) { transform = t; }
	Transformation& getTransform() { return transform; }

	void Draw(Renderer& renderer, const Camera& camera){
		material->Bind();
		material->getShader().SetUniformMat4f("u_Model", transform.getModelMatrix());
		material->getShader().SetUniformMat4f("u_View", camera.getViewMatrix());
		material->getShader().SetUniformMat4f("u_Projection", camera.getProjectionMatrix());

		renderer.Draw(mesh->getVAO(), mesh->getIBO(), material->getShader());
	}

private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
	Transformation transform;
};