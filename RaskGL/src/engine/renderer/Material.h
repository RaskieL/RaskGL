#pragma once

#include <memory>

#include "Shader.h"
#include "Texture.h"

class Material
{
public:
	Material(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture)
		: shader(shader), texture(texture) {}

	void Bind() const {
		shader->Bind();
		texture->Bind(0);
		shader->SetUniform1i("u_Texture", 0);

		glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f));
		shader->SetUniform3f("lightDir", lightDir.x, lightDir.y, lightDir.z);
	}

	Shader& getShader() { return *shader;  }
	Texture& getTexture() { return *texture;  }

private:
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Texture> texture;
};

