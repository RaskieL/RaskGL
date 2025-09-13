#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex> vertices, const std::vector<unsigned int> indices) 
	: vertexBuffer(vertices.data(), (unsigned int)(vertices.size() * sizeof(Vertex))),
	indexBuffer(indices.data(), (unsigned int)indices.size())
{
	VertexBufferLayout layout;
	layout.Push<float>(3); // position
	layout.Push<float>(2); // uv
	layout.Push<float>(3); // normals

	vertexArrayObject.AddBuffer(vertexBuffer, indexBuffer, layout);
}
