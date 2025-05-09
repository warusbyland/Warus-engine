#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <glm/glm.hpp>

#include "ManageShader.h"
#include "Camera.h"
#include "chunkmanager.h"

struct ChunkPosition { // Demure hash key
    int x, z;

    ChunkPosition(int x, int z) : x(x), z(z) {}

    bool operator==(const ChunkPosition& other) const {
        return x == other.x && z == other.z;
    }
};

namespace std {
    template<>
    struct hash<ChunkPosition> {
        std::size_t operator()(const ChunkPosition& pos) const {
            std::size_t hx = std::hash<int>()(pos.x); // Hash value individually
            std::size_t hz = std::hash<int>()(pos.z);
            return hx ^ (hz << 1); // Combine hashes to avoid collision
        }
    };
}

// Managing Chunks and Creating Meshes
class World {
private:
    // Insert values - cannot overwrite
    //chunks.emplace(
    //    std::piecewise_construct,
    //    std::forward_as_tuple(x, z), // Construct ChunkPosition
    //    std::forward_as_tuple(x, z)  // Construct Chunk
    //);
    // Insert values - can overwrite
    // chunks.insert_or_assign(ChunkPosition(x, z), Chunk(x, z));
    std::unordered_map<ChunkPosition, Chunk> chunks;
public:
    void ensureChunkLoaded(int x, int z);
    void loadChunk(int x, int z);
    Chunk& getOrCreateChunk(int x, int z);
    SubChunk* getSubChunk(int x, int y, int z);

    void compileSubChunkMesh(int x, int y, int z);
    void compileChunk(int x, int z);

    void loadPerimeterRadius(int radius);
    void loadRenderDistance(Camera& camera);
    void render(Shader& shaderProgram,Camera& camera);
};