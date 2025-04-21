#pragma once

#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>

#include"Math.hpp"
#include"Entity.hpp"
#include"Field.hpp"

class RenderWindow
{
private:
  SDL_Window* window;
  SDL_Renderer* renderer;
public:
  RenderWindow(const char* title, int w, int h);
  SDL_Texture* loadTexture(const char* filePath);
  TTF_Font* openFont(const char* filePath, int fontSize);
  void setWindowIcon(const char* filePath);
  void destroy();
  void setRenderColor(int r, int g, int b);
  void clear();
  void render(Entity& ent);
  void render(Field& f, TTF_Font* font);
  void render(const char* text, TTF_Font* font, SDL_Color color, Vector2f pos);
  void display();
};
