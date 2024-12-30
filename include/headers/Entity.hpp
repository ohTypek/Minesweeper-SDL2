#pragma once

#include<SDL.h>
#include<SDL_image.h>

#include"Math.hpp"

class Entity {
private:
  SDL_Texture* texture;
  Vector2f pos;
  Vector2f size;
public:
  Entity(Vector2f pos, SDL_Texture* tex, Vector2f size);
  SDL_Texture* getTex();
  Vector2f getPos();
  void setPos(Vector2f npos);
  Vector2f getSize();
};