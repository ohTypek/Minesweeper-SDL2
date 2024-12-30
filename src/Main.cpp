#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include<vector>

#include"Math.hpp"
#include"RenderWindow.hpp"
#include"Entity.hpp"
#include"Field.hpp"
#include"Game.hpp"
#include"Utils.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 700;

const int COLUMNS = 11;
const int ROWS = 16;

const int BOMBS = 35;

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

  TTF_Font* bFont = window.openFont("fonts/counterFont.ttf", 80);
  TTF_Font* sFont = window.openFont("fonts/counterFont.ttf", 40);
  TTF_Font* fieldFont = window.openFont("fonts/fieldFont.ttf", 30);

  SDL_Surface* bombs = TTF_RenderText_Blended(bFont, "00", {255,0,0,255});
  SDL_Surface* time = TTF_RenderText_Blended(bFont, "000", {255,0,0,255});

  SDL_Texture* fieldTex = window.loadTexture("res/gfx/field.png");
  SDL_Texture* clickedFieldTex = window.loadTexture("res/gfx/clickedField.png");
  SDL_Texture* crossedField = window.loadTexture("res/gfx/crossedField.png");
  SDL_Texture* smileTex = window.loadTexture("res/gfx/smile.png");
  SDL_Texture* deadTex = window.loadTexture("res/gfx/dead.png");
  SDL_Texture* fontBg = window.loadTexture("res/gfx/fontBg.png");
  SDL_Texture* borderTex = window.loadTexture("res/gfx/border.png");

  Entity bombsBg(Vector2f(25, (WINDOW_HEIGHT-50*COLUMNS-bombs->h-5)/2), fontBg, Vector2f(bombs->w+20, bombs->h+5));
  Entity timeBg(Vector2f(WINDOW_WIDTH-45-time->w, (WINDOW_HEIGHT-50*COLUMNS-time->h-5)/2), fontBg, Vector2f(time->w+20, time->h+5));
  Entity smileBg(Vector2f((WINDOW_WIDTH-60)/2, (WINDOW_HEIGHT-50*COLUMNS-60)/2), fieldTex, Vector2f(60,60));
  Entity smile(Vector2f((WINDOW_WIDTH-50)/2, (WINDOW_HEIGHT-50*COLUMNS-50)/2), smileTex, Vector2f(50,50));
  Entity dead(Vector2f((WINDOW_WIDTH-50)/2, (WINDOW_HEIGHT-50*COLUMNS-50)/2), deadTex, Vector2f(50,50));
  Entity border(Vector2f(0,0), borderTex, Vector2f(800,150));

  Game game;

  std::vector<Field> fields = game.createFields(fieldTex, fieldFont, COLUMNS, ROWS, WINDOW_HEIGHT, BOMBS);

  int startTime = 0;
  int currentTime = 0;

  bool firstClick = true;
  bool showFields = false;

  bool gameRunning = true;
  while (gameRunning) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT)
        gameRunning = false;
            
      if(event.type == SDL_MOUSEBUTTONUP && game.gameState < 2) {
        firstClick = false;
        game.revealField(event, fields, clickedFieldTex, crossedField, game);
        game.checkWin(fields, currentTime-startTime);
      }

      if(event.type == SDL_MOUSEBUTTONUP) {
        int mouseX = event.motion.x;
        int mouseY = event.motion.y;

        int resetBtnX = smileBg.getPos().x;
        int resetBtnY = smileBg.getPos().y;

        if(mouseX > resetBtnX && mouseX < resetBtnX + smileBg.getSize().x 
            && mouseY > resetBtnY && mouseY < resetBtnY + smileBg.getSize().y) {
            firstClick = true;
            game.gameState = 0;
            fields.clear();
            fields = game.createFields(fieldTex, fieldFont, COLUMNS, ROWS, WINDOW_HEIGHT, BOMBS);
          }

        if(game.gameState == 2 && !showFields) {
          if(mouseY > 155 && currentTime+3 <= utils::hireTimeInSeconds())
            showFields = true;
        }
        }
      }
      
    window.setRenderColor(185,185,185);
    window.clear();

    if(firstClick)
      startTime = utils::hireTimeInSeconds();

    if(game.gameState < 2)
      currentTime = utils::hireTimeInSeconds();

    const char* timeC = game.getTimeC(startTime, currentTime).c_str();

    SDL_Surface* bombs = TTF_RenderText_Blended(bFont, std::to_string(game.getBombAmount()).c_str(), {255,0,0,255});
    SDL_Surface* time = TTF_RenderText_Blended(bFont, timeC, {255,0,0,255});

    window.render(bombsBg);
    window.render(timeBg);
    window.render(bombs, Vector2f(25+(bombs->w+20)/2-(bombs->w)/2, (WINDOW_HEIGHT-50*COLUMNS-bombs->h)/2));
    window.render(time, Vector2f(WINDOW_WIDTH-35-time->w, (WINDOW_HEIGHT-50*COLUMNS-time->h)/2));
    window.render(border);

    window.render(smileBg);
    
    if(game.gameState == 3)
      window.render(dead);
    else
      window.render(smile);

    if(game.gameState == 2 && showFields == false) {
      SDL_Surface* winText = TTF_RenderText_Blended(bFont, "Wygrywasz!", {0,0,0,255});
      window.render(winText, Vector2f((WINDOW_WIDTH-winText->w)/2, (50*COLUMNS-winText->h)/2));
      SDL_Surface* timeText = TTF_RenderText_Blended(bFont, ("Twoj czas: " + game.stylizeTime(currentTime-startTime)).c_str(), {0,0,0,255});
      window.render(timeText, Vector2f((WINDOW_WIDTH-timeText->w)/2, (50*COLUMNS-timeText->h)/2+70));
      SDL_Surface* recText = TTF_RenderText_Blended(bFont, ("Rekord: " + game.stylizeTime(game.getHighScore())).c_str(), {0,0,0,255});
      window.render(recText, Vector2f((WINDOW_WIDTH-recText->w)/2, (50*COLUMNS-recText->h)/2+140));
      SDL_Surface* infoText1 = TTF_RenderText_Blended(sFont, "Kliknij lewy przycisk myszy", {0,0,0,255});
      window.render(infoText1, Vector2f((WINDOW_WIDTH-infoText1->w)/2, WINDOW_HEIGHT-100));
      SDL_Surface* infoText2 = TTF_RenderText_Blended(sFont, "by zobaczyc plansze", {0,0,0,255});
      window.render(infoText2, Vector2f((WINDOW_WIDTH-infoText2->w)/2, WINDOW_HEIGHT-60));
    } else {
      for(Field& f : fields)
        window.render(f);
      for(int i = 0; i<fields.size(); i++)
        fields[i].setSurf(fieldFont);
    }


    window.display();
  }
  
  TTF_CloseFont(bFont);
  TTF_CloseFont(sFont);
  TTF_CloseFont(fieldFont);
  SDL_DestroyTexture(fieldTex);
  SDL_DestroyTexture(clickedFieldTex);
  SDL_DestroyTexture(deadTex);
  SDL_DestroyTexture(smileTex);
  SDL_DestroyTexture(fontBg);
  SDL_DestroyTexture(borderTex);
  window.destroy();
  SDL_Quit();
  return 0;
}