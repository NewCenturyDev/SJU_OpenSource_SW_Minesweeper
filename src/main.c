#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "stack.h"
#include "exception.h"

extern void AllocNInitVar(void);
extern void GetInitSetFromInput(void);
extern void GetInitSetFromDefault(int select);
extern void InitSetFromConsoleArg(int argc, char **argv);
extern void MineFieldSizeException(void);
extern void PrintInitSet(void);
extern void PrintMineField(int isCheatEnable);
extern void PrintMineField(int isCheatEnable);
extern void SetRandSeed(void);
extern int OptionScreen(void);
extern bool TitleScreen(void);

/* 전역 변수 선언 */
AreaInfo **areaInfo = NULL; //지뢰판의 정보를 저장할 변수
InitialSetting initSet = { 0, 0, 0, 0 };        //초기설정값 - initSet이 초기화되지 않았습니다 컴파일 오류를 피하기 위해 불가피하게 임의의 쓰레기 값을 하드코딩
const int CONTINUE = 0; //게임 계속 진행
const int LOSE = 1;     //게임 패배
const int WIN = 2;      //게임 승리

int main(int argc, char **argv) {
	bool willStart; //게임 시작여부
	int optionSelection;    //옵션 화면에서 선택된 옵션
	int visibleAreaCnt = 0; //밝혀진 지뢰의 숫자를 0으로 초기화한다
	int isInitialized = false;      //아직 지뢰판 초기화가 되지 않았으므로 초기화 여부를 FALSE로 설정한다
	int gameResult = CONTINUE;      //게임 승패 여부를 저장
	char anyKey;    //게임 종료시 사용자에게 결과를 보여주기 위해 사용 (GUI 환경에서 터미널창으로 실행시 도스창이 바로 꺼지는 것을 방지하기 위함)

	if (argc > 3)
		//프로그램 실행 전 옵션으로 인자를 같이 주었다면 해당 인자로 초기조건 설정
		InitSetFromConsoleArg(argc, argv);
	else {
		//타이틀 화면 출력
		willStart = TitleScreen();

		//게임시작이 선택되었을 때만 진행하고 그렇지 않은 경우 계속 타이틀 화면을 표출합니다. (3번 선택시 종료)
		while (willStart != true)
			willStart = TitleScreen();

		//옵션화면에서 설정한 내용을 받아온다
		optionSelection = OptionScreen();

		switch (optionSelection) {
		case 0: case 1: case 2: case 3:
			//사용자가 난이도를 선택했을 경우 하드코딩 되어있는 기본 초기조건으로 설정
			GetInitSetFromDefault(optionSelection);
			break;
		case 4:
			//사용자가 커스텀 게임을 선택했을 경우 사용자로부터 초기조건 입력받음
			GetInitSetFromInput();
			//사용자 입력 검증
			MineFieldSizeException();
			MineNumException();
			break;
		default:
			break;
		}
	}

	//게임 시작

	//시드값 설정
	SetRandSeed();
	srand(initSet.seed);

	//설정된 정보 다시 출력해줌
	PrintInitSet();

	//지뢰판 동적할당
	AllocNInitVar();

	//지뢰판 최초 출력
	PrintMineField(false);

	while (gameResult == CONTINUE)
		//게임결과 플래그가 진행인 동안 게임 계속 진행
		gameResult = ProcessUserInput(&isInitialized, &visibleAreaCnt);

	//프로그램 종료시 메모리 할당 해제
	MemoryUnallocate();
	printf("PRESS ANY KEY...");
	scanf_s("%c", &anyKey, 1);
	return 0;
}
