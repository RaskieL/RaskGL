#pragma once

#include "GLTools.h"

class InstanceBuffer {
public:
	InstanceBuffer(const void* data, unsigned int size);
	~InstanceBuffer();
	void Bind() const;
	void Unbind() const;

	unsigned int getId() const { return m_RendererID; }
private:
	unsigned int m_RendererID;
};