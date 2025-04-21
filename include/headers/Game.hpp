#pragma once

#include<iostream>
#include<fstream>
#include<vector>

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>

#include"Utils.hpp"
#include"Math.hpp"
#include"Entity.hpp"
#include"Field.hpp"

class Game {
private:
  int rows, cols;
  int bombs;
  int flagged;
  int record;
  std::vector<Field> fields;
public:
  int state;
  Game(int pBomb, int row, int col);
  std::vector<Field>& getFields();
  void createFields(SDL_Texture* field, TTF_Font* font);
  int getFieldID(float mouseX, float mouseY);
  void fillFields(int ID);
  void revealField(int ID, int button, SDL_Texture* blank, SDL_Texture* click);
  void revealBlanks(int ID, SDL_Texture* click);
  int getBombsLeft();
  void checkWin(int time);
  int getRecord();
  ~Game();
};