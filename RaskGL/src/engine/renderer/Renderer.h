#pragma once

#include "GLTools.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Camera.h"

class Renderer {
public:
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void DrawArrays(Camera camera, const VertexArray& va, const IndexBuffer& ib, Shader& shader, GLenum drawMode) const;
    void Clear() const;
    void Init() const;
};