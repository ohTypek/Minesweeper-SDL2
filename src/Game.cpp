#include<iostream>
#include<vector>
#include<ctime>

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>

#include"Utils.hpp"
#include"Math.hpp"
#include"Entity.hpp"
#include"Field.hpp"
#include"Game.hpp"

Game::Game(int pBomb, int row, int col) 
 : bombs(pBomb), rows(row), cols(col), state(0), flagged(0) 
{
  std::string temp;
  std::ifstream read("res/saves/hs.txt");
  std::getline(read, temp);
  read.close();

  record = std::stoi(temp);

  if(record < 0) record = INT32_MAX;
}

std::vector<Field>& Game::getFields() {
  return fields;
}

void Game::createFields(SDL_Texture* field, TTF_Font* font) {
  flagged = 0;
  fields.clear();
  for(int y = 0; y<cols; y++) {
    for(int x = 0; x<rows; x++) {
      Field f(x,y, font, field, Vector2f(x*50, y*50+150));
      fields.push_back(f);
    }
  }
}

void Game::fillFields(int ID) {
  flagged = 0;

  // Place bombs
  srand(time(0));
  int neighbours[8] = {
    -1, -rows, -rows-1, -rows+1,
     1,  rows,  rows-1,  rows+1
  };

  int temp = 0;
  while(temp < bombs) {
    int randomID = rand() % (rows*cols);

    bool correctField = true;
    for(int& n : neighbours)
      if(randomID == ID+n || randomID == ID) correctField = false;

    if(!correctField || fields[randomID].getValue() != "") continue;

    fields[randomID].setValue("*");
    temp++;
  }

  // Assign Values
  for(int i = 0; i<rows*cols; i++) {
    if(fields[i].getValue() == "*") continue;
    int touchingBombs = 0;
    for(int& n : neighbours) {
      if(i+n < 0 || i+n >= rows*cols) continue;

      int newC = fields[i+n].getC();
      int oldC = fields[i].getC();

      if((n == -rows-1 || n == -1 || n == rows-1) && newC+1 != oldC) continue;
      if((n == -rows+1 || n == +1 || n == rows+1) && newC-1 != oldC) continue;
      if((n == -rows   || n == rows) && newC != oldC) continue;

      if(fields[i+n].getValue() == "*") touchingBombs++;
    }
    fields[i].setValue(std::to_string(touchingBombs));
  }
}

int Game::getFieldID(float mouseX, float mouseY) {
  int mX = (int)(mouseX/50);
  int mY = (int)(mouseY/50);

  return mY * rows + mX;
}

void Game::revealBlanks(int ID, SDL_Texture* click) {
  int neighbours[8] = {
    -1, -rows, -rows-1, -rows+1,
     1,  rows,  rows-1,  rows+1
  };

  for(int& n : neighbours) {
    if(ID+n >= rows*cols || ID+n < 0) continue;
    if(fields[ID+n].isUncovered) continue;

    int newC = fields[ID+n].getC();
    int oldC = fields[ID].getC();

    if((n == -rows-1 || n == -1 || n == rows-1) && newC+1 != oldC) continue;
    if((n == -rows+1 || n == +1 || n == rows+1) && newC-1 != oldC) continue;
    if((n == -rows   || n == rows) && newC != oldC) continue;

    fields[ID+n].changeTex(click);
    fields[ID+n].isUncovered = true;
    fields[ID+n].isFlagged = false;

    if(fields[ID+n].getValue() == "0") revealBlanks(ID+n, click);
  }
}

void Game::revealField(int ID, int button, SDL_Texture* blank, SDL_Texture* click) {
  Field& workingField = fields[ID];

  if(workingField.isUncovered) return;

  if(button == SDL_BUTTON_LEFT) {
    if(workingField.isFlagged) return;

    workingField.changeTex(click);
    workingField.isUncovered = true;

    if(workingField.getValue() == "*") state = 1;
    if(workingField.getValue() == "0") revealBlanks(ID, click);
  }

  if(button == SDL_BUTTON_RIGHT) {
    if(workingField.isFlagged) {
      workingField.isFlagged = false;
      flagged -= 1;
    }
    else {
      if(flagged < bombs) {
        workingField.isFlagged = true;
        flagged += 1;
      }
    }
  }
}

int Game::getBombsLeft() {
  return bombs - flagged;
}

void Game::checkWin(int time) {
  bool win = true;

  for(Field& f : fields) {
    if(!f.isUncovered && f.getValue() != "*") win = false;
  }

  if(win) state = 3;
  if(time < record && win) record = time;
}

int Game::getRecord() {
  return record;
}

Game::~Game() {
  std::ofstream file("res/saves/hs.txt");
  file << record;
  file.close();
}