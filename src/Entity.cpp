#include<SDL.h>
#include<SDL_image.h>

#include"Math.hpp"
#include"Entity.hpp"

Entity::Entity(Vector2f pos, SDL_Texture* tex, Vector2f size = Vector2f(50,50)) 
: pos(pos), texture(tex), size(size) {}

SDL_Texture* Entity::getTex() {
  return texture;
}

Vector2f Entity::getPos() {
  return pos;
}

void Entity::setPos(Vector2f npos) {
  pos = npos;
}

Vector2f Entity::getSize() {
  return size;
}