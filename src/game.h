#ifndef __GAME_H__
#define __GAME_H__

#include <stdio.h>
#include <stdbool.h>

typedef struct AreaInfo { //지뢰판의 구역의 정보를 다루는 구조체
	int isMine;       //지뢰의 유무를 저장하는 변수
	int isVisible;    //해당 칸이 보여지는 상태를 저장하는 변수
	int mark;         //해당 칸의 표식이 무엇인지 저장하는 변수
	int mineNum;      //해당 칸의 주변 지뢰의 수를 저장하는 변수
}AreaInfo;

typedef struct InitialSetting { // 게임 시작시에 설정하는 맵의 길이, 높이, 지뢰의 갯수, 시드를 저장하는 구조체
	int width;          // 맵의 길이를 저장하는 변수
	int height;               // 맵의 높이를 저장하는 변수
	int num;          // 지뢰의 갯수를 저장하는 변수
	int seed;         // 시드를 저장하는 변수
}InitialSetting;

typedef struct Position { //계산 혹은 함수값 리턴시 x, y의 값을 한번에 다루기 위한 구조체
	int x;
	int y;
}Position;

int IsHereMine(Position pos);
int IsVisible(Position pos);
int Mark(Position pos);
int MineNum(Position pos);
bool IsOut(Position pos);
void SetMine(Position pos);
void SetVisible(Position pos, int visibility);
void SetMark(Position pos, int mark);
void SetAdjecantPosition(Position *adjPos, Position pos);
void BFS(Position pos, int* visibleAreaCnt);
void Gameover(void);
int SearchMineHere(Position inputPos, int* visibleAreaCnt);
int MemoHere(Position inputPos, int mark);
int CheckUnSearchedMines(int* visibleAreaCnt);
int SwitchingCommand(Position inputPos, int* visibleAreaCnt, char command);
int ProcessUserInput(int* init, int* visibleAreaCnt);
void MemoryUnallocate(void);

#endif
