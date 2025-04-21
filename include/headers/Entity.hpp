#pragma once

#include<SDL.h>
#include<SDL_image.h>
#include"Math.hpp"

class Entity {
private:
  SDL_Texture* tex;
  Vector2f size;
  Vector2f pos;
public:
  Entity(SDL_Texture* texture, Vector2f pos, Vector2f size = Vector2f(-1,-1));
  SDL_Texture* getTex();
  void changeTex(SDL_Texture* texture);
  Vector2f getSize();
  Vector2f getPos();
  void setPos(float x, float y);
};
