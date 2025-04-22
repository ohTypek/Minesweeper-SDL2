#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<iostream>
#include<vector>

#include"Math.hpp"
#include"Utils.hpp"
#include"RenderWindow.hpp"
#include"Entity.hpp"
#include"Field.hpp"
#include"Game.hpp"

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 700;

int ROWS = (int)(WINDOW_WIDTH / 50);
int COLUMNS = (int)((WINDOW_HEIGHT-150) / 50);

bool firstClick = true;
int startTime = 0;
int passed = 0;

struct FieldTextures {
  SDL_Texture* crossed;
  SDL_Texture* clicked;
  SDL_Texture* normal;
};

void buttonClick(Game& g, SDL_Event& event, FieldTextures& texs, TTF_Font* font);
void stateDefault(Game& g, RenderWindow& w, TTF_Font* font);
void stateOne(Game& g, RenderWindow& w, FieldTextures& texs, TTF_Font* font);
void stateThr(Game& g, RenderWindow& w, TTF_Font* sFont, TTF_Font* bFont);

int main(int argc, char* argv[]) {
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not be initialized: " << SDL_GetError() << std::endl;
  } 

  if(!IMG_Init(IMG_INIT_PNG)) {
    std::cout << "IMG_init has failed" << SDL_GetError() << std::endl;
  }

  if(TTF_Init() < 0) {
    std::cout << "TTF_init has failed" << SDL_GetError() << std::endl;
  }

  RenderWindow window("Minesweeper", WINDOW_WIDTH, WINDOW_HEIGHT);
  window.setWindowIcon("res/icons/300x300.png");
  
  TTF_Font* bFont = window.openFont("fonts/counterFont.ttf", 90);
  TTF_Font* sFont = window.openFont("fonts/counterFont.ttf", 40);
  TTF_Font* fieldFont = window.openFont("fonts/fieldFont.ttf", 30);

  FieldTextures fieldTextures;
  fieldTextures.crossed = window.loadTexture("res/gfx/crossedField.png");
  fieldTextures.clicked = window.loadTexture("res/gfx/clickedField.png");
  fieldTextures.normal  = window.loadTexture("res/gfx/field.png");

  SDL_Texture* borderTex = window.loadTexture("res/gfx/border.png");
  SDL_Texture* bgTex = window.loadTexture("res/gfx/fontBg.png");
  SDL_Texture* deadTex = window.loadTexture("res/gfx/dead.png");
  SDL_Texture* smileTex = window.loadTexture("res/gfx/smile.png");

  Entity border(borderTex, Vector2f(0,0));
  Entity bombsText(bgTex, Vector2f(15,30), Vector2f(150, 90));
  Entity timeText(bgTex, Vector2f(635,30), Vector2f(150, 90));
  Entity faceBg(fieldTextures.normal, Vector2f(370,45), Vector2f(60,60));
  Entity face(smileTex, Vector2f(375,50), Vector2f(50,50));

  Game game(30, ROWS, COLUMNS);
  game.createFields(fieldTextures.normal, fieldFont);

  bool gameRunning = true;
  while(gameRunning) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_QUIT:
          gameRunning = false;
          break;
        case SDL_MOUSEBUTTONUP:
          buttonClick(game, event, fieldTextures, fieldFont);
          break;
        default:
          break;
      }
    }

    if(game.state == 0) face.changeTex(smileTex);
    if(game.state == 1) face.changeTex(deadTex);

    window.setRenderColor(185,185,185);
    window.clear();

    window.render(border);
    window.render(bombsText);
    window.render(std::to_string(game.getBombsLeft()).c_str(), bFont, {255,0,0,255}, Vector2f(90, 75));
    window.render(timeText);
    
    std::string time;

    if(game.state == 0) passed = (int)(utils::hireTimeInSeconds()-startTime);

    if(passed == 0 || firstClick) time = "000";
    else if(passed < 10) time = "00" + std::to_string(passed);
    else if(passed < 100) time = "0" + std::to_string(passed);
    else time = std::to_string(passed);

    window.render(time.c_str(), bFont, {255,0,0,255}, Vector2f(710, 75));

    window.render(faceBg);
    window.render(face);

    switch(game.state) {
      case 0:
        stateDefault(game, window, fieldFont);
        break;
      case 1:
        stateOne(game, window, fieldTextures, fieldFont);
        break;
      case 2:
        stateDefault(game, window, fieldFont);
        break;
      case 3:
        stateThr(game, window, sFont, bFont);
        break;
    }

    window.display();
  }

  window.destroy();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  return 0;
}

void buttonClick(Game& g, SDL_Event& event, FieldTextures& texs, TTF_Font* font) {
  int index = g.getFieldID(event.motion.x, event.motion.y-150);

  if(firstClick) { 
    g.fillFields(index); 
    startTime = utils::hireTimeInSeconds();
    firstClick = false; 
  }

  if(g.state == 0) {
    g.revealField(index, event.button.button, texs.normal, texs.clicked);
    g.checkWin(passed);
  }
  else if(g.state == 3 && utils::hireTimeInSeconds()-startTime >= passed+2) {
    g.state = 2;
    for(Field& f : g.getFields()) {
      if(!f.isUncovered) f.isFlagged = true;
    }
  }


  if(event.motion.x > 370 && event.motion.x < 430 
  && event.motion.y > 45  && event.motion.y < 105)
  {
    firstClick = true;
    g.state = 0;
    g.createFields(texs.normal, font);
    passed = 0;
  }
}

void stateOne(Game& g, RenderWindow& w, FieldTextures& texs, TTF_Font* font) {
  for(Field& f : g.getFields()) {
    if(f.isFlagged && f.getValue() != "*") 
      f.changeTex(texs.crossed);
    else if(f.getValue() == "*" && !f.isFlagged) {
      f.isUncovered = true;
      f.changeTex(texs.clicked);
    }
    
    w.render(f, font);
  }
}

void stateDefault(Game& g, RenderWindow& w, TTF_Font* font) {
  for(Field& f : g.getFields())
    w.render(f, font);
}

void stateThr(Game& g, RenderWindow& w, TTF_Font* sFont, TTF_Font* bFont) {
  w.render("Wygrywasz!", bFont, {0,0,0,0}, Vector2f(WINDOW_WIDTH/2, 250));
  w.render(("Twoj czas: " + utils::transformSecondsToClock(passed)).c_str(), bFont, {0,0,0,0}, Vector2f(WINDOW_WIDTH/2, 325));
  w.render(("Rekord: " + utils::transformSecondsToClock(g.getRecord())).c_str(), bFont, {0,0,0,0}, Vector2f(WINDOW_WIDTH/2, 400));
  w.render("Kliknij lewy przycisk myszy", sFont, {0,0,0,0}, Vector2f(WINDOW_WIDTH/2, 620));
  w.render("by obejrzec plansze", sFont, {0,0,0,0}, Vector2f(WINDOW_WIDTH/2, 660));
}