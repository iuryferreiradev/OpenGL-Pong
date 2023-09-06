#include "ball.hpp"

float map(float value, float start1, float stop1, float start2, float stop2) {
  return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

Ball::Ball(Renderer *renderer)
{
  this->renderer = renderer;
  this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  this->speed = glm::vec2(0.0f, 0.0f);
  this->radius = 8.0f;

  this->reset();
}

void Ball::Render()
{
  this->renderer->DrawCircle(this->position, this->radius, this->color);
}

void Ball::reset()
{
  this->position = glm::vec2(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5);

  // RNG engine
  std::random_device rd; // Hardware entropy
  std::mt19937 mt(rd()); // Mersenne Twister

  // // Distribution 1 - 10
  std::uniform_real_distribution<float> dist(1.0f, 100.0f);

  float randomNumber = dist(mt);

  float angle;
  
  if(randomNumber < 50) // Less than 5 it goes left
    angle = map(randomNumber, 1.0f, 50.0f, glm::radians(225.0f), glm::radians(135.0f));
  else // Else it goes right
    angle = map(randomNumber, 50.0f, 100.0f, glm::radians(-45.0f), glm::radians(45.0f));

  this->speed.x = 200 * cos(angle);
  this->speed.y = 200 * sin(angle);
}


void Ball::Update(GameObject playerLeft, GameObject playerRight, float deltaTime)
{
  if(this->position.x <= 0) // Player Left lost a point
  {
    // Give a point to player right
    if(ResourceManager::PlayerRightScore == 10 || ResourceManager::PlayerLeftScore == 10)
    {
      // Player rihgt won
      this->speed.x = 0;
      this->speed.y = 0;
    }
    else
    {
      ResourceManager::PlayerRightScore++;
      this->reset();
    }
    // Reset the ball
    return;
  }

  if(this->position.x >= SCREEN_WIDTH) // Player Right lost a point
  {
    // Give a point to player left
    if(ResourceManager::PlayerRightScore == 10 || ResourceManager::PlayerLeftScore == 10)
    {
      // Player left won
      this->speed.x = 0;
      this->speed.y = 0;
    }
    else
    {
      ResourceManager::PlayerLeftScore++;
      this->reset();
    }
    return;
  }

  if(ResourceManager::PlayerRightScore == 10 || ResourceManager::PlayerLeftScore == 10)
    return;

  this->position.x += this->speed.x * deltaTime;
  this->position.y += this->speed.y * deltaTime;

  if(this->position.y >= SCREEN_HEIGHT - this->radius) // Screen top collision check
  {
    this->position.y = SCREEN_HEIGHT - this->radius;
    this->speed.y = -this->speed.y;
  }
  else if(this->position.y <= this->radius) // Screen bottom collision check
  {
    this->position.y = this->radius;
    this->speed.y = -this->speed.y;
  }

  if( // Collision between ball and right paddle
      this->position.x + this->radius >= playerRight.Position.x - playerRight.Size.x / 2 &&
      this->position.y + this->radius >= playerRight.Position.y - playerRight.Size.y / 2 &&
      this->position.y - this->radius <= playerRight.Position.y + playerRight.Size.y / 2
    )
  {
    float diff = this->position.y - (playerRight.Position.y - playerRight.Size.y / 2);
    float angle = map(diff, 0, playerRight.Size.y, glm::radians(225.0), glm::radians(135.0));
    
    this->speed.x = 300 * cos(angle);
    this->speed.y = 300 * sin(angle);
  } else if( // Collision between ball and left paddle
      this->position.x - this->radius <= playerLeft.Position.x + playerLeft.Size.x / 2 &&
      this->position.y + this->radius >= playerLeft.Position.y - playerLeft.Size.y / 2 &&
      this->position.y - this->radius <= playerLeft.Position.y + playerLeft.Size.y / 2
    )
  {
    float diff = this->position.y - (playerLeft.Position.y - playerLeft.Size.y / 2);
    float angle = map(diff, 0, playerLeft.Size.y, glm::radians(-45.0), glm::radians(45.0));

    this->speed.x = 300 * cos(angle);
    this->speed.y = 300 * sin(angle);
  }
}