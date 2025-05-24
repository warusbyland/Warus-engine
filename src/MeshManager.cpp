#include "ChunkManager.h"

constexpr std::array<glm::vec3, 4> positiveXQuadOffset = 
{
    glm::vec3(0.5f,-0.5f, 0.5f),
    glm::vec3(0.5f,-0.5f,-0.5f),
    glm::vec3(0.5f, 0.5f, 0.5f),
    glm::vec3(0.5f, 0.5f,-0.5f)
};
constexpr std::array<glm::vec3, 4> negativeXQuadOffset = 
{
    glm::vec3(-0.5f, 0.5f, 0.5f),
    glm::vec3(-0.5f, 0.5f,-0.5f),
    glm::vec3(-0.5f,-0.5f, 0.5f),
    glm::vec3(-0.5f,-0.5f,-0.5f)
};
constexpr std::array<glm::vec3, 4> positiveYQuadOffset = 
{
    glm::vec3(-0.5f, 0.5f,-0.5f),
    glm::vec3(-0.5f, 0.5f, 0.5f),
    glm::vec3( 0.5f, 0.5f,-0.5f),
    glm::vec3( 0.5f, 0.5f, 0.5f)
};
constexpr std::array<glm::vec3, 4> negativeYQuadOffset = 
{
    glm::vec3( 0.5f,-0.5f,-0.5f),
    glm::vec3( 0.5f,-0.5f, 0.5f),
    glm::vec3(-0.5f,-0.5f,-0.5f),
    glm::vec3(-0.5f,-0.5f, 0.5f)
};
constexpr std::array<glm::vec3, 4> positiveZQuadOffset = 
{
    glm::vec3( 0.5f, 0.5f, 0.5f),
    glm::vec3(-0.5f, 0.5f, 0.5f),
    glm::vec3( 0.5f,-0.5f, 0.5f),
    glm::vec3(-0.5f,-0.5f, 0.5f) 
};
constexpr std::array<glm::vec3, 4> negativeZQuadOffset = 
{
    glm::vec3( 0.5f,-0.5f,-0.5f),
    glm::vec3(-0.5f,-0.5f,-0.5f),
    glm::vec3( 0.5f, 0.5f,-0.5f),
    glm::vec3(-0.5f, 0.5f,-0.5f)
};

constexpr glm::vec2 uvs[4] = { {0, 1}, {1, 1}, {0, 0}, {1, 0} };
void addQuad(std::vector<Vertex>& vertices, std::vector<GLuint>& indices,
             const glm::vec3& pos, uint8_t normalIndex, 
             const std::array<glm::vec3, 4>& offsets, uint16_t blockId) {
    GLuint startIndex = vertices.size();
    indices.emplace_back(startIndex);
    indices.emplace_back(startIndex + 1);
    indices.emplace_back(startIndex + 2);
    indices.emplace_back(startIndex + 1);
    indices.emplace_back(startIndex + 3);
    indices.emplace_back(startIndex + 2);

    for (int i = 0; i < 4; ++i) {
        vertices.emplace_back(pos + offsets[i], normalIndex, uvs[i], blockId);
    }
}

constexpr int BITBUFFER_SIZE = CHUNK_WIDTH * CHUNK_WIDTH;
void Mesh::buildMesh(SubChunk* px, SubChunk* nx, SubChunk* py, SubChunk* ny, SubChunk* pz, SubChunk* nz) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    vertices.reserve(7500);
    indices.reserve(10000);

    // 1. CREATE MESH VERTICES
    // ------------------------------------------------------------------------

    // Convert block data to binary
    std::array<uint32_t, BITBUFFER_SIZE> xbits = {};
    std::array<uint32_t, BITBUFFER_SIZE> ybits = {};
    std::array<uint32_t, BITBUFFER_SIZE> zbits = {};

    // Add neibour chunks edges
    if (py) {
        std::cout << "py exists\n";
        for (int z = 0; z < CHUNK_WIDTH; ++z) {
            for (int x = 0; x < CHUNK_WIDTH; ++x) {
                if (!py->isAir(x, 0, z)) {
                    ybits[x * CHUNK_WIDTH + z] |= (1u << CHUNK_WIDTH + 1);
                }
            }
        }
    }

    if (ny) {
        for (int z = 0; z < CHUNK_WIDTH; ++z) {
            for (int x = 0; x < CHUNK_WIDTH; ++x) {
                if (!ny->isAir(x, CHUNK_WIDTH - 1, z)) {
                    ybits[x * CHUNK_WIDTH + z] |= 1u;
                }
            }
        }
    }
    
    for (int z = 0; z < CHUNK_WIDTH; z++) {
        int zOffset = z * CHUNK_WIDTH;
        for (int y = 0; y < CHUNK_WIDTH; y++) {
            int yzOffset = zOffset + y;
            for (int x = 0; x < CHUNK_WIDTH; x++) {
                if (!parent->isAir(x, y, z)) {
                    xbits[yzOffset] |= (1u << x);
                    ybits[x * CHUNK_WIDTH + z] |= (1u << (y + 1));
                    zbits[y * CHUNK_WIDTH + x] |= (1u << z);
                }
            }
        }
    }
        
    for (int z = 0; z < CHUNK_WIDTH; z++) {
        for (int y = 0; y < CHUNK_WIDTH; y++) {
            uint32_t xbit = xbits[z * CHUNK_WIDTH + y];
            uint32_t px = ~(xbit >> 1) & xbit;
            uint32_t nx = ~(xbit << 1) & xbit;

            uint32_t ybit = ybits[z * CHUNK_WIDTH + y];
            uint32_t py = ~(ybit >> 1) & ybit;
            uint32_t ny = ~(ybit << 1) & ybit;
            py = (py >> 1) & 0xFFFF;
            ny = (ny >> 1) & 0xFFFF;

            uint32_t zbit = zbits[z * CHUNK_WIDTH + y];
            uint32_t pz = ~(zbit >> 1) & zbit;
            uint32_t nz = ~(zbit << 1) & zbit;
            
            while (px) {
                int x = __builtin_ctz(px);
                uint16_t blockId = parent->getBlockID(x, y, z);
                glm::vec3 blockPos(x, y, z);
                
                addQuad(vertices, indices, blockPos, 0, positiveXQuadOffset, blockId);
                px &= px - 1;
            }
            while (nx) {
                int x = __builtin_ctz(nx);
                uint16_t blockId = parent->getBlockID(x, y, z);
                glm::vec3 blockPos(x, y, z);
                
                addQuad(vertices, indices, blockPos, 1, negativeXQuadOffset, blockId);
                nx &= nx - 1;
            }

            while (py) {
                int x = __builtin_ctz(py);
                uint16_t blockId = parent->getBlockID(z, x, y);
                glm::vec3 blockPos(z, x, y);
                
                addQuad(vertices, indices, blockPos, 2, positiveYQuadOffset, blockId);
                py &= py - 1;
            }
            while (ny) {
                int x = __builtin_ctz(ny);
                uint16_t blockId = parent->getBlockID(z, x, y);
                glm::vec3 blockPos(z, x, y);
                
                addQuad(vertices, indices, blockPos, 3, negativeYQuadOffset, blockId);
                ny &= ny - 1;
            }

            while (pz) {
                int x = __builtin_ctz(pz);
                uint16_t blockId = parent->getBlockID(y, z, x);
                glm::vec3 blockPos(y, z, x);
                
                addQuad(vertices, indices, blockPos, 4, positiveZQuadOffset, blockId);
                pz &= pz - 1;
            }
            while (nz) {
                int x = __builtin_ctz(nz);
                uint16_t blockId = parent->getBlockID(y, z, x);
                glm::vec3 blockPos(y, z, x);
                
                addQuad(vertices, indices, blockPos, 5, negativeZQuadOffset, blockId);
                nz &= nz - 1;
            }
        }
    }

    mesh_size = indices.size();

    // 2. COMPILE
    // ------------------------------------------------------------------------
    VAO0.Bind();

    // std::cout << vertices.size() << "-" << indices.size() << "\n";

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO0(&vertices);
	// Generates Element Buffer Object and links it to indices
	EBO EBO0(&indices);

    // Attribute Pointers
    // Float attribute (position)
    VAO0.LinkAttrib(VBO0, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    // Integer attribute (normalIndex)
    VAO0.LinkAttrib(VBO0, 1, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, normalIndex), true);
    // Float attribute (texCoords)
    VAO0.LinkAttrib(VBO0, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    // Integer attribute (blockTypeID)
    VAO0.LinkAttrib(VBO0, 3, 1, GL_UNSIGNED_SHORT, sizeof(Vertex), (void*)offsetof(Vertex, blockTypeID), true);

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

    glDrawElements(GL_TRIANGLES, mesh_size, GL_UNSIGNED_INT, 0);
}