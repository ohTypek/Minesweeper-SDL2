#pragma once

#include<SDL.h>

namespace utils {
  inline float hireTimeInSeconds() {
    return SDL_GetTicks()/1000;
  }

  inline std::string transformSecondsToClock(int seconds) {
    
    std::string minutes = std::to_string(seconds/60), 
                   secs = std::to_string(seconds%60);

    if(seconds/60 < 10) minutes.insert(0,1,'0');
    if(seconds%60 < 10)    secs.insert(0,1,'0');

    std::string msg = "";
    msg += minutes + ":" + secs; 

    return msg;
  }
}