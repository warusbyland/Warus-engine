#include "World.h"

// HELPER FUNCTIONS
// ------------------------------------------------------------------------

// CHUNKS FUNCTIONS
// ------------------------------------------------------------------------
// Load or generate chunk
void World::ensureChunkLoaded(int x, int z) {
    if (chunks.count(ChunkPosition(x, z)) == 0) {
        chunks.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(x, z), // Construct ChunkPosition
            std::forward_as_tuple(x, z)  // Construct Chunk
        );
    }
}
// Load or generate chunk - Not checking existence of chunk - used for start-up
void World::loadChunk(int x, int z) {
    chunks.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(x, z), // Construct ChunkPosition
        std::forward_as_tuple(x, z)  // Construct Chunk
    );
}
// Gets a reference to the chunk (loads if not present)
Chunk& World::getOrCreateChunk(int x, int z) {
    ChunkPosition pos = {x, z};
    auto it = chunks.find(pos);
    if (it != chunks.end()) return it->second;

    // Generate new chunk
    Chunk newChunk(x, z);
    chunks[pos] = std::move(newChunk);
    return chunks[pos];
}
// Returns a pointer of SubChunk
SubChunk* World::getSubChunk(int x, int y, int z) { return chunks[ChunkPosition(x, z)].getSubChunk(y); }

// MESH FUNCTIONS
// ------------------------------------------------------------------------
void World::compileSubChunkMesh(int x, int y, int z) { 
    chunks[ChunkPosition(x, z)].buildMesh( y, 
        getSubChunk(x+1,y,z),
        getSubChunk(x-1,y,z),
        getSubChunk(x,y,z+1),
        getSubChunk(x,y,z-1)
    );
}
void World::compileChunk(int x, int z) {  
    for (int y = 0; y < SUBCHUNK_COUNT; y++) {
        compileSubChunkMesh(x, y, z);
    }
}

// WORLD FUNCTIONS
// ------------------------------------------------------------------------
void World::loadPerimeterRadius(int radius) {
    for (int dx = -radius; dx <= radius; dx++) {
        for (int dz = -radius; dz <= radius; dz++) {
            loadChunk(dx, dz);
        }
    }
    for (int dx = -radius; dx <= radius; dx++) {
        for (int dz = -radius; dz <= radius; dz++) {
            compileChunk(dx, dz);
        }
    }
}

void World::loadRenderDistance(Camera& camera) {
    int playerChunk_x = static_cast<int>(std::floor(camera.Position.x / CHUNK_WIDTH));
    int playerChunk_z = static_cast<int>(std::floor(camera.Position.z / CHUNK_WIDTH));
    for (int dx = -1; dx <= 1; dx++) {
        for (int dz = -1; dz <= 1; dz++) {
            ensureChunkLoaded(playerChunk_x + dx, playerChunk_z + dz);
            compileChunk(dx, dz);
        }
    }
}

// Render chunks within frustrum
void World::render(Shader& shaderProgram, Camera& camera) {
    for (auto& [_, chunk] : chunks) {
        chunk.draw(shaderProgram, camera);
    }
}