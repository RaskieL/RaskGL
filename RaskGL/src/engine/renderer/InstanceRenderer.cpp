#include "InstanceRenderer.h"
#include <iostream>


InstanceRenderer::InstanceRenderer(std::shared_ptr<Mesh> mes, std::shared_ptr<Material> mat, const std::vector<glm::mat4>& trans)
	: mesh(mes), material(mat), transforms(trans) {

	instanceBuffer = std::make_shared<InstanceBuffer>(
		transforms.data(),
		transforms.size() * sizeof(glm::mat4)
	);
	m_RendererID = instanceBuffer->getId();
	// Tell VAO about instance buffer
	mesh->getVAO().AddInstanceBuffer(*instanceBuffer, 3, 4);

	std::cout << "VAO: " << mesh->getVAO().getId()
		<< " IBO: " << mesh->getIBO().getId()
		<< " VBO: " << mesh->getVBO().getId()
		<< " IB: " << instanceBuffer->getId() << std::endl;
}


void InstanceRenderer::UpdateInstances() {
	instanceBuffer->Bind();
	if(!transforms.empty()) GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, transforms.size() * sizeof(glm::mat4), transforms.data()));
}

void InstanceRenderer::Draw(const Camera& camera) const {
	material->Bind();
	material->getShader().SetUniformMat4f("u_View", camera.getViewMatrix());
	material->getShader().SetUniformMat4f("u_Projection", camera.getProjectionMatrix());

	GLCall(glBindVertexArray(mesh->getVAO().getId()));
	GLCall(glDrawElementsInstanced(GL_TRIANGLES, mesh->getIBO().GetCount(), GL_UNSIGNED_INT, 0, transforms.size()));
	GLCall(glBindVertexArray(0));
}

void InstanceRenderer::Init() const
{
	/* Texture Shader Blending */
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	/* Enable Depth testing */
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LESS));

	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glCullFace(GL_BACK));
	GLCall(glFrontFace(GL_CCW));
}

void InstanceRenderer::Clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void InstanceRenderer::setTransforms(const std::vector<glm::mat4>& trans){
	transforms = trans;

	instanceBuffer->Bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, transforms.size() * sizeof(glm::mat4),
		transforms.data(), GL_DYNAMIC_DRAW));
}