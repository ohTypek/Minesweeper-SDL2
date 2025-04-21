#pragma once

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<string>

#include"Math.hpp"
#include"Entity.hpp"

class Field : public Entity {
private:
  int col, row;
  std::string value;
  TTF_Font* font;
  SDL_Surface* surf;
public:
  bool isFlagged;
  bool isUncovered;
  Field(int col, int row, TTF_Font* font, SDL_Texture* texture, Vector2f pos, Vector2f size = Vector2f(-1,-1));
  ~Field();
  int getC();
  std::string getValue();
  void setValue(std::string val);
  SDL_Surface* getSurf();
};