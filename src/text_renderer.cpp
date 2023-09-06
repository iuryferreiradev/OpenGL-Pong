#include "text_renderer.hpp"

void TextRenderer::Init()
{
  this->initData();
}

void TextRenderer::initData()
{
  // Generate arrays
  glGenVertexArrays(1, &this->arrays[0]);
  glGenBuffers(1, &this->arrays[1]);

  glBindVertexArray(this->arrays[0]);
  glBindBuffer(GL_ARRAY_BUFFER, this->arrays[1]);

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, NULL, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Create the text shader
  ResourceManager::LoadShader("text", "assets/shaders/text.vert", "assets/shaders/text.frag");
}

void TextRenderer::Load(const char* fontPath, unsigned int size)
{
  FT_Library ft;
  if(FT_Init_FreeType(&ft))
  {
    std::cout << "ERROR::FREETYPE: Could not initilize FreeType library" << std::endl;
  }

  FT_Face face;
  if(FT_New_Face(ft, fontPath, 0, &face))
  {
    std::cout << "ERROR::FREETYPE: Could not load font" << std::endl;
  }

  FT_Set_Pixel_Sizes(face, 0, size);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // Load the first 128 characters of the ASCII set
  for(unsigned char c = 0; c < 128; c++)
  {
    // Load char glyph into the face
    if(FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      std::cout << "ERROR::FREETYPE: Could not load char" << std::endl;
      continue;
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(
      GL_TEXTURE_2D, 
      0, 
      GL_RED, 
      face->glyph->bitmap.width, 
      face->glyph->bitmap.rows,
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      face->glyph->bitmap.buffer
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Character character = {
      texture,
      glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
      glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
      static_cast<unsigned int>(face->glyph->advance.x)
    };

    this->characters.insert(std::pair<char, Character>(c, character));
  }
  glBindTexture(GL_TEXTURE_2D, 0);

  // Cleanup
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

void TextRenderer::DrawText(std::string text, float x, float y, float scale, glm::vec3 color)
{
  Shader shader = ResourceManager::GetShader("text");

  // glm::vec2 size = this->getSize(text, scale);

  // x -= size.x / 2.0;
  // y -= size.y / 2.0;

  shader.Use();
  shader.SetVector3fv("textColor", color);
  shader.SetMatrix4fv("projection", glm::ortho(0.0, 800.0, 600.0, 0.0));
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(this->arrays[0]);

  std::string::const_iterator c;

  for(c = text.begin(); c != text.end(); c++)
  {
        Character ch = this->characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y + (this->characters['H'].Bearing.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // update VBO for each character
        float vertices[] = {
             xpos,     ypos + h,   0.0f, 1.0f,
             xpos + w, ypos,       1.0f, 0.0f,
             xpos,     ypos,       0.0f, 0.0f,

             xpos,     ypos + h,   0.0f, 1.0f,
             xpos + w, ypos + h,   1.0f, 1.0f,
             xpos + w, ypos,       1.0f, 0.0f
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, this->arrays[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec2 TextRenderer::getSize(std::string text, float scale)
{
  std::string::const_iterator c;
  glm::vec2 total = glm::vec2(0.0f, 0.0f);

  for(c = text.begin(); c != text.end(); c++)
  {
    Character ch = this->characters[*c];
    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;

    total.x += w;

    if(total.y < h)
      total.y = h;
  }

  return total;
}