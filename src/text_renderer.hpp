#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <iostream>
#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "resource_manager.hpp"

struct Character {
  unsigned int TextureID; // ID handle of the glyph texture
  glm::ivec2   Size;      // Size of glyph
  glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
  unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class TextRenderer
{
  public:
    TextRenderer(){};
    void Init();
    void DrawText(std::string text, float x, float y, float scale, glm::vec3 color);
    void Load(const char* fontPath, unsigned int size);
    glm::vec2 getSize(std::string text, float scale);
  private:
    void initData();
    std::map<char, Character> characters;
    unsigned int arrays[3];
};

#endif