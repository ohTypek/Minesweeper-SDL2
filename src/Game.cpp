#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<ctime>
#include<fstream>

#include"Field.hpp"
#include"Game.hpp"

Game::Game()
: gameState(0), bombs(0), initBombs(0), highScore(0) {
  std::string temp;

  std::ifstream read("res/saves/hs.txt");
  std::getline(read, temp);
  read.close();

  if(temp == "0" || temp == " " || temp == "")
    highScore = 10000;
  else 
    highScore = stoi(temp);
}

std::vector<Field> Game::createFields(SDL_Texture* tex, TTF_Font* font, int COLUMNS, int ROWS, int WINDOW_HEIGHT, int BOMBS) {
  bombs = 0;
  std::vector<Field> fields;

  int i = 0;
  for(int col = 0; col<COLUMNS; col++) {
    for(int row = 0; row<ROWS; row++) {
      const char* id = (std::to_string(col)+"_"+std::to_string(row)).c_str();
      Field field(Vector2f(50*row,50*col+(WINDOW_HEIGHT-50*COLUMNS)), tex, Vector2f(50,50), col, row, i++);
      fields.push_back(field);
    }
  }

  placeBombs(BOMBS, fields);
  assignValue(fields);

  for(Field& f : fields) {
    f.setSurf(font);
  }

  return fields;
}

void Game::placeBombs(int bombs, std::vector<Field> &fields) {
  this->initBombs = bombs;
  srand(time(0));
  while(this->bombs < bombs) {
    const int index = rand()%177;
    if(fields[index].getVal() != '*') {
      this->bombs++;
      fields[index].setVal('*');
    }
  }
}

void Game::assignValue(std::vector<Field> &fields) {
    for(int i = 0; i<fields.size(); i++) {
      if(fields[i].getVal() == '*')
        continue;

      int b = 0;

      if(i-17>=0 && fields[i-17].getC() == fields[i].getC()-1 && fields[i-17].getVal() == '*')
        b++;
      if(i-16>=0 && fields[i-16].getC() == fields[i].getC()-1 && fields[i-16].getVal() == '*')
        b++;
      if(i-15>=0 && fields[i-15].getC() == fields[i].getC()-1 && fields[i-15].getVal() == '*')
        b++;
      if(i-1>=0  && fields[i-1].getC()  == fields[i].getC()   && fields[i-1].getVal() == '*')
        b++;
      if(i+1<=fields.size()  && fields[i+1].getC()  == fields[i].getC()   && fields[i+1].getVal() == '*')
        b++;
      if(i+15<=fields.size() && fields[i+15].getC() == fields[i].getC()+1 && fields[i+15].getVal() == '*')
        b++;
      if(i+16<=fields.size() && fields[i+16].getC() == fields[i].getC()+1 && fields[i+16].getVal() == '*')
        b++;
      if(i+17<=fields.size() && fields[i+17].getC() == fields[i].getC()+1 && fields[i+17].getVal() == '*')
        b++;

      fields[i].setVal(static_cast<char>('0'+ b));
    }
}

void Game::flags(bool state) {
  if(state)
    bombs--;
  else 
    bombs++;
}

void Game::revealField(SDL_Event& event, std::vector<Field> &fields, SDL_Texture* click, SDL_Texture* cross, Game& game) {
  if(event.button.button == SDL_BUTTON_LEFT) {
    for(Field& f : fields) {
      if(!f.clicked && !f.flagged && f.isClicked(event.motion.x, event.motion.y)) {
        f.setTex(click);
        game.revealBlanks(fields, f.getIndex(), click);
        if(f.getVal() == '*') {
          game.lost(fields, click, cross);
        }
        break;
      }
    }
  } else if(event.button.button == SDL_BUTTON_RIGHT) {
      for(Field& f : fields) {
        if(!f.clicked && f.isClicked(event.motion.x, event.motion.y)) {
          f.clicked = false;
          if(!f.flagged && !f.clicked && game.getBombAmount() > 0) {
            f.flagged = true;
            game.flags(true);
          } else if(!f.clicked && f.flagged) {
            f.flagged = false;
            game.flags(false);
          }
          break;
        }
      }
    }
}

void Game::revealBlanks(std::vector<Field> &fields, int i, SDL_Texture* click) {
  int currentCol = fields[i].getC();

  if(fields[i].getVal() != '0')
    return;

  const int neighbours[8] = {
      -1, -15, -16, -17,
       1,  15,  16,  17
  };

  for(const auto& n : neighbours) {
    if(i+n < 0 || i+n > fields.size()) continue; 

    Field& currentField = fields[i+n];

    if(n < -1 && currentField.getC()+1 != currentCol)
      continue;
    else if(n > 1 && currentField.getC()-1 != currentCol)
      continue;
    else if((n == 1 || n == -1) && currentField.getC() != currentCol)
      continue;

    if(currentField.flagged || currentField.clicked) continue;

    currentField.clicked = true;
    currentField.setTex(click);

    if(currentField.getVal() == '0') {
      revealBlanks(fields, i+n, click);
    }
  }
}

int Game::getBombAmount() {
  return bombs;
}

std::string Game::getTimeC(int startTime, int currentTime) {
  int elapsedTime = currentTime - startTime;
  std::string timeC;

  if(elapsedTime < 10)
    timeC = "00" + std::to_string(currentTime-startTime);
  else if(elapsedTime < 100)
    timeC = "0" + std::to_string(currentTime-startTime);
  else
    timeC = std::to_string(currentTime-startTime);

  return timeC;
}

void Game::checkWin(std::vector<Field> &fields, int time) {
  
  int clickedCount = 0;
  for(Field& f : fields) {
    if(f.clicked && f.getVal() != '*')
      clickedCount++;
  }

  if(clickedCount >= 176-initBombs) {
    setHighScore(time);
    gameState = 2;
  }
}

int Game::getHighScore() {
  return highScore;
}

void Game::setHighScore(int hg) {
  if(hg > highScore)
    return;

  highScore = hg;

  std::ofstream write("res/saves/hs.txt");
  write << highScore;
  write.close();
}

std::string Game::stylizeTime(int time) {
  int m = (time-time%60)/60;
  int s = time%60;

  std::string min = std::to_string(m);
  std::string sec = std::to_string(s);

  if(m < 10)
    min = "0" + min;
  if(s < 10)
    sec = "0" + sec;

  return min + ":" + sec;
}

void Game::lost(std::vector<Field> &fields, SDL_Texture* click, SDL_Texture* cross) {
  for(Field& f : fields) {
    if(f.getVal() == '*' && !f.flagged) {
      f.clicked = true;
      f.setTex(click);
    }
    if(f.getVal() != '*' && f.flagged) {
      f.setTex(cross);
    }
    gameState = 3;
  }
}
