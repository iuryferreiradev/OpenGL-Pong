#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glm/glm.hpp>
#include "renderer.hpp"

class GameObject
{
  public:
    glm::vec2 Position;
    glm::vec2 Size;
    glm::vec4 Color;
    GameObject(glm::vec2 position, glm::vec2 size, glm::vec4 color);
    void Render(Renderer &renderer);
};

#endif