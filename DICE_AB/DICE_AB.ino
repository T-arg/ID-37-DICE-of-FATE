/*
  DICE of FATE: https://t-arg.github.io/ID-37-DICE-of-FATE/

  Arduboy version 1.7.2:  https://github.com/T-arg/ID-37-DICE-of-FATE

  MADE by TEAM a.r.g. : https://t-arg.github.io/

  2016 - CASTPIXEL - JO3RI

  License: MIT : https://opensource.org/licenses/MIT

*/

//determine the game
#define GAME_ID 37

#include "globals.h"
#include "menu.h"
#include "game.h"

typedef void (*FunctionPointer) ();
const FunctionPointer PROGMEM  mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateDiceTypeAndAmount,
  stateDiceRolling,
  stateDiceResult,
};

void setup()
{
  arduboy.begin();
  arduboy.setFrameRate(60);
  arduboy.initRandomSeed();
}

void loop()
{
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  ((FunctionPointer) pgm_read_word (&mainGameLoop[gameState]))();
  arduboy.display();
  for (byte i = 0; i < 4; i++) if (buttonPressed[i] == true) pressedButtonCounter++;
  if (pressedButtonCounter > 6)
  {
    for (byte i = 0; i < 4; i++)
    {
      buttonPressed[i] = false;
    }
    pressedButtonCounter = 0;
  }
}

