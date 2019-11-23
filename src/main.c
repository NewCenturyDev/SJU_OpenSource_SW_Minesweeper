#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* 구조체 정의 */
typedef struct AreaInfo { //지뢰판의 구역의 정보를 다루는 구조체
	int isMine;       //지뢰의 유무를 저장하는 변수
	int isVisible;    //해당 칸이 보여지는 상태를 저장하는 변수
	int mark;         //해당 칸의 표식이 무엇인지 저장하는 변수
	int mineNum;      //해당 칸의 주변 지뢰의 수를 저장하는 변수
}AreaInfo;

typedef struct SetInfo { // 게임 시작시에 설정하는 맵의 길이, 높이, 지뢰의 갯수, 시드를 저장하는 구조체
	int len;          // 맵의 길이를 저장하는 변수
	int col;		  // 맵의 높이를 저장하는 변수
	int num;          // 지뢰의 갯수를 저장하는 변수
	int seed;         // 시드를 저장하는 변수
}SetInfo;

typedef struct Pos { //계산 혹은 함수값 리턴시 x, y의 값을 한번에 다루기 위한 구조체
	int posX;
	int posY;
}Pos;

typedef struct StackNode { //스택 구조를 사용하기 위한 구조체
	Pos ele;
	struct StackNode *next;
}StackNode;

/* 전역 변수 선언 */
//부울 연산과 정수 연산을 시각적으로 구분하기 위한 상수
//코드 여러 군데에서 빈번하게 사용되기 때문에 전역 변수로 선언하였습니다
//C 문법만 사용 옵션(C++ 문법 제외)을 넣은 Visual Studio 상에서 true, false, bool, boolean 키워드가 작동하지 않아 선언했습니다.
const int TRUE = 1;	//참인 상태를 나타낼때 쓰일 상수로, 정수 1의 값을 가진다.
const int FALSE = 0;	//거짓인 상태를 나타낼때 쓰일 상수로, 정수 0의 값을 가진다.
AreaInfo **areaInfo = NULL; //지뢰판의 정보를 저장할 변수

/* 함수 원형 선언 */
//todo: 맨 마지막에 작업할 예정입니다. (디버깅시 혼동 방지를 위해)

//해당 좌표의 지뢰 유무 값 전달
int IsHereMine(int x, int y) {
	int result;
	result = areaInfo[x][y].isMine;
	return result;
}

//해당 좌표가 보여지는 상태에 대한 값 전달
int IsVisible(int x, int y) {
	int result;
	result = areaInfo[x][y].isVisible;
	return result;
}

//해당 좌표의 표식의 값 전달
int Mark(int x, int y) {
	int result;
	result = areaInfo[x][y].mark;
	return result;
}

//해당 칸 주변 지뢰의 수 전달
int MineNum(int x, int y) {
	int result;
	result = areaInfo[x][y].mineNum;
	return result;
}

//입력된 좌표가 범위 밖인지 판단
int IsOut(int x, int y, SetInfo setInfo) {
	const int OUT = 1; //입력 좌표가 범위 밖임을 나타내는 상수, 정수 1의 값을 가진다.
	const int IN = 0; //입력 좌표가 범위 안에 있음을 나타내는 상수, 정수 0의 값을 가진다.

	if (x >= setInfo.len || y >= setInfo.col || x < 0 || y < 0) {
		return OUT;
	}
	return IN;
}

void SetMine(int x, int y) {
	areaInfo[x][y].isMine = 1;
	return;
}

//해당 칸을 볼 수 있는지 아닌지에 대한 값 설정
void SetVisible(int x, int y, int s) {
	if (s == TRUE) {
		areaInfo[x][y].isVisible = 1;
	}
	if (s == FALSE) {
		areaInfo[x][y].isVisible = 0;
	}
	return;
}

//마크 값 설정
void SetMark(int x, int y, int s) {
	switch (s) {
	case 0:
		areaInfo[x][y].mark = 0;
		break;

	case 1:
		areaInfo[x][y].mark = 1;
		break;

	case 2:
		areaInfo[x][y].mark = 2;
		break;

	case 3:
		areaInfo[x][y].mark = 3;
		break;

	default:
		return;
	}
}

void IncNum(int x, int y, SetInfo setInfo) {
	if (IsOut(x, y, setInfo))
		return;
	else
		areaInfo[x][y].mineNum = areaInfo[x][y].mineNum + 1;
	return;
}

void InitArea(SetInfo setInfo) {
	/*
	기능: 지뢰판을 이차원 배열로 할당하고 초기화가 필요한 구조체 멤버들을 초기화한다.
	파라미터 : len은 지뢰판의 행의 수를 col은 지뢰판의 열의 수를 나타낸다.
	*/
	const int ZERO = 0;  //갯수가 0일때 쓰일 상수로, 정수 0의 값을 가진다.
	int i;
	int j;

	//변수 areaInfo에 이차원 배열을 할당한다.
	areaInfo = (AreaInfo **)malloc(setInfo.len * sizeof(AreaInfo *));
	if (areaInfo == NULL) { // 동적 할당에 실패한 경우 아래 문구와 함께 종료된다.
		printf("Not enough memory!");
		return;
	}
	for (i = 0; i < setInfo.len; i++) {
		areaInfo[i] = (AreaInfo *)malloc(setInfo.col * sizeof(AreaInfo)); // 동적 할당에 실패한 경우 아래 문구와 함께 종료된다.
		if (areaInfo[i] == NULL) {
			printf("Not enough memory!");
			return;
		}
	}

	//구조체 배열의 멤버 값을 초기화한다.
	for (i = 0; i < setInfo.len; i++) {
		for (j = 0; j < setInfo.col; j++) {
			areaInfo[i][j].isMine = FALSE;
			areaInfo[i][j].isVisible = FALSE;
			areaInfo[i][j].mark = ZERO;
			areaInfo[i][j].mineNum = ZERO;
		}
	}
	return;
}

StackNode *Push(StackNode *stack, Pos pos) { //스택에 푸시하는 함수
	StackNode *newNode; //새로 생성한 노드
	StackNode *pushNode; //푸시할 위치의 전 노드

	newNode = (StackNode *)malloc(sizeof(StackNode)); //노드에 메모리를 할당
	(newNode->ele).posX = pos.posX; //새로 생성한 노드에 푸시하고자하는 위치값을 저장
	(newNode->ele).posY = pos.posY;
	newNode->next = NULL; //새로 생성한 노드의 다음은 널 포인터를 가리켜 마지막을 알 수 있게함

	if (stack == NULL) //스택이 비어있다면 새로 생성한 노드를 리턴
		return newNode;

	pushNode = stack; //스택이 비어있지않다면 현재의 마지막 노드로 이동하여 새로 생성한 노드를 마지막에 오도록 추가
	while (pushNode->next != NULL)
		pushNode = pushNode->next;
	pushNode->next = newNode;

	return stack; //스택을 리턴
}

Pos Pop(StackNode **stack) { //스택의 노드를 팝하는 함수
	Pos e; //팝할 노드의 원소를 담을 변수
	StackNode *popNode; //메모리 해제를 하기 위해 팝할 노드를 가리킬 변수

	e = (*stack)->ele; //팝할 노드의 원소와 노드를 저장
	popNode = *stack;

	*stack = (*stack)->next; //스택이 스택의 다음 노드를 가리키게 함

	free(popNode); //팝한 노드 메모리 할당 해제
	return e; //팝한 노드의 원소를 리턴
}

int IsEmptyStack(StackNode *stack) { //스택이 비어있는지 확인하는 함수
	if (stack == NULL) //스택이 비어있다면 TRUE를 리턴
		return TRUE;
	return FALSE; //스택이 비어있지않다면 FALSE를 리턴
}

void SetAdjecantPosition(Pos *adjPos, Pos pos) { //x, y 주변의 인접한 8칸의 좌표들을 배열에 옮겨담는 함수
	adjPos[0].posX = pos.posX; //기존의 좌표에서 북쪽에 있는 좌표 
	adjPos[0].posY = pos.posY + 1;

	adjPos[1].posX = pos.posX + 1; //북동쪽에 있는 좌표
	adjPos[1].posY = pos.posY + 1;

	adjPos[2].posX = pos.posX + 1; //동쪽에 있는 좌표
	adjPos[2].posY = pos.posY;

	adjPos[3].posX = pos.posX + 1; //남동쪽에 있는 좌표
	adjPos[3].posY = pos.posY - 1;

	adjPos[4].posX = pos.posX; //남쪽에 있는 좌표
	adjPos[4].posY = pos.posY - 1;

	adjPos[5].posX = pos.posX - 1; //남서쪽에 있는 좌표
	adjPos[5].posY = pos.posY - 1;

	adjPos[6].posX = pos.posX - 1; //서쪽에 있는 좌표
	adjPos[6].posY = pos.posY;

	adjPos[7].posX = pos.posX - 1; //북서쪽에 있는 좌표
	adjPos[7].posY = pos.posY + 1;

	return;
}

void bfs(SetInfo setInfo, int x, int y, int* visi) {
	const int adjNum = 8; //인접한 칸의 최대 갯수
	StackNode *stack = NULL; //스택을 가리킬 헤드 포인터
	Pos adjPos[8]; //스택에서 팝한 좌표의 주변 8칸의 좌표값을 저장할 배열
	Pos pos; //팝한 노드의 좌표를 저장할 변수

	if (IsOut(x, y, setInfo) || IsVisible(x, y)) //선택한 좌표가 보이는 상태이거나 범위 밖의 좌표이면 리턴
		return;

	pos.posX = x; //선택한 노드의 x, y 좌표를 구조체 변수에 저장
	pos.posY = y;
	stack = Push(stack, pos); //스택에 푸시
	SetAdjecantPosition(adjPos, pos); //인접한 8칸의 좌표를 배열에 저장

	while (!IsEmptyStack(stack)) { //스택이 비어있지않다면 시행
		pos = Pop(&stack); //스택의 노드를 팝하고 그 원소를 저장

		if (!(IsOut(pos.posX, pos.posY, setInfo) || IsVisible(pos.posX, pos.posY))) { //팝한 좌표가 보이는 상태와 범위 밖의 좌표가 아닐 경우 실행
			++(*visi); //보이는 수를 증가시키고 해당 좌표의 칸을 보이는 상태로 변경
			SetVisible(pos.posX, pos.posY, 1);

			if (!(areaInfo[pos.posX][pos.posY].mineNum)) { //주변의 지뢰가 매설되어 있지 않은 경우 실행
				SetAdjecantPosition(adjPos, pos); //주변 8칸의 좌표를 배열에 저장
				for (int i = 0; i < adjNum; i++) { //8칸의 좌표를 스택에 푸시
					stack = Push(stack, adjPos[i]);
				}
			}
		}
	}

	return;
}

void print(int c, SetInfo setInfo) {
	char p[] = { ' ', 'S', 'X', '?' }; //  기존의 'O', '_'를 각각 'S', '?'로 변경하였습니다.
	int x, y;
	char mineNumToChar;

	printf("  ");
	for (x = 0; x < setInfo.len; ++x) {
		printf("%-2d", x);
	}
	printf("\n");
	for (y = setInfo.col - 1; y >= 0; --y) {
		printf("%-2d", y);
		if (c)
			for (x = 0; x < setInfo.len; ++x) {
				if (IsHereMine(x, y))
					printf("%-2c", '*');
				else {
					mineNumToChar = (char)areaInfo[x][y].mineNum + '0';
					printf("%-2c", mineNumToChar);
				}
			}
		else
			for (x = 0; x < setInfo.len; ++x) {
				if (IsVisible(x, y)) {
					if (IsHereMine(x, y))
						printf("%-2c", '*');
					else {
						mineNumToChar = (char)areaInfo[x][y].mineNum + '0';
						printf("%-2c", mineNumToChar);
					}

				}
				else
					printf("%-2c", p[Mark(x, y)]);
			}
		printf("%-2d", y);
		printf("\n");
	}
	printf("  ");
	for (x = 0; x < setInfo.len; ++x) {
		printf("%-2d", x);
	}
	printf("\n");
	if (c)
		printf("WARNING: CHAETING DETECTED\n");
}

void Gameover(SetInfo setInfo) {
	int i, j;
	for (i = 0; i < setInfo.len; ++i) {
		for (j = 0; j < (setInfo.col); j++) {
			areaInfo[i][j].isVisible = 1;
		}
	}
	return;
}

void Initialize(SetInfo setInfo, int* init, int x, int y) {
	/* 함수 설명 */
	//지뢰판을 초기화하기 위한 함수입니다.
	int leftMineToInstall = setInfo.num;
	int a, b, i;	//랜덤으로 선정된 x,y좌표쌍 (a,b)와 for문 반복자

	for (i = 0; i < leftMineToInstall; ++i) {
		//반복해서 랜덤한 (a, b) 좌표 생성
		a = rand() % setInfo.len;
		b = rand() % setInfo.col;
		if (IsHereMine(a, b) || (a == x && b == y)) {
			leftMineToInstall++;	//이번 반복횟수를 무효로 하기 위해 1 증가시켜야 함. (continue 하더라도 i는 1 증가해버림. 이를 되돌려주기 위한 조치)
			continue;
		}
		else {
			//(a,b)좌표에 지뢰 매설 및 주변 8개 칸의 안내숫자 1씩 증가
			IncNum(a - 1, b - 1, setInfo);
			IncNum(a - 1, b, setInfo);
			IncNum(a - 1, b + 1, setInfo);
			IncNum(a, b - 1, setInfo);
			SetMine(a, b);
			IncNum(a, b + 1, setInfo);
			IncNum(a + 1, b - 1, setInfo);
			IncNum(a + 1, b, setInfo);
			IncNum(a + 1, b + 1, setInfo);
		}
	}
	*init = TRUE;	//초기화 완료
}

int input(SetInfo setInfo, int* init, int* visi) {	//사용자의 입력, 입력값 검증, 지뢰 탐색 등의 여러 기능이 있는 함수.
	//TODO: 함수 기능 세분화(하나의 함수가 하나의 기능만 하도록 하기) 하기.
	int x, y, s;	//사용자로부터 입력받을 x,y좌표와 명령어(s)

	//사용자 입력 처리
	printf("Enter X coordinate, Y coordinate, and instruction\n");
	scanf("%d %d %d", &x, &y, &s);

	//입력값 검증 (좌표값 범위 검사)
	if (IsOut(x, y, setInfo)) {
		print(0, setInfo);
		printf("Invaild Command: Out of range\n");
		return 0;	//게임 계속 진행
	}

	//게임판 초기화
	if (*init != TRUE) {
		Initialize(setInfo, init, x, y);
	}

	//명령어 검사 및 분기처리
	if (s) {
		if (s == (-1)*(setInfo.seed)) {	//-seed 값을 명령어로 입력할 경우 치트 동작
			print(1, setInfo);
		}
		else if (s <= 4 && s > 0) {
			//해당 좌표에 메모하는 명령(1,2,3,4)를 입력했을 경우
			if (!IsVisible(x, y))
				SetMark(x, y, s - 1);
			else {
				print(0, setInfo);
				printf("Invaild Command: Already visible\n");
				return 0;	//게임 계속 진행
			}
			print(0, setInfo);
		}
		else {
			//잘못된 명령어를 입력했을 경우
			print(0, setInfo);
			printf("Invaild Command: Command does not exist\n");
		}
	}
	else {
		//지뢰를 파보는 명령(0)을 입력했을 경우
		SetMark(x, y, 0);
		if (IsHereMine(x, y)) {	//지뢰 밟았을 경우 패배 처리
			printf("You have lost\n");
			Gameover(setInfo);
			print(0, setInfo);
			return 1;	//패배
		}
		bfs(setInfo, x, y, visi);
		print(0, setInfo);
	}
	if (*visi == (setInfo.len) * (setInfo.col) - (setInfo.num)) {	//남은 지뢰 갯수 검사
		//다 찾았을 경우 승리 처리
		printf("You win\n");
		Gameover(setInfo);
		print(0, setInfo);
		return 2;	//승리
	}
	return 0;	//게임 계속 진행
}

int main(int argc, char **argv) {
	SetInfo setInfo;
	int visi = 0;	//밝혀진 지뢰의 숫자를 0으로 초기화한다
	int init = FALSE;	//아직 지뢰판 초기화가 되지 않았으므로 초기화 여부를 FALSE로 설정한다
	int i = 0;

	if (argc > 3) {
		printf("Getting information from argument\n");
		setInfo.len = atoi(argv[1]);
		setInfo.col = atoi(argv[2]);
		setInfo.num = atoi(argv[3]);
		if (argc > 4)
			setInfo.seed = atoi(argv[4]);
		else setInfo.seed = (int)time(NULL);
	}
	else {
		printf("Enter length, width, mineage, and seed(random seeds fill in -1)\n");
		scanf("%d %d %d %d", &setInfo.len, &setInfo.col, &setInfo.num, &setInfo.seed);

		while (setInfo.num > setInfo.len * setInfo.col || setInfo.num < 0) {
			if (setInfo.num > setInfo.len * setInfo.col) {
				printf("The number of mines you entered is larger than the size of the entire map. Try again.\n");
				printf("Enter length, width, mineage, and seed(random seeds fill in -1)\n");
				scanf("%d %d %d %d", &setInfo.len, &setInfo.col, &setInfo.num, &setInfo.seed);
			}

			if (setInfo.num < 0) {
				printf("The number of mines you entered is negative. Try again.\n");
				printf("Enter length, width, mineage, and seed again(random seeds fill in -1)\n");
				scanf("%d %d %d %d", &setInfo.len, &setInfo.col, &setInfo.num, &setInfo.seed);
			}
		}
	
		if (setInfo.seed < 0)
			setInfo.seed = (int)time(NULL);
	}

	srand(setInfo.seed);
	printf("Length: %d\n", setInfo.len);
	printf("Column: %d\n", setInfo.col);
	printf("Mines: %d\n", setInfo.num);
	printf("Seed：%d\n", setInfo.seed);
	InitArea(setInfo);
	print(0, setInfo);
	for (; !input(setInfo, &init, &visi););

	for (i = 0; i < setInfo.len; i++) {
		free(areaInfo[i]);
	}

	free(areaInfo);
	return 0;
}
