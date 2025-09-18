#pragma once

#include "GLTools.h"
#include <glm/glm.hpp>
#include <vector>

#include "Block.h"

#define CHUNK_SIZE 16

struct ivec3Hash {
	std::size_t operator()(const glm::ivec3& k) const {
		return std::hash<int>()(k.x) ^ std::hash<int>()(k.y << 1) ^ std::hash<int>()(k.z << 2);
	}
};

struct Chunk {
	glm::vec3 position;
	std::unordered_map<glm::ivec3, Block, ivec3Hash> blocks;

	bool HasBlock(const glm::ivec3& localPos) const {
		return blocks.find(localPos) != blocks.end();
	}

    std::vector<glm::mat4> GetVisibleBlocks(
        const std::unordered_map<glm::ivec3, Chunk, ivec3Hash>& world) const
    {
        static const glm::ivec3 directions[6] = {
            { 1, 0, 0 }, {-1, 0, 0},
            { 0, 1, 0 }, { 0,-1, 0},
            { 0, 0, 1 }, { 0, 0,-1}
        };

        std::vector<glm::mat4> visible;

        for (auto& b : blocks) {
            if (b.second.blockId == 0) continue;

            bool hasVisibleFace = false;

            for (glm::ivec3 dir : directions) {
                glm::ivec3 neighborPos = b.first + dir;
                bool neighborSolid = false;

                if (neighborPos.x >= 0 && neighborPos.x < CHUNK_SIZE &&
                    neighborPos.y >= 0 && neighborPos.y < CHUNK_SIZE &&
                    neighborPos.z >= 0 && neighborPos.z < CHUNK_SIZE)
                {
                    auto it = blocks.find(neighborPos);
                    neighborSolid = (it != blocks.end() && it->second.blockId != 0);
                }
                else {
                    glm::ivec3 neighborChunkPos = position;
                    glm::ivec3 neighborLocalPos = neighborPos;

                    if (neighborLocalPos.x < 0) { neighborChunkPos.x++; neighborLocalPos.x -= CHUNK_SIZE; }
                    else if (neighborLocalPos.x >= CHUNK_SIZE) { neighborChunkPos.x--; neighborLocalPos.x += CHUNK_SIZE; } 

                    if (neighborLocalPos.y < 0) { neighborChunkPos.y++; neighborLocalPos.y -= CHUNK_SIZE; }
                    else if (neighborLocalPos.y >= CHUNK_SIZE) { neighborChunkPos.y--; neighborLocalPos.y += CHUNK_SIZE; }

                    if (neighborLocalPos.z < 0) { neighborChunkPos.z++; neighborLocalPos.z -= CHUNK_SIZE; } 
                    else if (neighborLocalPos.z >= CHUNK_SIZE) { neighborChunkPos.z--; neighborLocalPos.z += CHUNK_SIZE; }

                    auto neighborChunkIt = world.find(neighborChunkPos);
                    if (neighborChunkIt != world.end()) {
                        auto& neighborBlocks = neighborChunkIt->second.blocks;
                        auto it = neighborBlocks.find(neighborLocalPos);
                        neighborSolid = true; // (it != neighborBlocks.end() && it->second.blockId != 0);
                    }
                }

                if (!neighborSolid) {
                    hasVisibleFace = true;
                    break;
                }
            }

            if (hasVisibleFace) {
                visible.push_back(b.second.transform);
            }
        }

        return visible;
    }


};

struct VisibleChunk {
	glm::vec3 position;
	std::vector<glm::mat4> blockTransforms;
};