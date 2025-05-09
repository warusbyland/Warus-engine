#pragma once

#include <vector>
#include <iostream> //remove later
#include <cstdint>
#include "ManageShader.h"
#include "BufferObjects.h"
#include "Camera.h"
#include "ChunkManager.h"

class SubChunk; // Forward declaration of SubChunk

struct Quad {
    glm::vec3 pos;
    glm::vec3 size;
    glm::vec3 normal;
    uint16_t type;
};

class Mesh
{
public:
    Mesh(SubChunk* parent) : chunk(parent) {}
    ~Mesh() { VAO0.Delete(); }
    
    inline void setWorldPosition(const glm::vec3& pos) { model = glm::translate(glm::mat4(1.0f), pos); }
    inline void setWorldPosition(int x, int y, int z) { model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)); }

    void buildMesh(SubChunk* px, SubChunk* nx, SubChunk* py, SubChunk* ny, SubChunk* pz, SubChunk* nz);
    void draw(Shader& shaderProgram, Camera& camera);
private:
    SubChunk* chunk;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    glm::mat4 model;

    VAO VAO0;

    void incrementIndex();
};