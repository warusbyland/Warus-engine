#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    uint8_t normalIndex;
    glm::vec2 texCoords;
    uint16_t blockTypeID;

    Vertex(const glm::vec3& pos, uint8_t aNormalIndex, const glm::vec2& txtCords, uint16_t blockType) : position(pos), normalIndex(aNormalIndex), texCoords(txtCords), blockTypeID(blockType) {}
};

class VBO
{
public:
    GLuint ID;

    // Generate VBO, insert vertices
    VBO(std::vector<Vertex>* vertices) {
        glGenBuffers(1, &ID);
        // Bind as target for glBufferData to insert data
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), vertices->data(), GL_STATIC_DRAW);
    }

    void Bind() {glBindBuffer(GL_ARRAY_BUFFER, ID);}
    void Unbind() {glBindBuffer(GL_ARRAY_BUFFER, 0);}
    void Delete() {glDeleteBuffers(1, &ID);}
};
class EBO
{
public:
    GLuint ID;

    // Generate EBO, insert indices
    EBO(std::vector<GLuint>* indices) {
        glGenBuffers(1, &ID);
        // Bind as target for glBufferData to insert data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(GLuint), indices->data(), GL_STATIC_DRAW);
    }

    void Bind() {glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);}
    void Unbind() {glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);}
    void Delete() {glDeleteBuffers(1, &ID);}
};
class VAO
{
public:
    GLuint ID;

    VAO() {
        glGenVertexArrays(1, &ID);
    }

    void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset, bool isInteger = false)
    {
        VBO.Bind();
        if (isInteger)
            glVertexAttribIPointer(layout, numComponents, type, stride, offset);
        else
            glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
        glEnableVertexAttribArray(layout);
        VBO.Unbind();
    }
    void Bind() {glBindVertexArray(ID);}
    void Unbind() {glBindVertexArray(0);}
    void Delete() {glDeleteVertexArrays(1, &ID);}
};