#pragma once

#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<string>

#include"Math.hpp"
#include"Entity.hpp"

class Field : public Entity {
private:
  int col, row;
  int index;
  SDL_Texture* tex;
  SDL_Surface* valueSurf;
  char value;
public:
  bool clicked;
  bool flagged;
  Field(Vector2f pos, SDL_Texture* tex, Vector2f size, int c, int r, int index);
  void setTex(SDL_Texture* ntex);
  SDL_Texture* getTex();
  SDL_Surface* getSurf();
  void setSurf(TTF_Font* font);
  char getVal();
  void setVal(char nval);
  int getC();
  int getR();
  int getIndex();
  bool isClicked(int pX, int pY);
};
