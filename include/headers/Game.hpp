#pragma once

#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<ctime>
#include<fstream>
#include<vector>

#include"Field.hpp"

class Game {
private:
  int bombs;
  int initBombs;
  int highScore;
public:
  int gameState;
  Game();
  std::vector<Field> createFields(SDL_Texture* tex, TTF_Font* font, int COLUMNS, int ROWS, int WINDOW_HEIGHT, int BOMBS);
  void placeBombs(int bombs, std::vector<Field> &fields);
  void assignValue(std::vector<Field> &fields);
  void flags(bool state);
  void revealField(SDL_Event& event, std::vector<Field> &fields, SDL_Texture* click, SDL_Texture* cross, Game& game);
  void revealBlanks(std::vector<Field> &fields, int index, SDL_Texture* click);
  int getBombAmount();
  std::string getTimeC(int startTime, int currentTime);
  void checkWin(std::vector<Field> &fields, int time);
  int getHighScore();
  void setHighScore(int hg);
  std::string stylizeTime(int time);
  void lost(std::vector<Field> &fields, SDL_Texture* click, SDL_Texture* cross);
};