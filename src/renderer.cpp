#include "renderer.hpp"

void Renderer::DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color)
{
  glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position, 0.0f));
  model = glm::scale(model, glm::vec3(size, 0.0f));

  Shader shader = ResourceManager::GetShader("quad");

  shader.Use();
  shader.SetMatrix4fv("projection", projection);
  shader.SetMatrix4fv("model", model);
  shader.SetVector4fv("color", color);

  glBindVertexArray(this->Arrays["quad"][0]);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}

void Renderer::DrawCircle(glm::vec2 position, float radius, glm::vec4 color)
{
  glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position, 0.0f));
  model = glm::scale(model, glm::vec3(radius * 2.0, radius * 2.0, 0.0f));

  Shader shader = ResourceManager::GetShader("quad");

  glBindVertexArray(this->Arrays["circle"][0]);

  shader.SetMatrix4fv("projection", projection);
  shader.SetMatrix4fv("model", model);

  shader.SetVector4fv("color", color);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 64);

  // This draws only the outline
  // shader.SetVector3fv("color", glm::vec3(0.5, 0.3, 0.3));
  // glLineWidth(3.0);
  // glDrawArrays(GL_LINE_LOOP, 0, 128);
}

void Renderer::Init()
{
  this->initQuadData();
  this->initCircleData();
}

void Renderer::initQuadData()
{
  float vertices[] = {
    -0.5f, 0.5f,
    -0.5f, -0.5f,
    0.5f, 0.5f,
    0.5f, -0.5f
  };

  unsigned int indices[] = {
    0, 1, 2,
    1, 2, 3
  };

  unsigned int VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  this->Arrays["quad"][0] = VAO;
  this->Arrays["quad"][1] = VBO;
  this->Arrays["quad"][2] = EBO;
}

void Renderer::initCircleData()
{
  float segments = 64.0f;
  float angle = 360.0f / segments;
  float radius = 0.5f;

  std::vector<float> vertices;

  for(int i = 0; i < segments; i++)
  {
    float x = radius * cos(glm::radians(angle * i));
    float y = radius * sin(glm::radians(angle * i));

    // This changes the origin of the circle to 0.0 instead of the center
    // vertices.push_back(x + 1);
    // vertices.push_back(y + 1);

    vertices.push_back(x);
    vertices.push_back(y);
  }

  unsigned int VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &EBO);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  this->Arrays["circle"][0] = VAO;
  this->Arrays["circle"][1] = VBO;
  this->Arrays["circle"][2] = EBO;
}