#include "Chunk.h"
#include <iostream>


Chunk::Chunk(std::shared_ptr<Mesh> m, std::shared_ptr<Material> mat, const std::vector<glm::mat4>& mats, unsigned int chunkSize, glm::vec3 chunkPosition)
	: mesh(m), material(mat), transforms(mats) {
	this->chunkSize = chunkSize;
	this->chunkPosition = chunkPosition;

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


void Chunk::UpdateInstances() {
	instanceBuffer->Bind();
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, transforms.size() * sizeof(glm::mat4), transforms.data()));
}

void Chunk::Draw(const Camera& camera,const Frustum& frustum) const {
	glm::vec3 center = chunkPosition + glm::vec3((float)chunkSize / 2.0f);
	float radius = glm::sqrt(3.0f * ((float)chunkSize / 2.0f) * ((float)chunkSize / 2.0f));


	if (!SphereInFrustum(frustum, center, radius)) {
		std::cout << "Culled chunk at " << chunkPosition.x << chunkPosition.y << chunkPosition.z << std::endl;
		return;
	}
		

	material->Bind();
	material->getShader().SetUniformMat4f("u_View", camera.getViewMatrix());
	material->getShader().SetUniformMat4f("u_Projection", camera.getProjectionMatrix());

	GLCall(glBindVertexArray(mesh->getVAO().getId()));
	GLCall(glDrawElementsInstanced(GL_TRIANGLES, mesh->getIBO().GetCount(), GL_UNSIGNED_INT, 0, transforms.size()));
	GLCall(glBindVertexArray(0));
}