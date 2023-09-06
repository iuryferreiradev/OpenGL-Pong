#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "resource_manager.hpp"
#include <map>
#include <vector>
#include <math.h>

class Renderer
{
  public:
    std::map<std::string, unsigned int[3]> Arrays;
    Renderer(){};
    void DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color);
    void DrawCircle(glm::vec2 position, float radius, glm::vec4 color);
    void Init();
  private:
    void initQuadData();
    void initCircleData();
};

#endif