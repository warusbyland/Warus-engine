#include "ChunkManager.h"

void Mesh::incrementIndex() {
    GLuint startIndex = vertices.size();
    indices.push_back(startIndex);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex + 2);
    indices.push_back(startIndex + 3);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex);
}

// Bit representation for a single line slice of SubChunk for culling
constexpr int BREP_SIZE = 18; 

void Mesh::buildMesh(SubChunk* px, SubChunk* nx, SubChunk* py, SubChunk* ny, SubChunk* pz, SubChunk* nz) {
    vertices.clear();
    indices.clear();
    // 1. CREATE MESH VERTICES
    // ------------------------------------------------------------------------
    for (int d = 0; d < 3; d++) {
        for (int axis = 0; axis < BREP_SIZE; axis++) {
            uint32_t bits[BREP_SIZE] = {};
            for (int row = 0; row < BREP_SIZE; row++) {
                for (int col = 0; col < BREP_SIZE; col++) {
                    bits[row] |= !chunk->isAir(axis, row, col) >> col;
                }
            }
        }
    }


    // 2. COMPILE
    // ------------------------------------------------------------------------
    VAO0.Bind();

    // std::cout << vertices.size() << "-" << indices.size() << "\n";

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO0(&vertices);
	// Generates Element Buffer Object and links it to indices
	EBO EBO0(&indices);

    VAO0.LinkAttrib(VBO0, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO0.LinkAttrib(VBO0, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    VAO0.LinkAttrib(VBO0, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    VBO0.Bind();
    glVertexAttribIPointer(3, 1, GL_UNSIGNED_SHORT, sizeof(Vertex), (void*)offsetof(Vertex, blockTypeID));
    glEnableVertexAttribArray(3);
    VBO0.Unbind();
	// Unbind all to prevent accidentally modifying them, ITO
	VAO0.Unbind();
	VBO0.Unbind();
	EBO0.Unbind();
}

void Mesh::draw(Shader& shaderProgram,Camera& camera) {
    shaderProgram.use();
    shaderProgram.setInt("tex0", 0);
    VAO0.Bind();

    // Export data to VS
    camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
    shaderProgram.setMat4("modelMatrix", model);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}