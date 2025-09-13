#pragma once

#include "Vertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh
{
public:
	Mesh(const std::vector<Vertex> vertices, const std::vector<unsigned int> indices);
	VertexArray& getVAO() { return vertexArrayObject; }
	IndexBuffer& getIBO() { return indexBuffer;  }
	VertexBuffer& getVBO() { return vertexBuffer; }

private:
	VertexArray vertexArrayObject;
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
};

