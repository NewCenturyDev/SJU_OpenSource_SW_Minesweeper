#pragma warning(disable: 4996)
#include <stdlib.h>
#include "game.h"
#include "stack.h"

extern void Initialize(int* init, Position inputPos);
extern void PrintMineField(int isCheatEnable);
extern void ProcessGameResult(int result);
extern AreaInfo **areaInfo;
extern InitialSetting initSet;
extern const int CONTINUE;
extern const int LOSE;
extern const int WIN;

int IsHereMine(Position pos) {
	//해당 좌표의 지뢰 유무 값 전달하는 함수
	int result;
	result = areaInfo[pos.x][pos.y].isMine;
	return result;
}
int IsVisible(Position pos) {
	//해당 좌표가 보여지는 상태에 대한 값 전달하는 함수
	int result;
	result = areaInfo[pos.x][pos.y].isVisible;
	return result;
}

int Mark(Position pos) {
	//해당 좌표의 표식의 값을 전달하는 함수
	int result;
	result = areaInfo[pos.x][pos.y].mark;
	return result;
}

int MineNum(Position pos) {
	//해당 칸 주변 지뢰의 수 전달하는 함수
	int result;
	result = areaInfo[pos.x][pos.y].mineNum;
	return result;
}

bool IsOut(Position pos) {
	//입력된 좌표가 범위 밖인지 판단하는 함수
	//좌표의 x,y값이 초기설정한 판 크기보다 크거나, 0보다 작다면 범위 바깥입니다.
	if (pos.x >= initSet.width || pos.y >= initSet.height || pos.x < 0 || pos.y < 0)
		return true;
	return false;
}

void SetMine(Position pos) {
	areaInfo[pos.x][pos.y].isMine = true;
	return;
}

void SetVisible(Position pos, int visibility) {
	//해당 칸의 볼 수 있음 여부를 갱신하는 함수
	if (visibility == true) {
		areaInfo[pos.x][pos.y].isVisible = true;
	}
	if (visibility == false) {
		areaInfo[pos.x][pos.y].isVisible = false;
	}
	return;
}

void SetMark(Position pos, int mark) {
	//마크 값 설정 함수
	const int DELETE = 0;
	const int SAFE = 1;
	const int DANGER = 2;
	const int QUERY = 3;
	switch (mark) {
	case 1:
		areaInfo[pos.x][pos.y].mark = DELETE;
		break;

	case 2:
		areaInfo[pos.x][pos.y].mark = SAFE;
		break;

	case 3:
		areaInfo[pos.x][pos.y].mark = DANGER;
		break;

	case 4:
		areaInfo[pos.x][pos.y].mark = QUERY;
		break;

	default:
		return;
	}
}

void SetAdjecantPosition(Position *adjPos, Position pos) {
	//x, y 주변의 인접한 8칸의 좌표들을 12시 방향부터 시계방향 순서대로 배열에 옮겨담는 함수
	//기존의 좌표에서 북쪽에 있는 좌표
	adjPos[0].x = pos.x;
	adjPos[0].y = pos.y + 1;

	//북동쪽에 있는 좌표
	adjPos[1].x = pos.x + 1;
	adjPos[1].y = pos.y + 1;

	//동쪽에 있는 좌표
	adjPos[2].x = pos.x + 1;
	adjPos[2].y = pos.y;

	//남동쪽에 있는 좌표
	adjPos[3].x = pos.x + 1;
	adjPos[3].y = pos.y - 1;

	//남쪽에 있는 좌표
	adjPos[4].x = pos.x;
	adjPos[4].y = pos.y - 1;

	//남서쪽에 있는 좌표
	adjPos[5].x = pos.x - 1;
	adjPos[5].y = pos.y - 1;

	//서쪽에 있는 좌표
	adjPos[6].x = pos.x - 1;
	adjPos[6].y = pos.y;

	//북서쪽에 있는 좌표
	adjPos[7].x = pos.x - 1;
	adjPos[7].y = pos.y + 1;
	return;
}

void BFS(Position pos, int* visibleAreaCnt) {
	const int adjNum = 8; //인접한 칸의 최대 갯수
	StackNode *stack = NULL; //스택을 가리킬 헤드 포인터
	Position adjPos[8]; //스택에서 팝한 좌표의 주변 8칸의 좌표값을 저장할 배열
	Position poppedPos; //팝한 노드의 좌표를 저장할 변수

	if (IsOut(pos) || IsVisible(pos))
		//선택한 좌표가 보이는 상태이거나 범위 밖의 좌표이면 예외처리
		return;

	//스택에 푸시하고 인접한 8칸의 좌표를 배열에 저장
	stack = Push(stack, pos);
	SetAdjecantPosition(adjPos, pos); //

	while (!IsEmptyStack(stack)) {
		//스택이 비어있지않다면 스택의 노드를 팝하고 그 원소를 저장
		poppedPos = Pop(&stack);

		if (!(IsOut(poppedPos) || IsVisible(poppedPos))) {
			//팝한 좌표가 보이는 상태와 범위 밖의 좌표가 아닐 경우 실행
			//보이는 수를 증가시키고 해당 좌표의 칸을 보이는 상태로 변경
			++(*visibleAreaCnt);
			SetVisible(poppedPos, true);

			if (!(areaInfo[poppedPos.x][poppedPos.y].mineNum)) {
				//주변의 지뢰가 매설되어 있지 않은 경우 실행
				//주변 8칸의 좌표를 배열에 저장하고 스택에 푸시
				SetAdjecantPosition(adjPos, poppedPos);
				for (int i = 0; i < adjNum; i++)
					stack = Push(stack, adjPos[i]);
			}
		}
	}
	return;
}

void Gameover(void) {
	int i, j;
	for (i = 0; i < initSet.width; ++i) {
		for (j = 0; j < (initSet.height); j++) {
			areaInfo[i][j].isVisible = (int)true;
		}
	}
	return;
}

int SearchMineHere(Position inputPos, int* visibleAreaCnt) {
	//해당 위치의 지뢰를 파 보는 함수
	const int DELETE = 0;
	int gameResult = CONTINUE;

	SetMark(inputPos, DELETE);
	if (IsHereMine(inputPos)) {     //지뢰 밟았을 경우 패배 처리
		gameResult = LOSE;
		ProcessGameResult(gameResult);  //패배 처리
		return gameResult;
	}
	BFS(inputPos, visibleAreaCnt);
	WipeScreen();
	PrintInitSet(); //초기정보 출력
	PrintMineField(false);  //지뢰판 출력
	return gameResult;
}

int MemoHere(Position inputPos, int mark) {
	//해당 위치에 메모하는 함수
	const int ERROR = 1;

	if (!IsVisible(inputPos))
		SetMark(inputPos, mark);
	else {
		WipeScreen();
		PrintInitSet(); //초기정보 출력
		PrintMineField(false);  //지뢰판 출력
		printf("Invaild Command: Already visible\n");
		return ERROR;   //예외 발생
	}
	WipeScreen();
	PrintInitSet(); //초기정보 출력
	PrintMineField(false);  //지뢰판 출력
	return CONTINUE;        //정상 진행
}

int CheckUnSearchedMines(int* visibleAreaCnt) {
	//남은 지뢰 갯수를 검사하는 함수
	int gameResult = CONTINUE;
	int safeAreas = initSet.width * initSet.height - initSet.num;

	if (*visibleAreaCnt == safeAreas) {
		//다 찾았을 경우 승리 처리
		gameResult = WIN;
		ProcessGameResult(gameResult);  //승리 처리
		return gameResult;      //승리
	}
	return gameResult;
}

int SwitchingCommand(Position inputPos, int* visibleAreaCnt, char command) {
	//명령어 분기처리 함수
	const int ERROR = 1;
	int gameResult = CONTINUE;
	int skipToNext = CONTINUE;

	switch (command) {
	case 0:
		//지뢰를 파보는 명령(0)
		gameResult = SearchMineHere(inputPos, visibleAreaCnt);
		if (gameResult == 1)
			//만약 패배했다면
			return gameResult;      //패배
		break;

	case 1: case 2: case 3: case 4:
		//메모 기능 명령(1,2,3,4)
		skipToNext = MemoHere(inputPos, command);
		if (skipToNext == ERROR)
			//예외 발생시 이번 input 프로세스를 무효화하고 건너뜀
			return gameResult;      //계속 진행
		break;

	case 9:
		//치트 기능 실행(9)
		WipeScreen();
		PrintInitSet(); //초기정보 출력
		PrintMineField(true);   //치트 동작
		return gameResult;      //계속 진행

	default:
		//잘못된 명령어를 입력했을 경우
		WipeScreen();
		PrintInitSet(); //초기정보 출력
		PrintMineField(false);
		printf("Invaild Command: Command does not exist\n");
		return gameResult;      //계속 진행
	}
	return gameResult;
}

int ProcessUserInput(int* init, int* visibleAreaCnt) {  //사용자의 입력, 입력값 검증, 지뢰 탐색 등의 여러 기능이 있는 함수.
	Position inputPos;      //사용자로부터 입력받을 x,y좌표와 명령어(s)
	char command;   //사용자가 입력한 명령어
	int gameResult = CONTINUE;

	//사용자 입력 처리
	printf("Enter X coordinate, Y coordinate, and instruction\n");
	scanf_s("%d %d", &inputPos.x, &inputPos.y);
	scanf_s(" %c", &command, 1);
	getchar();
	printf("\n");

	if (command >= '0' && command <= '9') {
		command = command - '0';
	}
	//입력값 검증 (좌표값 범위 검사)
	if (IsOut(inputPos)) {
		WipeScreen();
		PrintInitSet();
		PrintMineField(false);
		printf("Invaild Command: Out of range\n");
		return gameResult;      //계속 진행
	}

	//게임판 초기화
	if (*init != true)
		Initialize(init, inputPos);

	//명령어 검사 및 분기처리
	gameResult = SwitchingCommand(inputPos, visibleAreaCnt, command);
	if (gameResult != CONTINUE)
		return gameResult;

	//지뢰를 전부 찾았는지 검사
	gameResult = CheckUnSearchedMines(visibleAreaCnt);
	return gameResult;      //계속 진행 또는 승리
}

void MemoryUnallocate(void) {
	//프로그램 종료시 메모리 할당 해제 함수
	int i;
	for (i = 0; i < initSet.width; i++)
		free(areaInfo[i]);
	free(areaInfo);
	return;
}
