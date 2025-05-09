#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GL/gl.h>
#include <stb\stb_image.h>
#include <unordered_map>
#include <vector>

#include "ManageShader.h"

class Texture
{
public:
    GLuint ID;
    
    Texture(const char* filename, GLenum slot, GLenum format, GLenum pixelType) {
        // 1. Create Texture Object
        // ------------------------------------------------------------------------
        glGenTextures(1, &ID);
        glActiveTexture(slot);
        glBindTexture(GL_TEXTURE_2D, ID);

        // Note - Add Flexibility to these settings below
        // Setting 1. Set minifying and magnifying textures to GL_NEAREST or GL_LINEAR
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // Setting 2. Options when using coords outside of normalised range
        // GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // 2. Load image
        // ------------------------------------------------------------------------
	    int widthImg, heightImg, numColCh;
	    stbi_set_flip_vertically_on_load(true);
	    // Reads the image from a file and stores it in bytes
	    unsigned char* bytes = stbi_load(filename, &widthImg, &heightImg, &numColCh, 0);

        // 3. Connect loaded image -> Object
        // ------------------------------------------------------------------------
        if (bytes) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, format, pixelType, bytes);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "couldn't load da image: " << filename << "\n";
        }
        stbi_image_free(bytes);
    }

    void texUnit(Shader& shader, const char* uniform, GLuint unit) {
        // Location uniform
	    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	    // Changing the uniform of this shader
	    shader.use();
	    // Sets value
	    glUniform1i(texUni, unit);
    }

    void Bind() {glBindTexture(GL_TEXTURE_2D, ID);}
    void UnBind() {glBindTexture(GL_TEXTURE_2D, 0);}
    void Delete() {glDeleteTextures(1, &ID);}
};

class TextureSystem
{
public:
    static constexpr size_t MAX_TEXTURES = 1024;
        
    TextureSystem() {
        // Note - Add Flexibility to these settings below
        // Setting 1. Set minifying and magnifying textures to GL_NEAREST or GL_LINEAR
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    // Setting 2. Options when using coords outside of normalised range
	    // GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	    // float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f}; // When using GL_CLAMP_TO_BORDER
	    // glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, flatColor);
    }
};