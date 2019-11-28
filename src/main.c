#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/* 구조체 정의 */
typedef struct AreaInfo { //지뢰판의 구역의 정보를 다루는 구조체
	int isMine;       //지뢰의 유무를 저장하는 변수
	int isVisible;    //해당 칸이 보여지는 상태를 저장하는 변수
	int mark;         //해당 칸의 표식이 무엇인지 저장하는 변수
	int mineNum;      //해당 칸의 주변 지뢰의 수를 저장하는 변수
}AreaInfo;

typedef struct InitialSetting { // 게임 시작시에 설정하는 맵의 길이, 높이, 지뢰의 갯수, 시드를 저장하는 구조체
	int width;          // 맵의 길이를 저장하는 변수
	int height;		  // 맵의 높이를 저장하는 변수
	int num;          // 지뢰의 갯수를 저장하는 변수
	int seed;         // 시드를 저장하는 변수
}InitialSetting;

typedef struct Position { //계산 혹은 함수값 리턴시 x, y의 값을 한번에 다루기 위한 구조체
	int x;
	int y;
}Position;

typedef struct StackNode { //스택 구조를 사용하기 위한 구조체
	Position elem;
	struct StackNode *next;
}StackNode;

/* 전역 변수 선언 */
AreaInfo **areaInfo = NULL; //지뢰판의 정보를 저장할 변수
const int CONTINUE = 0;	//게임 계속 진행
const int LOSE = 1;	//게임 패배
const int WIN = 2;	//게임 승리

/* 함수 원형 선언 */
//todo: 맨 마지막에 작업할 예정입니다. (디버깅시 혼동 방지를 위해)

//타이틀 화면으로 게임 시작, 도움말, 게임 종료 중 하나의 메뉴를 선택할 수 있다.
int TitleScreen(void) {
	int select = 1;

	printf(	"    ┏━━━━━━ CLS MineSweeper ━━━━━━┓\n");
	printf("    ┃                             ┃\n");
	printf("    ┃                             ┃\n");
	printf("    ┃      ① PLAY                ┃\n");
	printf("    ┃      ② HELP(현재 미구현)   ┃\n");
	printf("    ┃      ③ EXIT                ┃\n");
	printf("    ┃                             ┃\n");
	printf("    ┃                             ┃\n");
	printf("    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");

	printf("     Select number to continue ☞ ");

	do {
		scanf_s("%d", &select);
		if (select != 1 && select != 2 && select != 3) {
			printf("Wrong input\n");
			printf("Please enter again\n");
		}
	} while (select != 1 && select != 2 && select != 3);
	printf("\n");
	switch (select) {
		//게임을 시작합니다
	case 1:
		return 1;
		//도움말 화면으로 이동합니다
		//TODO: 도움말 화면의 구현
	case 2:
		printf("현재 미구현"); //TODO: 도움말 화면 구현시 제거해야함
		exit(0);
		return 2;
		//게임을 종료합니다
	case 3:
		exit(0);
	default:
		break;
	}
}

//옵션 화면으로, 게임의 난이도를 선택할 수 있다.
int OptionScreen() {
	int select;

	printf("	    STAGE LEVEL\n\n");
	printf("	    ⑴ EASY\n");
	printf("	    ⑵ MEDIUM\n");
	printf("	    ⑶ HARD\n");
	printf("	    ⑷ EXPERT\n");
	printf("	    ⑸ CUSTOM LEVEL\n\n");

	do {
		printf("     Select number to continue ☞  ");
		scanf_s("%d", &select);
		if (select != 1 && select != 2 && select != 3 && select != 4 && select != 5) {
			printf("※ Wrong input ※\n");
			printf("Please enter again");
		}
	} while (select != 1 && select != 2 && select != 3 && select != 4 && select != 5);

	switch (select-1) {
	case 0:
		return 0;
	case 1:
		return 1;
	case 2:
		return 2;
	case 3:
		return 3;
	case 4:
		return 4;
	default:
		break;
	}
}

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

bool IsOut(Position pos, InitialSetting initSet) {
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

void IncNum(int x, int y, InitialSetting initSet) {
	//안내숫자 갱신 함수
	//주변 8방향에 대해 반복 호출시 pos 구조체의 빈번한 갱신에 대한 불편함이 있어 불가피하게 pos 구조체 미사용
	Position pos;
	pos.x = x;
	pos.y = y;
	if (IsOut(pos, initSet))
		return;
	else
		areaInfo[pos.x][pos.y].mineNum = areaInfo[pos.x][pos.y].mineNum + 1;
	return;
}

void AllocNInitVar(InitialSetting initSet) {
	/*
	기능: 지뢰판을 이차원 배열로 할당하고 초기화가 필요한 구조체 멤버들을 초기화한다.
	파라미터 : width은 지뢰판의 가로 칸수를 height은 지뢰판의 세로 칸수를 나타낸다.
	*/
	const int ZERO = 0;  //갯수가 0일때 쓰일 상수로, 정수 0의 값을 가진다.
	int i;
	int j;

	areaInfo = (AreaInfo **)malloc(initSet.width * sizeof(AreaInfo *));
	if (areaInfo == NULL) {
		// 동적 할당에 실패한 경우 아래 문구와 함께 종료된다.
		printf("Not enough memory!");
		return;
	}
	for (i = 0; i < initSet.width; i++) {
		areaInfo[i] = (AreaInfo *)malloc(initSet.height * sizeof(AreaInfo));
		if (areaInfo[i] == NULL) {
			// 동적 할당에 실패한 경우 아래 문구와 함께 종료된다.
			printf("Not enough memory!");
			return;
		}
	}

	//구조체 배열의 멤버 값을 초기화한다.
	for (i = 0; i < initSet.width; i++) {
		for (j = 0; j < initSet.height; j++) {
			areaInfo[i][j].isMine = false;
			areaInfo[i][j].isVisible = false;
			areaInfo[i][j].mark = ZERO;
			areaInfo[i][j].mineNum = ZERO;
		}
	}
	return;
}

StackNode *Push(StackNode *stack, Position pos) {
	//스택에 푸시하는 함수
	StackNode *newNode; //새로 생성한 노드
	StackNode *pushNode; //푸시할 위치의 전 노드

	newNode = (StackNode *)malloc(sizeof(StackNode));
	//새로 생성한 노드에 푸시하고자하는 위치값을 저장
	(newNode->elem).x = pos.x;
	(newNode->elem).y = pos.y;
	//새로 생성한 노드의 다음은 널 포인터를 가리켜 마지막을 알 수 있게함
	newNode->next = NULL;

	if (stack == NULL)
		//스택이 비어있다면 새로 생성한 노드를 리턴
		return newNode;
	pushNode = stack;

	//스택이 비어있지않다면 현재의 마지막 노드로 이동하여 새로 생성한 노드를 마지막에 오도록 추가
	while (pushNode->next != NULL)
		pushNode = pushNode->next;
	pushNode->next = newNode;

	return stack;
}

Position Pop(StackNode **stack) {
	//스택의 노드를 팝하는 함수
	Position poppedElem; //팝할 노드의 원소를 담을 변수
	StackNode *popNode; //메모리 해제를 하기 위해 팝할 노드를 가리킬 변수

	//팝할 노드의 원소와 노드를 저장
	poppedElem = (*stack)->elem;
	popNode = *stack;

	*stack = (*stack)->next;
	free(popNode);
	return poppedElem;
}

int IsEmptyStack(StackNode *stack) {
	//스택이 비어있는지 확인하는 함수
	if (stack == NULL)
		//스택이 비어있다면 TRUE를 리턴
		return true;
	//스택이 비어있지않다면 FALSE를 리턴
	return false;
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

void BFS(InitialSetting initSet, Position pos, int* visibleAreaCnt) {
	const int adjNum = 8; //인접한 칸의 최대 갯수
	StackNode *stack = NULL; //스택을 가리킬 헤드 포인터
	Position adjPos[8]; //스택에서 팝한 좌표의 주변 8칸의 좌표값을 저장할 배열
	Position poppedPos; //팝한 노드의 좌표를 저장할 변수

	if (IsOut(pos, initSet) || IsVisible(pos))
		//선택한 좌표가 보이는 상태이거나 범위 밖의 좌표이면 예외처리
		return;

	//스택에 푸시하고 인접한 8칸의 좌표를 배열에 저장
	stack = Push(stack, pos);
	SetAdjecantPosition(adjPos, pos); //

	while (!IsEmptyStack(stack)) {
		//스택이 비어있지않다면 스택의 노드를 팝하고 그 원소를 저장
		poppedPos = Pop(&stack);

		if (!(IsOut(poppedPos, initSet) || IsVisible(poppedPos))) {
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

void PrintMineField(int isCheatEnable, InitialSetting initSet) {
	char p[] = { ' ', 'S', 'X', '?' }; //  기존의 'O', '_'를 각각 'S', '?'로 변경하였습니다.
	Position nowProcHere;
	char mineNumToChar;

	printf("  ");
	for (nowProcHere.x = 0; nowProcHere.x < initSet.width; ++nowProcHere.x) {
		printf("%-2d", nowProcHere.x);
	}
	printf("\n");
	for (nowProcHere.y = initSet.height - 1; nowProcHere.y >= 0; --nowProcHere.y) {
		printf("%-2d", nowProcHere.y);
		if (isCheatEnable)
			for (nowProcHere.x = 0; nowProcHere.x < initSet.width; ++nowProcHere.x) {
				if (IsHereMine(nowProcHere))
					printf("%-2c", '*');
				
				else {
					mineNumToChar = (char)areaInfo[nowProcHere.x][nowProcHere.y].mineNum + '0';
					printf("%-2c", mineNumToChar);
				}
			}
		else
			for (nowProcHere.x = 0; nowProcHere.x < initSet.width; ++nowProcHere.x) {
				if (IsVisible(nowProcHere)) {
					if (IsHereMine(nowProcHere))
						printf("%-2c", '*');
					
					else {
						mineNumToChar = (char)areaInfo[nowProcHere.x][nowProcHere.y].mineNum + '0';
						printf("%-2c", mineNumToChar);
					}

				}
				else 
					printf("%-2c", p[Mark(nowProcHere)]);
				
			}
		printf("%-2d", nowProcHere.y);
		printf("\n");
	}
	printf("  ");
	for (nowProcHere.x = 0; nowProcHere.x < initSet.width; ++nowProcHere.x) {
		printf("%-2d", nowProcHere.x);
	}
	printf("\n");
	if (isCheatEnable)
		printf("※ WARNING: CHAETING DETECTED ※\n");
}

void Gameover(InitialSetting initSet) {
	int i, j;
	for (i = 0; i < initSet.width; ++i) {
		for (j = 0; j < (initSet.height); j++) {
			areaInfo[i][j].isVisible = (int)true;
		}
	}
	return;
}

void Initialize(InitialSetting initSet, int* init, Position inputPos) {
	/* 함수 설명 */
	//지뢰판을 초기화하기 위한 함수입니다.
	Position randPos;		//랜덤으로 선정된 x, y좌표
	int leftMineToInstall = initSet.num;
	int i;

	for (i = 0; i < leftMineToInstall; ++i) {
		//반복해서 랜덤한 (a, b) 좌표 생성
		randPos.x = rand() % initSet.width;
		randPos.y = rand() % initSet.height;
		if (IsHereMine(randPos) || randPos.x == inputPos.x || randPos.y == inputPos.y) {
			leftMineToInstall++;	//이번 반복횟수를 무효로 하기 위해 1 증가시켜야 함. (continue 하더라도 i는 1 증가해버림. 이를 되돌려주기 위한 조치)
			continue;
		}
		else {
			//(a,b)좌표에 지뢰 매설 및 주변 8개 칸의 안내숫자 1씩 증가
			IncNum(randPos.x - 1, randPos.y - 1, initSet);
			IncNum(randPos.x - 1, randPos.y, initSet);
			IncNum(randPos.x - 1, randPos.y + 1, initSet);
			IncNum(randPos.x, randPos.y - 1, initSet);
			SetMine(randPos);
			IncNum(randPos.x, randPos.y + 1, initSet);
			IncNum(randPos.x + 1, randPos.y - 1, initSet);
			IncNum(randPos.x + 1, randPos.y, initSet);
			IncNum(randPos.x + 1, randPos.y + 1, initSet);
		}
	}
	*init = true;
	return;
}

void ProcessGameResult(InitialSetting initSet, int result) {
	//게임 승리 또는 패배시의 작업을 처리하는 함수
	switch (result) {
	case 1:	//게임 패배
		Gameover(initSet);
		PrintMineField(false, initSet);
		printf("\n");
		printf("   ≪ You lose ! ≫\n");
		
		return;
	case 2:	//게임 승리
		Gameover(initSet);
		PrintMineField(false, initSet);
		printf("\n");
		printf("   ≪ You win ! ≫\n");
		return;
	default:	//게임 계속 진행
		return;
	}
}

int SearchMineHere(InitialSetting initSet, Position inputPos, int* visibleAreaCnt) {
	//해당 위치의 지뢰를 파 보는 함수
	const int DELETE = 0;
	int gameResult = CONTINUE;

	SetMark(inputPos, DELETE);
	if (IsHereMine(inputPos)) {	//지뢰 밟았을 경우 패배 처리
		gameResult = LOSE;
		ProcessGameResult(initSet, gameResult);	//패배 처리
		return gameResult;
	}
	BFS(initSet, inputPos, visibleAreaCnt);
	PrintMineField(false, initSet);
	return gameResult;
}

int MemoHere(InitialSetting initSet, Position inputPos, int mark) {
	//해당 위치에 메모하는 함수
	const int ERROR = 1;

	if (!IsVisible(inputPos))
		SetMark(inputPos, mark);
	else {
		PrintMineField(false, initSet);
		printf("Invaild Command: Already visible\n");
		return ERROR;	//예외 발생
	}
	PrintMineField(false, initSet);
	return CONTINUE;	//정상 진행
}

int CheckUnSearchedMines(InitialSetting initSet, int* visibleAreaCnt) {
	//남은 지뢰 갯수를 검사하는 함수
	int gameResult = CONTINUE;
	int safeAreas = initSet.width * initSet.height - initSet.num;

	if (*visibleAreaCnt == safeAreas) {
		//다 찾았을 경우 승리 처리
		gameResult = WIN;
		ProcessGameResult(initSet, gameResult);	//승리 처리
		return gameResult;	//승리
	}
	return gameResult;
}

int SwitchingCommand(InitialSetting initSet, Position inputPos, int* visibleAreaCnt, char command) {
	//명령어 분기처리 함수
	const int ERROR = 1;
	int gameResult = CONTINUE;
	int skipToNext = CONTINUE;

	switch (command) {
	case 0:
		//지뢰를 파보는 명령(0)
		gameResult = SearchMineHere(initSet, inputPos, visibleAreaCnt);
		if (gameResult == 1)
			//만약 패배했다면
			return gameResult;	//패배
		break;

	case 1: case 2: case 3: case 4:
		//메모 기능 명령(1,2,3,4)
		skipToNext = MemoHere(initSet, inputPos, command);
		if (skipToNext == ERROR)
			//예외 발생시 이번 input 프로세스를 무효화하고 건너뜀
			return gameResult;	//계속 진행
		break;

	case 9:
		//치트 기능 실행(9)
		PrintMineField(true, initSet);	//치트 동작
		return gameResult;	//계속 진행

	default:
		//잘못된 명령어를 입력했을 경우
		PrintMineField(false, initSet);
		printf("Invaild Command: Command does not exist\n");
		return gameResult;	//계속 진행
	}
	return gameResult;
}

int ProcessUserInput(InitialSetting initSet, int* init, int* visibleAreaCnt) {	//사용자의 입력, 입력값 검증, 지뢰 탐색 등의 여러 기능이 있는 함수.
	Position inputPos;	//사용자로부터 입력받을 x,y좌표와 명령어(s)
	char command;	//사용자가 입력한 명령어
	int gameResult = CONTINUE;

	//사용자 입력 처리
	printf("Enter X coordinate, Y coordinate, and instruction\n");
	scanf_s("%d %d %c", &inputPos.x, &inputPos.y, &command);
	getchar();
	if (command >= '0' && command <= '9') {
		command = command - '0';
	}
	//입력값 검증 (좌표값 범위 검사)
	if (IsOut(inputPos, initSet)) {
		PrintMineField(false, initSet);
		printf("Invaild Command: Out of range\n");
		return gameResult;	//계속 진행
	}

	//게임판 초기화
	if (*init != true)
		Initialize(initSet, init, inputPos);

	//명령어 검사 및 분기처리
	gameResult = SwitchingCommand(initSet, inputPos, visibleAreaCnt, command);
	if (gameResult != CONTINUE)
		return gameResult;

	//지뢰를 전부 찾았는지 검사
	gameResult = CheckUnSearchedMines(initSet, visibleAreaCnt);
	return gameResult;	//계속 진행 또는 승리
}

void MemoryUnallocate(InitialSetting initSet) {
	//프로그램 종료시 메모리 할당 해제 함수
	int i;
	for (i = 0; i < initSet.width; i++)
		free(areaInfo[i]);
	free(areaInfo);
	return;
}

void GetInitSetFromInput(InitialSetting *initSet) {
	double width, height, num, seed;	//잘못된 입력이 주어질 때 프로그램이 정지하지 않도록 입력을 임시로 받아줄 변수
	//초기조건 사용자 입력 처리 함수
	printf("Enter width, height, mineage, and seed(random seeds fill in -1)\n");
	scanf_s("%lf %lf %lf %lf", &width, &height, &num, &seed); // 정수형이 아닌 실수형으로 입력받습니다.
	initSet->width = (int)width; // 실수를 입력받은 경우, int로 형 변환하여 initSet.width에 저장
	initSet->height = (int)height; // 실수를 입력받은 경우, int로 형 변환하여 initSet.height에 저장
	initSet->num = (int)num; // 실수를 입력받은 경우, int로 형 변환하여 initSet.num에 저장
	initSet->seed = (int)seed; // 실수를 입력받은 경우, int로 형 변환하여 initSet.seed에 저장
	return;
}

void GetInitSetFromDefault(InitialSetting *initSet, int select) {
	double width, height, num, seed;	//잘못된 입력이 주어질 때 프로그램이 정지하지 않도록 입력을 임시로 받아줄 변수

	switch (select) {
	case 0: //EASY
		width = 10;
		height = 10;
		num = 10;
		seed = -1;
		break;
	case 1: //MEDIUM
		width = 20;
		height = 20;
		num = 40;
		seed = -1;
		break;
	case 2: //HARD
		width = 30;
		height = 20;
		num = 100;
		seed = -1;
		break;
	case 3: //EXPERT
		width = 50;
		height = 50;
		num = 500;
		seed = -1;
		break;
	default:
		break;
	}

	initSet->width = (int)width; // 실수를 입력받은 경우, int로 형 변환하여 initSet.width에 저장
	initSet->height = (int)height; // 실수를 입력받은 경우, int로 형 변환하여 initSet.height에 저장
	initSet->num = (int)num; // 실수를 입력받은 경우, int로 형 변환하여 initSet.num에 저장
	initSet->seed = (int)seed; // 실수를 입력받은 경우, int로 형 변환하여 initSet.seed에 저장
	return;
}

void MineFieldSizeException(InitialSetting *initSet) {
	// 초기조건 입력 오류시 예외처리 코드 (width, height값)
	int isWrongInput = false;	//잘못된 초기조건 입력 예외처리 플래그 (TRUE == 예외 발생 / FALSE == 정상 진행)

	do {
		if (initSet->width < 1 || initSet->height < 1)
			//지뢰판의 가로 및 세로 길이가 음수로 입력된 경우
			isWrongInput = true;
		else if (initSet->width > 99 || initSet->height > 99)
			//지뢰판의 가로 및 세로 길이가 100 이상으로 너무 크게 입력된 경우
			isWrongInput = true;
		else
			//지뢰판의 가로 및 세로 길이가 정상 입력된 경우
			isWrongInput = false;
		if (isWrongInput) {
			//잘못된 입력이 주어지면 다시 입력을 요구합니다
			printf("[ERROR]width and height must be in range 1~99. Try again.\n");
			GetInitSetFromInput(initSet);
		}
	} while (isWrongInput);
	return;
}

void MineNumException(InitialSetting *initSet) {
	// 초기조건 입력 오류시 예외처리 코드 (mineNum값)
	int isWrongInput = false;	//잘못된 초기조건 입력 예외처리 플래그 (TRUE == 예외 발생 / FALSE == 정상 진행)

	//초기조건 입력 오류시 예외처리 코드(num값)
	do {
		if (initSet->num >= initSet->width * initSet->height)
			//지뢰 갯수가 지뢰판 크기와 같거나 더 크게 입력된 경우
			isWrongInput = true;
		else if (initSet->num <= 0)
			//지뢰 갯수가 0개 또는 음수로 입력된 경우
			isWrongInput = true;
		else
			//지뢰 갯수가 정상 입력된 경우
			isWrongInput = false;
		if (isWrongInput) {
			//잘못된 입력이 주어지면 다시 입력을 요구합니다.
			printf("[ERROR]The number of mines must be in range 1 to (board size - 1). Try again.\n");
			GetInitSetFromInput(initSet);
		}
	} while (isWrongInput);
	return;
}

void SetRandSeed(InitialSetting *initSet) {
	//랜덤시드 설정 함수
	if (initSet->seed == -1)
		//사용자가 초기조건 입력시 시드값으로 -1을 입력된 경우
		initSet->seed = (int)time(NULL);
	return;
}

void PrintInitSet(InitialSetting initSet) {
	//설정된 초기설정값 출력 함수
	printf("Width: %d\n", initSet.width);
	printf("Height: %d\n", initSet.height);
	printf("Mines: %d\n", initSet.num);
	printf("Seed：%d\n", initSet.seed);
	return;
}

void InitSetFromConsoleArg(InitialSetting *initSet, int argc, char **argv) {
	//프로그램 실행 전 옵션으로 인자를 같이 주었다면 해당 인자로 초기조건 설정하는 함수
	printf("Getting information from argument\n");
	initSet->width = atoi(argv[1]);
	initSet->height = atoi(argv[2]);
	initSet->num = atoi(argv[3]);
	if (argc > 4)
		initSet->seed = atoi(argv[4]);
	else
		initSet->seed = (int)time(NULL);
	return;
}

int main(int argc, char **argv) {
	InitialSetting initSet = { 0, 0, 0, 0 };	//초기설정값 - initSet이 초기화되지 않았습니다 컴파일 오류를 피하기 위해 불가피하게 임의의 쓰레기 값을 하드코딩
	int titleSelection;
	int optionSelection;
	int visibleAreaCnt = 0;	//밝혀진 지뢰의 숫자를 0으로 초기화한다
	int isInitialized = false;	//아직 지뢰판 초기화가 되지 않았으므로 초기화 여부를 FALSE로 설정한다
	int gameResult = CONTINUE;	//게임 승패 여부를 저장

	if (argc > 3)
		//프로그램 실행 전 옵션으로 인자를 같이 주었다면 해당 인자로 초기조건 설정
		InitSetFromConsoleArg(&initSet, argc, argv);
	else {
		titleSelection = TitleScreen();

		//게임시작
		if (titleSelection == 1) {
			optionSelection = OptionScreen();

			switch (optionSelection) {
			case 0:	case 1:	case 2:	case 3:
				//기존에 지정되어있는 초기조건으로 입력받음
				GetInitSetFromDefault(&initSet, optionSelection);
				break;
			case 4:
				//별도의 인자를 받지 않았다면 초기조건 사용자 입력받음
				GetInitSetFromInput(&initSet);
				break;
			default:
				break;
			}
		}

		//사용자 입력 검증
		MineFieldSizeException(&initSet);
		MineNumException(&initSet);
	}

	//시드값 설정
	SetRandSeed(&initSet);
	srand(initSet.seed);

	//설정된 정보 다시 출력해줌
	PrintInitSet(initSet);

	//지뢰판 동적할당
	AllocNInitVar(initSet);

	//지뢰판 최초 출력
	PrintMineField(false, initSet);

	while (gameResult == CONTINUE)
		//게임결과 플래그가 진행인 동안 게임 계속 진행
		gameResult = ProcessUserInput(initSet, &isInitialized, &visibleAreaCnt);

	//프로그램 종료시 메모리 할당 해제
	MemoryUnallocate(initSet);
	return 0;
}
