#include<iostream>
#include<SDL.h>

#include"Math.hpp"
#include"Entity.hpp"
#include"Field.hpp"

Field::Field(Vector2f pos, SDL_Texture* tex, Vector2f size, int c, int r, int index) 
: row(r), col(c), value(' '), tex(tex), valueSurf(NULL), clicked(false), index(index), flagged(false), Entity(pos,tex,size) {}

void Field::setTex(SDL_Texture* ntex) {
  this->tex = ntex;
}

SDL_Texture* Field::getTex() {
  return this->tex;
}

SDL_Surface* Field::getSurf() {
  return this->valueSurf;
}

void Field::setSurf(TTF_Font* font) {
  if(flagged) {
    this->valueSurf = TTF_RenderText_Blended(font, "`", {0,0,0,0});
    return;
  }

  if(!clicked || value == '0') {
    this->valueSurf = TTF_RenderText_Blended(font, " ", {0,0,0,0});
    return;
  }

  SDL_Color color = {0,0,0,255};

  if(value == '1')
    color = {0,0,255,255};
  else if(value == '2')
    color = {0,128,0,255};
  else if(value == '3')
    color = {255,0,0,255};
  else if(value == '4')
    color = {0,0,139,255};
  else if(value == '5')
    color = {165,42,42,255};
  else if(value == '6')
    color = {0,255,255,255};
  else if(value == '7')
    color = {0,0,0,255};
  else if(value == '8')
    color = {128,0,128,255};

  std::string str(1, this->value);
  this->valueSurf = TTF_RenderText_Blended(font, str.c_str(), color);
}

void Field::setVal(char nval) {
  this->value = nval;
}

char Field::getVal() {
  return value;
}

int Field::getC() {
  return col;
}

int Field::getR() {
  return row;
}

int Field::getIndex() {
  return index;
}

bool Field::isClicked(int pX, int pY) {
  int x = getPos().x;
  int y = getPos().y;

  if(pX > x && pX < x+getSize().x && pY > y && pY < y+getSize().y) {
    clicked = true;
  }

  return clicked;
}
