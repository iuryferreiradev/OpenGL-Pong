#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "shader.hpp"
#include "resource_manager.hpp"
#include "renderer.hpp"
#include "game_object.hpp"
#include "text_renderer.hpp"
#include "ball.hpp"
#include "constants.hpp"

void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window, float deltaTime);

GameObject playerLeft(glm::vec2(30.0f, SCREEN_HEIGHT * 0.5), glm::vec2(20.0f, 100.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
GameObject playerRight(glm::vec2(SCREEN_WIDTH - 30.0f, SCREEN_HEIGHT * 0.5), glm::vec2(20.0f, 100.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

int main()
{
  glfwSetErrorCallback(error_callback);
  if(!glfwInit())
  {
    std::cout << "ERROR | GLFW_INIT" << std::endl;
    exit(1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PONG", NULL, NULL);
  if(!window)
  {
    std::cout << "ERROR | GLFW_WINDOW" << std::endl;
    exit(1);
  }

  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "ERROR | GLAD_INIT" << std::endl;
    exit(1);
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Shader shader = ResourceManager::LoadShader("quad", "assets/shaders/default.vert", "assets/shaders/default.frag");
  Renderer renderer;
  renderer.Init();
  TextRenderer textRenderer;
  textRenderer.Load("assets/fonts/OCRAEXT.ttf", 128);
  textRenderer.Init();

  Ball ball(&renderer);

  float lastTime = glfwGetTime();
  float deltaTime = 0.0f;
  float targetFps = 60.0f;

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while(!glfwWindowShouldClose(window))
  {
    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;

    if(targetFps == 0.0 || deltaTime >= 1.0f / targetFps)
    {
      lastTime = currentTime;

      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT);

      process_input(window, deltaTime);

      playerLeft.Render(renderer);
      playerRight.Render(renderer);
      ball.Update(playerLeft, playerRight, deltaTime);
      ball.Render();

      renderer.DrawQuad(glm::vec2(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0), glm::vec2(6.0, SCREEN_HEIGHT), glm::vec4(1.0, 1.0, 1.0, 0.3));
      textRenderer.DrawText(std::to_string(ResourceManager::PlayerLeftScore), SCREEN_WIDTH / 2.0 - 100.0f, 30.0f, 0.5, glm::vec3(1.0, 1.0, 1.0));
      textRenderer.DrawText(std::to_string(ResourceManager::PlayerRightScore), SCREEN_WIDTH / 2.0 + 60.0f, 30.0f, 0.5, glm::vec3(1.0, 1.0, 1.0));

      if(ResourceManager::PlayerLeftScore == 10 || ResourceManager::PlayerRightScore == 10)
      {
        std::string winner = ResourceManager::PlayerLeftScore == 10 ? "Player 1 wins!" : "Player 2 wins!";
        textRenderer.DrawText(winner, 180.0, 180.0, 0.4, glm::vec3(1.0, 1.0, 1.0));
      }

      glfwSwapBuffers(window);
    }
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}


void error_callback(int error, const char* description)
{
  std::cout << "ERROR | " << error << " | " << description << std::endl;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window, float deltaTime)
{
  float speed = 200.0f;

  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && playerLeft.Position.y - playerLeft.Size.y / 2 > 0)
    playerLeft.Position.y -= speed * deltaTime;
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && playerLeft.Position.y + playerLeft.Size.y * 0.5 < SCREEN_HEIGHT)
    playerLeft.Position.y += speed * deltaTime;

  if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && playerRight.Position.y - playerRight.Size.y / 2 > 0)
    playerRight.Position.y -= speed * deltaTime;
  if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && playerRight.Position.y + playerRight.Size.y * 0.5 < SCREEN_HEIGHT)
    playerRight.Position.y += speed * deltaTime;
}