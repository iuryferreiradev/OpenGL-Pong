#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "shader.hpp"

class ResourceManager
{
  public:
    static std::map<std::string, Shader> Shaders;
    static int PlayerLeftScore;
    static int PlayerRightScore;
    static Shader LoadShader(std::string name, const char* vShaderPath, const char* fShaderPath);
    static Shader GetShader(std::string name);
  private:
    static std::string readFile(const char* path);
};

#endif