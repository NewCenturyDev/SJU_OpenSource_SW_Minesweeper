#include "exception.h"

extern InitialSetting initSet;
extern void GetInitSetFromInput(void);

void MineFieldSizeException(void) {
	// 초기조건 입력 오류시 예외처리 코드 (width, height값)
	int isWrongInput = false;       //잘못된 초기조건 입력 예외처리 플래그 (TRUE == 예외 발생 / FALSE == 정상 진행)

	do {
		if (initSet.width < 1 || initSet.height < 1)
			//지뢰판의 가로 및 세로 길이가 음수로 입력된 경우
			isWrongInput = true;
		else if (initSet.width > 99 || initSet.height > 99)
			//지뢰판의 가로 및 세로 길이가 100 이상으로 너무 크게 입력된 경우
			isWrongInput = true;
		else
			//지뢰판의 가로 및 세로 길이가 정상 입력된 경우
			isWrongInput = false;
		if (isWrongInput) {
			//잘못된 입력이 주어지면 다시 입력을 요구합니다
			WipeScreen();
			printf("[ERROR]width and height must be in range 1~99. Try again.\n");
			GetInitSetFromInput();
		}
	} while (isWrongInput);
	return;
}

void MineNumException(void) {
	// 초기조건 입력 오류시 예외처리 코드 (mineNum값)
	int isWrongInput = false;       //잘못된 초기조건 입력 예외처리 플래그 (TRUE == 예외 발생 / FALSE == 정상 진행)

									//초기조건 입력 오류시 예외처리 코드(num값)
	do {
		if (initSet.num >= initSet.width * initSet.height)
			//지뢰 갯수가 지뢰판 크기와 같거나 더 크게 입력된 경우
			isWrongInput = true;
		else if (initSet.num <= 0)
			//지뢰 갯수가 0개 또는 음수로 입력된 경우
			isWrongInput = true;
		else
			//지뢰 갯수가 정상 입력된 경우
			isWrongInput = false;
		if (isWrongInput) {
			//잘못된 입력이 주어지면 다시 입력을 요구합니다.
			WipeScreen();
			printf("[ERROR]The number of mines must be in range 1 to (board size - 1). Try again.\n");
			GetInitSetFromInput();
		}
	} while (isWrongInput);
	return;
}
