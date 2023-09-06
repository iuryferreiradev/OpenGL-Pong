#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <format>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
  public:
    unsigned int ShaderProgram;
    Shader(){};
    Shader(const char* vShaderSource, const char* fShaderSource);
    void Compile(const char* vShaderSource, const char* fShaderSource);
    void Use();
    void SetVector3fv(const char* name, glm::vec3 value);
    void SetVector4fv(const char* name, glm::vec4 value);
    void SetMatrix4fv(const char* name, glm::mat4 matrix);
  private:
    void printErrors(unsigned int id, const char* type);
};

#endif