#include "game_object.hpp"

GameObject::GameObject(glm::vec2 position, glm::vec2 size, glm::vec4 color)
{
  this->Position = position;
  this->Size = size;
  this->Color = color;
}

void GameObject::Render(Renderer &renderer)
{
  renderer.DrawQuad(this->Position, this->Size, this->Color);
}