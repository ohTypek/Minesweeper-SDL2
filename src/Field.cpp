#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>

#include"Math.hpp"
#include"Entity.hpp"
#include"Field.hpp"

Field::Field(int col, int row, TTF_Font* font, SDL_Texture* texture, Vector2f pos, Vector2f size)
 : Entity(texture, pos, size), col(col), row(row), font(font), value(""), surf(nullptr), isFlagged(false), isUncovered(false) {}

int Field::getC() {
  return col;
}

std::string Field::getValue() {
  return value;
}

void Field::setValue(std::string val) {
  value = val;
  if(surf != nullptr) SDL_FreeSurface(surf);

  SDL_Color color = {0,0,0,255};

  if(value == "1")
    color = {0,0,255,255};
  else if(value == "2")
    color = {0,128,0,255};
  else if(value == "3")
    color = {255,0,0,255};
  else if(value == "4")
    color = {0,0,139,255};
  else if(value == "5")
    color = {165,42,42,255};
  else if(value == "6")
    color = {0,255,255,255};
  else if(value == "7")
    color = {0,0,0,255};
  else if(value == "8")
    color = {128,0,128,255};

  surf = TTF_RenderText_Blended(font, val.c_str(), color);
}

SDL_Surface* Field::getSurf() {
  return this->surf;
}

Field::~Field() {
  if(surf != nullptr) SDL_FreeSurface(surf);
}