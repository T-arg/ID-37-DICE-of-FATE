#ifndef MENU_H
#define MENU_H

#include "globals.h"

byte titleFrame = 0;
byte titleCounter = 27;
boolean slideMenuLeft;
boolean slidingMenu;
byte slideMenuCounter = 0;
byte menuX;
byte soundMenuX;

void titleScreen()
{
  if (arduboy.everyXFrames(6)) titleCounter++;
  if (titleCounter > 47)titleCounter = 0;
  titleFrame = titleCounter;
  if (titleFrame > 7) titleFrame = 0;
  for (byte i = 0; i < 2; i++)
  {
    sprites.drawSelfMasked(i * 64, 0, titleTop, i);
    sprites.drawSelfMasked(i * 64, 12, titleGleam, titleFrame + (i * 8));
  }
}

void updateSlidingMenu(byte amount)
{
  if (slideMenuLeft && slidingMenu && (slideMenuCounter < amount + 1))
  {
    slideMenuCounter++;
    if (amount == 25)menuX++;
    if (amount == 21)soundMenuX++;
  }

  if (!slideMenuLeft && slidingMenu && (slideMenuCounter < amount + 1))
  {
    slideMenuCounter++;
    if (amount == 25)menuX--;
    if (amount == 21)soundMenuX--;
  }

  if (slideMenuCounter > amount - 1)
  {
    slideMenuCounter = 0;
    slidingMenu = false;
  }
}

void stateMenuIntro()
{
  if (!arduboyIntro()) return;
  globalCounter++;
  sprites.drawSelfMasked(49, 20, T_arg, 0);
  drawTwinkles(49, 20, globalCounter);
  if (globalCounter > 180)
  {
    gameState = STATE_MENU_MAIN;
    slideMenuLeft = false;
    slidingMenu = false;
    menuX = 25 * (menuSelection - 3);
    soundMenuX = 21 * !arduboy.audio.enabled();
  }
}

void stateMenuMain()
{
  titleScreen();
  for (byte i = 0; i < 2; i++) sprites.drawSelfMasked(i * 64, 36, titleBottom, i);
  updateSlidingMenu(25);
  if (!slidingMenu) sprites.drawSelfMasked(52, 47, titleLine, 0);
  for (byte i = 0; i < 4; i++)
  {
    bool sel = (3 + i) == menuSelection && !slidingMenu;
    int x = 52 - menuX + 25 * i;
    if (i < 3) sprites.drawPlusMask(x, 48, titleText_plus_mask, i + 6 * sel);
    else
    {
      arduboy.fillRect(x - 1, 47, 26, 9, BLACK);                           // black behind JBOX: 2 pixels before the word, 1 after
      sprites.drawOverwrite(x + 1, 47, jboxMenu, !sel);                    // JBOX: 23 x 9, frame 0 selected, 1 not
    }
  }

  sprites.drawSelfMasked(51, 56, titleSelector, 0);
  if (arduboy.justPressed(LEFT_BUTTON) && !slidingMenu && (menuSelection > 3))
  {
    slidingMenu = true;
    slideMenuLeft = false;
    menuSelection--;
    sfx(SFX_MOVE);
  }
  else if (arduboy.justPressed(RIGHT_BUTTON) && !slidingMenu && (menuSelection < STATE_MENU_JBOX))
  {
    slidingMenu = true;
    slideMenuLeft = true;
    menuSelection++;
    sfx(SFX_MOVE);
  }

  else if (arduboy.justPressed(B_BUTTON))
  {
    gameState = menuSelection;
    sfx(SFX_SELECT);
  }
}

void stateMenuHelp()
{
  if (arduboy.justPressed(B_BUTTON)) gameState = STATE_MENU_MAIN;
}


void stateMenuInfo()
{
  titleScreen();
  for (byte i = 0; i < 2; i++) sprites.drawSelfMasked(i * 64, 36, infoScreen, i);
  if (arduboy.justPressed(B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuSoundfx()
{
  titleScreen();
  for (byte i = 0; i < 2; i++) sprites.drawSelfMasked(i * 64, 36, titleBottom, i);
  updateSlidingMenu(21);
  if (!slidingMenu) sprites.drawSelfMasked(54, 47, titleLine, 1);
  sprites.drawPlusMask(soundMenuX + 21 - 9, 48, titleText_plus_mask, 5);
  sprites.drawPlusMask(soundMenuX + 21 + 12, 48, titleText_plus_mask, 3 + ((1 - slidingMenu) * (6 * (1 - arduboy.audio.enabled()))));
  sprites.drawPlusMask(soundMenuX + 21 + 33, 48, titleText_plus_mask, 4 + ((1 - slidingMenu) * (6 * arduboy.audio.enabled())));


  sprites.drawSelfMasked(51, 56, titleSelector, 1);
  if (arduboy.justPressed(LEFT_BUTTON) && !slidingMenu && arduboy.audio.enabled())
  {
    slidingMenu = true;
    slideMenuLeft = true;
    arduboy.audio.off();
  }
  else if (arduboy.justPressed(RIGHT_BUTTON) && !slidingMenu && !arduboy.audio.enabled())
  {
    slidingMenu = true;
    slideMenuLeft = false;
    arduboy.audio.on();
  }
  else if (arduboy.justPressed(B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
  }
}

// the jukebox: UP / DOWN pick MUSIC or SNDFX, LEFT / RIGHT the number, B plays, A stops (or leaves)
byte jbRow = 0;
byte jbNr[2] = {1, 1};

// little arrows around the number at x, y (w x h pixels): v = above and below (UP / DOWN changes it), else left and right (LEFT / RIGHT changes it)
void jbArrows(byte x, byte y, byte w, byte h, bool v)
{
  byte cx = x + w / 2, cy = y + h / 2;
  if (v)
  {
    arduboy.drawPixel(cx, y - 3, WHITE);
    arduboy.drawFastHLine(cx - 1, y - 2, 3, WHITE);
    arduboy.drawFastHLine(cx - 1, y + h + 1, 3, WHITE);
    arduboy.drawPixel(cx, y + h + 2, WHITE);
  }
  else
  {
    arduboy.drawPixel(x - 3, cy, WHITE);
    arduboy.drawFastVLine(x - 2, cy - 1, 3, WHITE);
    arduboy.drawFastVLine(x + w + 1, cy - 1, 3, WHITE);
    arduboy.drawPixel(x + w + 2, cy, WHITE);
  }
}

void stateMenuJbox()
{
  titleScreen();
  for (byte i = 0; i < 2; i++) sprites.drawSelfMasked(i * 64, 36, titleBottom, i);
  byte max = jbRow ? JBOX_SFX : JBOX_MUSIC;
  if (arduboy.justPressed(LEFT_BUTTON)) jbRow = 0;
  if (arduboy.justPressed(RIGHT_BUTTON)) jbRow = 1;
  if (arduboy.justPressed(UP_BUTTON)) jbNr[jbRow] = jbNr[jbRow] % max + 1;
  if (arduboy.justPressed(DOWN_BUTTON)) jbNr[jbRow] = (jbNr[jbRow] + max - 2) % max + 1;
  if (arduboy.justPressed(B_BUTTON))
  {
    if (jbRow) sfx(jbNr[1] - 1);
    else if (arduboy.audio.enabled()) musicPlay(jbNr[0] - 1);
  }
  if (arduboy.justPressed(A_BUTTON))
  {
    if (musicId != MUSIC_OFF) musicPlay(MUSIC_OFF);
    else gameState = STATE_MENU_MAIN;      // musicUpdate restarts the title tune
  }
  for (byte i = 0; i < 2; i++)
  {
    byte x = 12 + 64 * i;
    jbArrows(x + 31, 54, 9, 5, true);
    sprites.drawOverwrite(x, 52, jboxWords, 2 * i + (jbRow != i));         // frame 0 selected, 1 not
    if (jbRow != i || (arduboy.frameCount & 16))
    {
      sprites.drawSelfMasked(x + 31, 54, jboxNumbers, jbNr[i] / 10);
      sprites.drawSelfMasked(x + 36, 54, jboxNumbers, jbNr[i] % 10);
    }
  }
}

#endif
