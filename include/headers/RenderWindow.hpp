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
  void setWindowIcon(const char* path);
  SDL_Texture* loadTexture(const char* filePath);
  TTF_Font* openFont(const char* filePath, int fontSize);
  void destroy();
  void setRenderColor(int r, int g, int b);
  void clear();
  void render(Entity& ent);
  void render(Field& f);
  void render(SDL_Surface* surf, Vector2f pos);
  void display();
};
