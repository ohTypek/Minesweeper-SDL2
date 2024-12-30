#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>

#include"Math.hpp"
#include"RenderWindow.hpp"
#include"Entity.hpp"

RenderWindow::RenderWindow(const char* title, int w, int h) : window(NULL), renderer(NULL) {
  window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
  if(window == NULL)
    std::cout << "Couldn't create window: " << SDL_GetError() << std::endl;

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(renderer == NULL)
    std::cout << "Couldn't create renderer: " << SDL_GetError() << std::endl;
}

void RenderWindow::setWindowIcon(const char* path) {
  SDL_Surface* icon = NULL;

  icon = IMG_Load(path);

  if(renderer == NULL)
    std::cout << "Couldn't load icon: " << SDL_GetError() << std::endl;

  SDL_SetWindowIcon(window, icon);

  SDL_FreeSurface(icon);
}


SDL_Texture* RenderWindow::loadTexture(const char* filePath) {
  SDL_Texture* tex = NULL;
  tex = IMG_LoadTexture(renderer, filePath);

  if(tex == NULL)
    std::cout << "Couldn't open texture: " << SDL_GetError() << std::endl;

  return tex;
}

TTF_Font* RenderWindow::openFont(const char* filePath, int fontSize) {
  TTF_Font* font = NULL;
  font = TTF_OpenFont(filePath, fontSize);

  if(font == NULL)
    std::cout << "Couldn't open texture: " << SDL_GetError() << std::endl;

  return font;
}

void RenderWindow::destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void RenderWindow::setRenderColor(int r, int g, int b) {
  SDL_SetRenderDrawColor(renderer, r,g,b, 255);
}

void RenderWindow::clear() {
  SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& ent) {
  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = ent.getSize().x;
  src.h = ent.getSize().y;

  SDL_Rect dst;
  dst.x = ent.getPos().x;
  dst.y = ent.getPos().y;
  dst.w = src.w;
  dst.h = src.h;

  SDL_RenderCopy(renderer, ent.getTex(), &src, &dst);
}

void RenderWindow::render(Field& f) {
  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = f.getSize().x;
  src.h = f.getSize().y;

  SDL_Rect dst;
  dst.x = f.getPos().x;
  dst.y = f.getPos().y;
  dst.w = src.w;
  dst.h = src.h;

  SDL_RenderCopy(renderer, f.getTex(), &src, &dst);

  src.w = f.getSurf()->w;
  src.h = f.getSurf()->h;

  dst.x += (f.getSize().x-src.w)/2+2;
  dst.y += (f.getSize().y-src.h)/2;
  dst.w = src.w;
  dst.h = src.h;

  SDL_Texture* tex = NULL;
  tex = SDL_CreateTextureFromSurface(renderer, f.getSurf());

  if(tex == NULL)
    std::cout << "Couldn't create font texture: " << SDL_GetError() << std::endl;

  SDL_RenderCopy(renderer, tex, &src, &dst);
  SDL_DestroyTexture(tex);
  SDL_FreeSurface(f.getSurf());
}

void RenderWindow::render(SDL_Surface* surf, Vector2f pos) {
  SDL_Texture* tex = NULL;
  tex = SDL_CreateTextureFromSurface(renderer, surf);

  if(tex == NULL)
    std::cout << "Couldn't create font texture: " << SDL_GetError() << std::endl;

  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = surf->w;
  src.h = surf->h;

  SDL_Rect dst;
  dst.x = pos.x;
  dst.y = pos.y;
  dst.w = src.w;
  dst.h = src.h;

  SDL_RenderCopy(renderer, tex, &src, &dst);
  SDL_DestroyTexture(tex);
  SDL_FreeSurface(surf);
}

void RenderWindow::display() {
  SDL_RenderPresent(renderer);
}