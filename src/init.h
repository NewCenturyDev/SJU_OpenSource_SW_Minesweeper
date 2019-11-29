#ifndef __INIT_H__
#define __INIT_H__

#include "game.h"

void Initialize(int* init, Position inputPos);
void IncNum(int x, int y);
void InitSetFromConsoleArg(int argc, char **argv);
void GetInitSetFromInput(void);
void SetRandSeed(void);
void AllocNInitVar(void);

#endif
