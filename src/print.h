#ifndef __PRINT_H__
#define __PRINT_H__

#include "game.h"
#include <stdlib.h>

void PrintMineField(int isCheatEnable);
void PrintInitSet(void);
void PrintRow(void);
void WipeScreen(void);
void ProcessGameResult(int result);
void HelpScreen(void);
bool TitleScreen(void);
int OptionScreen(void);

#endif
