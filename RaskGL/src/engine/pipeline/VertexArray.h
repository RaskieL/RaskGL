#pragma once

#include "InstanceBuffer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const IndexBuffer& ib, const VertexBufferLayout& layout);
	void AddInstanceBuffer(const InstanceBuffer& ib, unsigned int index, unsigned int vecCount);

	void Bind() const;
	void Unbind() const;

	unsigned int getId() const { return m_RendererID; }
};