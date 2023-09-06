#include "shader.hpp"

Shader::Shader(const char* vShaderSource, const char* fShaderSource)
{
  this->Compile(vShaderSource, fShaderSource);
}

void Shader::Compile(const char* vShaderSource, const char* fShaderSource)
{
  unsigned int vShader, fShader;

  vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &vShaderSource, NULL);
  glCompileShader(vShader);
  printErrors(vShader, "VERTEX");

  fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &fShaderSource, NULL);
  glCompileShader(fShader);
  printErrors(fShader, "FRAGMENT");

  this->ShaderProgram = glCreateProgram();
  glAttachShader(this->ShaderProgram, vShader);
  glAttachShader(this->ShaderProgram, fShader);
  glLinkProgram(this->ShaderProgram);
  printErrors(this->ShaderProgram, "PROGRAM");

  glDeleteShader(vShader);
  glDeleteShader(fShader);
}


void Shader::Use()
{
  glUseProgram(this->ShaderProgram);
}

void Shader::SetVector3fv(const char* name, glm::vec3 value)
{
  this->Use();
  glUniform3f(glGetUniformLocation(this->ShaderProgram, name), value.x, value.y, value.z);
}

void Shader::SetVector4fv(const char* name, glm::vec4 value)
{
  this->Use();
  glUniform4f(glGetUniformLocation(this->ShaderProgram, name), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4fv(const char* name, glm::mat4 matrix)
{
  this->Use();
  glUniformMatrix4fv(glGetUniformLocation(this->ShaderProgram, name), 1, false, glm::value_ptr(matrix));
}


void Shader::printErrors(unsigned int id, const char* type)
{
  int success;
  char infoLog[1024];

  if(type == "PROGRAM")
  {
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success)
    {
      glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
      std::cout << std::format("SHADER::LINK_ERROR::{}\n{}", type, infoLog) << std::endl;
    }

    return;
  }

  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(id, sizeof(infoLog), NULL, infoLog);
    std::cout << std::format("SHADER::COMPILE_STATUS::{}\n{}", type, infoLog) << std::endl;
  }
}