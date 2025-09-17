#pragma once

#include "GLTools.h"
#include <glm/glm.hpp>
#include <vector>

#include "Block.h"

#define CHUNK_SIZE 16

struct Chunk {
	glm::vec3 position;
	std::vector<Block> blocks;
};

