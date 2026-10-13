#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduboy2.h>
#include "ATMconfig.h"
#include <ATMlib.h>
#include "bitmaps.h"

//define menu states (on main menu)
#define STATE_MENU_INTRO             0
#define STATE_MENU_MAIN              1
#define STATE_MENU_HELP              2
#define STATE_MENU_PLAY              3
#define STATE_MENU_INFO              4
#define STATE_MENU_SOUNDFX           5
#define STATE_MENU_JBOX              6

//define game states (on main menu)
#define STATE_DICE_TYPE_AND_AMOUNT   7
#define STATE_DICE_ROLLING           8
#define STATE_DICE_RESULT            9

#define BUTTON_A                     0
#define BUTTON_B                     1
#define BUTTON_UP                    2
#define BUTTON_DOWN                  3

Arduboy2Base arduboy;
Sprites sprites;
ATMsynth ATM;

byte gameState = STATE_MENU_INTRO;   // start the game with the TEAM a.r.g. logo
byte menuSelection = STATE_MENU_PLAY; // PLAY menu item is pre-selected
byte globalCounter = 0;
byte pressedButtonCounter = 0;
boolean buttonPressed[] = {false, false, false, false};

#define INTRO_DIV 2
#include "arduboy_intro.h"
#include "music.h"

#endif
