#ifndef MANAGESHADER_H
#define MANAGESHADER_H
  
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
  

class Shader
{
public:
    // the program ID
    GLuint ID;
  
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath) {
        // 1. Retrieve the vertex/fragment source code from filePath
        std::string vertexCode, fragmentCode;

        try {
            // Open and read both shader files into strings directly
            std::ifstream vShaderFile(vertexPath), fShaderFile(fragmentPath);
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (const std::ifstream::failure& e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderSource = vertexCode.c_str();
        const char* fShaderSource = fragmentCode.c_str();
        // 2. Compile Program
        unsigned int vertex, fragment;
        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderSource, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderSource, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // Shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // Delete redundant shaders
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    // use/activate the shader
    void use() {
        glUseProgram(ID);
    }
    void Delete() {
        glDeleteProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
  
#endif