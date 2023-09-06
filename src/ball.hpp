#ifndef BALL_H
#define BALL_H

#include <glm/glm.hpp>
#include "constants.hpp"
#include "renderer.hpp"
#include "game_object.hpp"
#include <random>

class Ball
{
  public:
    Ball(Renderer *renderer);
    void Update(GameObject playerLeft, GameObject playerRight, float deltaTime);
    void Render();
  private:
    Renderer *renderer;
    glm::vec2 position;
    glm::vec4 color;
    glm::vec2 speed;
    float radius;
    void reset();
};

#endif