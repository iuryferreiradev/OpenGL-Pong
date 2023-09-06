#include "resource_manager.hpp"

std::map<std::string, Shader> ResourceManager::Shaders;
int ResourceManager::PlayerLeftScore = 0;
int ResourceManager::PlayerRightScore = 0;

Shader ResourceManager::LoadShader(std::string name, const char* vShaderPath, const char* fShaderPath)
{

  std::string vShaderCode = readFile(vShaderPath);
  std::string fShaderCode = readFile(fShaderPath);

  const char* vShaderData = vShaderCode.c_str();
  const char* fShaderData = fShaderCode.c_str();

  Shader shader(vShaderData, fShaderData);

  Shaders[name] = shader;
  return shader;
}

Shader ResourceManager::GetShader(std::string name)
{
  return Shaders[name];
}

std::string ResourceManager::readFile(const char* path)
{
  std::string contents;
  try
  {
    std::filesystem::path filePath(path);
    if(!std::filesystem::exists(filePath) || !std::filesystem::is_regular_file(filePath))
    {
      std::cout << "File does not exist: " << path << std::endl;
      return contents;
    }

    std::ifstream file(filePath);
    if(!file.is_open())
    {
      std::cout << "Could not open file: " << path << std::endl;
      return contents;
    }

    std::ostringstream sstream;

    sstream << file.rdbuf();

    contents = sstream.str();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  return contents;
}