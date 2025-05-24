#include "ChunkManager.h"

FastNoiseLite noise;

void initializeNoiseObject() {
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetSeed(1337);
    noise.SetFrequency(0.1f);
}

//                               SUBCHUNKS
// ------------------------------------------------------------------------
// Chunk Coords parameters
void SubChunk::initChunk(int cx, int cy, int cz) {
    blocks.fill({0}); //Initialize chunk

    // Calculate base world position
    int cwx = cx * CHUNK_WIDTH, cwy = cy * SUBCHUNK_HEIGHT, cwz = cz * CHUNK_WIDTH;
    mesh.setWorldPosition(cwx, cwy, cwz); // Send to mesh for draw()
    if (cy == 6) {
        palette.getOrAdd(0);
        palette.getOrAdd(1);
        for (int z = 0; z < CHUNK_WIDTH; z++) {
            for (int x = 0; x < CHUNK_WIDTH; x++) {
                int worldX = cwx + x;
                int worldZ = cwz + z;

                float heightValue = noise.GetNoise((float)worldX, (float)worldZ);
                int height = static_cast<int>((heightValue + 1.0f) * 0.5f * CHUNK_WIDTH); //normalize and scale

                for (int y = 0; y <= height; y++) {
                    blocks[index(x, y, z)].id = 1;
                }
            }
        }
    } else {
        if (cy < 6) palette.getOrAdd(1);
        else palette.getOrAdd(0);
        blocks.fill({0});
    }
    std::cout << palette.size();
}


//                                 CHUNKS
// ------------------------------------------------------------------------
Chunk::Chunk(glm::ivec2 pos) : positionXZ(pos) {
    for (int y = 0; y < SUBCHUNK_COUNT; y++) 
        subChunks[y].initChunk(pos.x, y, pos.y);
}
Chunk::Chunk(int x, int z) : positionXZ(glm::ivec2(x, z)) {
    for (int y = 0; y < SUBCHUNK_COUNT; y++) 
        subChunks[y].initChunk(x, y, z);
}

SubChunk* Chunk::getSubChunk(int y) {
    if (y < 0 || y >= SUBCHUNK_COUNT) { return nullptr; }
    return &subChunks[y];
}
void Chunk::buildMesh(int y, SubChunk* px, SubChunk* nx, SubChunk* pz, SubChunk* nz) {
    subChunks[y].mesh.buildMesh(px, nx, getSubChunk(y + 1), getSubChunk(y - 1), pz, nz);
}

void Chunk::draw(Shader& shaderProgram,Camera& camera) { for (SubChunk& chunk : subChunks) chunk.mesh.draw(shaderProgram, camera); }