#include<SDL.h>
#include<SDL_image.h>

#include"Math.hpp"
#include"Entity.hpp"

Entity::Entity(SDL_Texture* texture, Vector2f pos, Vector2f size)
: tex(texture), pos(pos), size(size) {
  // If size isn't given set the variable to texture's resolution
  if(size.x == -1 && size.y == -1) {
    SDL_Point p;
    SDL_QueryTexture(this->tex, NULL, NULL, &p.x, &p.y);
    this->size.x = p.x;
    this->size.y = p.y;
  }
}

SDL_Texture* Entity::getTex() {
  return tex;
}

void Entity::changeTex(SDL_Texture* texture) {
  tex = texture;
}

Vector2f Entity::getSize() {
  return size;
}

Vector2f Entity::getPos() {
  return pos;
}

void Entity::setPos(float x, float y) {
  pos.x = x;
  pos.y = y;
} 