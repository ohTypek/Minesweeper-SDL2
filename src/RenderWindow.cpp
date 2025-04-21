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

void RenderWindow::setWindowIcon(const char* path) {
  SDL_Surface* icon = NULL;

  icon = IMG_Load(path);

  if(renderer == NULL)
    std::cout << "Couldn't load icon: " << SDL_GetError() << std::endl;

  SDL_SetWindowIcon(window, icon);

  SDL_FreeSurface(icon);
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

void RenderWindow::render(Field& f, TTF_Font* font) {
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

  if(f.isUncovered && f.getValue() != "0") {
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, f.getSurf());

    src.w = f.getSurf()->w;
    src.h = f.getSurf()->h;
    dst.x -= src.w/2 - dst.w/2 - 2;
    dst.y += 5;
    dst.w = src.w;
    dst.h = src.h;

    SDL_RenderCopy(renderer, tex, &src, &dst);
    SDL_DestroyTexture(tex);
  }

  if(f.isFlagged) {
    this->render("`", font, {0,0,0,0}, Vector2f(dst.x+dst.w/2, dst.y+dst.h/2));
  }
}

void RenderWindow::render(const char* text, TTF_Font* font, SDL_Color color, Vector2f pos) {
  SDL_Surface* textSurf = NULL;
  textSurf = TTF_RenderText_Blended(font, text, color);
  if(textSurf == NULL) std::cout << "Couldn't create font surface: " << SDL_GetError() << std::endl;

  SDL_Texture* texture = NULL;
  texture = SDL_CreateTextureFromSurface(renderer, textSurf);  
  if(texture == NULL) std::cout << "Couldn't create font texture: " << SDL_GetError() << std::endl;

  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = textSurf->w;
  src.h = textSurf->h;

  SDL_Rect dst;
  dst.x = pos.x - textSurf->w/2;
  dst.y = pos.y - textSurf->h/2;
  dst.w = src.w;
  dst.h = src.h;

  SDL_RenderCopy(renderer, texture, &src, &dst);

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(textSurf);
}

void RenderWindow::display() {
  SDL_RenderPresent(renderer);
}