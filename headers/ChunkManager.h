#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <cstdint>
#include <cassert>
#include <glm/glm.hpp>
#include "FastNoiseLite.h"
#include "ManageShader.h"
#include "Camera.h"
#include "MeshManager.h"

class Mesh; // Forward declaration of Mesh
void initializeNoiseObject();

constexpr int CHUNK_WIDTH = 16;
constexpr int CHUNK_HEIGHT = 256;
constexpr int SUBCHUNK_HEIGHT = 16;
constexpr int SUBCHUNK_COUNT = CHUNK_HEIGHT / SUBCHUNK_HEIGHT;

using BlockID = uint16_t; // Real blocks are BlockID, Local blocks are uint16_t
class Block {
public:
    uint16_t id; // Local Palette index, NOT real BlockID
    Block() {}
    Block(int _type)
    : id(_type) {}
};
class BlockPalette {
public:
    std::unordered_map<BlockID, uint16_t> id_to_index;
    std::vector<BlockID> index_to_id;

    uint16_t getOrAdd(BlockID id) {
        auto [it, inserted] = id_to_index.try_emplace(id, static_cast<uint16_t>(index_to_id.size()));
        if (inserted) {
            assert(it->second < UINT16_MAX);
            index_to_id.push_back(id);
        }
        return it->second;
    }

    BlockID getBlockID(uint16_t index) const {
        assert(index < index_to_id.size());
        return index_to_id[index];
    }

    size_t size() const {
        return index_to_id.size();
    }
};
    
class SubChunk {
public:
    static constexpr int TOTAL_BLOCKS = CHUNK_WIDTH * SUBCHUNK_HEIGHT * CHUNK_WIDTH;

    bool dirty = true;
    Mesh mesh;

    SubChunk() : mesh(this) {}

    void initChunk(int x, int y, int z);

    // Access blocks
    inline Block getBlock(int x, int y, int z) { return blocks[index(x, y, z)]; }
    inline uint16_t getLocalBlockID(int x, int y, int z) { return getBlock(x, y, z).id; }
    
    inline void setBlock(int x, int y, int z, BlockID id) { blocks[index(x, y, z)].id = palette.getOrAdd(id); }
    inline BlockID getBlockID(int x, int y, int z) const { return palette.getBlockID(blocks[index(x, y, z)].id); }

    inline bool isAir(int x, int y, int z) { return palette.getBlockID(blocks[(z * CHUNK_WIDTH + y) * CHUNK_WIDTH + x].id) == 0; }
    inline bool isChunkAir() { return palette.size() == 1 && palette.index_to_id[0] == 0; }
    inline bool isChunkSolid() { return palette.size() == 1 && palette.index_to_id[0] == 1; }

    const BlockPalette& getPallete() const { return palette; }
private:
    BlockPalette palette;
    std::array<Block, TOTAL_BLOCKS> blocks;

    // Helper method to convert 3D index to Block index
    inline int index(int x, int y, int z) const { return (z * CHUNK_WIDTH + y) * CHUNK_WIDTH + x; }
};



class Chunk {
public:
    glm::ivec2 positionXZ;

    Chunk() {}
    Chunk(glm::ivec2 pos);
    Chunk(int x, int z);
    
    SubChunk* getSubChunk(int y);
    void buildMesh(int y, SubChunk* px, SubChunk* nx, SubChunk* pz, SubChunk* nz);
    void draw(Shader& shaderProgram,Camera& camera);
private:
    std::array<SubChunk, SUBCHUNK_COUNT> subChunks;
};