#include "GLTools.h"
#include "VertexArray.h"
#include "glm/glm.hpp"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const IndexBuffer& ib, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];

		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type,
			element.normalized, layout.GetStride(), (const void*) offset));

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}

	ib.Bind();	
	Unbind();
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::AddInstanceBuffer(const InstanceBuffer& ib, unsigned int index, unsigned int vecCount) {
	Bind();
	ib.Bind();

	GLsizei vec4Size = sizeof(glm::vec4);
	for (unsigned int i = 0; i < vecCount; i++) {
		glEnableVertexAttribArray(index + i);
		glVertexAttribPointer(
			index + i,                 // location in shader (base + offset)
			4, GL_FLOAT, GL_FALSE,
			sizeof(glm::mat4),         // stride = one mat4
			(const void*)(i * vec4Size) // offset for each vec4 column
		);
		glVertexAttribDivisor(index + i, 1); // advance once per instance
	}

	Unbind();
}
