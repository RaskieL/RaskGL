#pragma once

#include "GLTools.h"
#include <glm/glm.hpp>

#define BLOCK_SCALE 1.0f

struct Block
{
	short blockId;
	glm::vec3 chunkPosition;
	glm::mat4 transform;
};