#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

// Length，Column，Mines, Seed, Visible Grid, Initialized or not
AreaInfo **areaInfo; //지뢰판의 정보를 저장할 변수
char p[] = { ' ', 'O', 'X', '_' };
// Mark characters

//해당 좌표의 지뢰 유무 값 전달
int IsMine(int x, int y) {
	int result;
	result = areaInfo[x][y].isMine;
	return result;
}

//해당 좌표가 보여지는 상태에 대한 값 전달
int IsVisible(int x, int y) {
	int result;
	result = areaInfo[x][y].isVisi;
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
}

//해당 칸을 볼 수 있는지 아닌지에 대한 값 설정
void SetVisi(int x, int y, int s) {
	const int TRUE = 1; //거짓인 상태를 나타낼때 쓰일 상수로, 정수 0의 값을 가진다.
	const int FALSE = 0;  //갯수가 0일때 쓰일 상수로, 정수 0의 값을 가진다.

	if (s == TRUE) {
		areaInfo[x][y].isVisi = 1;
	}
	if (s == FALSE) {
		areaInfo[x][y].isMine = 1;
	}
}

//마크 값 설정
void SetMark(int x, int y, int s) {
	switch (s) {
	case 0:
		areaInfo[x][y].isMark = 0;
		break;

	case 1:
		areaInfo[x][y].isMark = 1;
		break;

	case 2:
		areaInfo[x][y].isMark = 2;
		break;

	case 3:
		areaInfo[x][y].isMark = 3;
		break;

	default:
		return;

	}
}

void IncNum(int x, int y) {
	areaInfo[x][y].mineNum = areaInfo[x][y].mineNum + 1;
}

void InitArea(SetInfo setInfo) {
	/*
	기능: 지뢰판을 이차원 배열로 할당하고 초기화가 필요한 구조체 멤버들을 초기화한다.
	파라미터 : len은 지뢰판의 행의 수를 col은 지뢰판의 열의 수를 나타낸다.
	*/
	const int FALSE = 0; //거짓인 상태를 나타낼때 쓰일 상수로, 정수 0의 값을 가진다.
	const int ZERO = 0;  //갯수가 0일때 쓰일 상수로, 정수 0의 값을 가진다.
	int i;
	int j;

	//변수 areaInfo에 이차원 배열을 할당한다.
	areaInfo = (AreaInfo **)malloc(setInfo.len * sizeof(AreaInfo *));

	for (i = 0; i < setInfo.len; i++) {
		areaInfo[i] = (AreaInfo *)malloc(setInfo.col * sizeof(AreaInfo));

		for (j = 0; j < setInfo.col; j++) {
			//구조체 배열의 멤버 값을 초기화한다.
			areaInfo[i][j].isMine = FALSE;
			areaInfo[i][j].isVisible = FALSE;
			areaInfo[i][j].mineNum = ZERO;
		}
	}

	return;
}

void bfs(int x, int y, int *visi) {
	if (IS_OUT(x, y) || IS_VISI(x, y))
		return;
	++(*visi);
	SET_VISI(x, y, 1);
	if (!GET_NUM(x, y)) {
		bfs(x - 1, y - 1);
		bfs(x - 1, y);
		bfs(x - 1, y + 1);
		bfs(x, y - 1);
		bfs(x, y + 1);
		bfs(x + 1, y - 1);
		bfs(x + 1, y);
		bfs(x + 1, y + 1);
	}
}

void print(int c, SetInfo setInfo) {
	int x, y;
	printf("  ");
	for (x = 0; x < setInfo.len; ++x) {
		printf("%-2d", x);
	}
	printf("\n");
	for (y = setInfo.col - 1; y >= 0; --y) {
		printf("%-2d", y);
		if (c)
			for (x = 0; x < setInfo.len; ++x) {
				if (IS_MINE(x, y))
					printf("%-2c", '*');
				else
					printf("%-2c", GET_NUM(x, y) + '0');
			}
		else
			for (x = 0; x < setInfo.len; ++x) {
				if (IS_VISI(x, y)) {
					if (IS_MINE(x, y))
						printf("%-2c", '*');
					else
						printf("%-2c", GET_NUM(x, y) + '0');
				}
				else
					printf("%-2c", p[GET_MARK(x, y)]);
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
	for (int i = 0; i < (setInfo.len)*(setInfo.col); ++i)
		m[i] |= 2;
}

int input(int *visi, int *init, SetInfo setInfo) {	//사용자의 입력, 입력값 검증, 지뢰 탐색 등의 여러 기능이 있는 함수.
	//TODO: 함수 기능 세분화(하나의 함수가 하나의 기능만 하도록 하기) 하기.
	int x, y, s;	//사용자로부터 입력받을 x,y좌표와 명령어(s)

	//사용자 입력 처리
	printf("Enter X coordinate, Y coordinate, and instruction\n");
	scanf("%d %d %d", &x, &y, &s);

	//입력값 검증 (좌표값 범위 검사)
	if (IS_OUT(x, y)) {
		print(0, setInfo);
		printf("Invaild Command: Out of range\n");
		return 0;	//게임 계속 진행
	}

	//게임판 초기화
	if (!(*init)) {
		for (int i = 0, a, b; i < setInfo.num; ++i) {
			//반복해서 랜덤한 (a, b) 좌표 생성
			a = rand() % setInfo.len;
			b = rand() % setInfo.col;
			if (IS_MINE(a, b) || (a == x && b == y))
				continue;
			else {
				//(a,b)좌표에 지뢰 매설 및 주변 8개 칸의 안내숫자 1씩 증가
				INC_NUM(a - 1, b - 1);
				INC_NUM(a - 1, b);
				INC_NUM(a - 1, b + 1);
				INC_NUM(a, b - 1);
				SET_MINE(a, b, 1);
				INC_NUM(a, b + 1);
				INC_NUM(a + 1, b - 1);
				INC_NUM(a + 1, b);
				INC_NUM(a + 1, b + 1);
			}
		}
		*init = 1;	//초기화 완료
	}

	//명령어 검사 및 분기처리
	if (s) {
		if (s == (-1)*(setInfo.seed)) {	//-seed 값을 명령어로 입력할 경우 치트 동작
			print(1, setInfo);
		}
		else if (s <= 4 && s > 0) {
			//해당 좌표에 메모하는 명령(1,2,3,4)를 입력했을 경우
			if (!IS_VISI(x, y))
				SET_MARK(x, y, s - 1);
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
		SET_MARK(x, y, 0);
		if (IS_MINE(x, y)) {	//지뢰 밟았을 경우 패배 처리
			printf("You have lost\n");
			clear();
			print(0, setInfo);
			return 1;	//패배
		}
		bfs(x, y);
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

	int visi = 0;
	int init = 0;

	SetInfo setInfo;

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
		if (setInfo.seed < 0)
			setInfo.seed = (int)time(NULL);
	}

	srand(setInfo.seed);
	printf("Length: %d\n", setInfo.len);
	printf("Column: %d\n", setInfo.col);
	printf("Mines: %d\n", setInfo.num);
	printf("Seed：%d\n", setInfo.seed);
	initArea(setInfo);
	print(0, setInfo);
	for (; !input(&visi, &init, setInfo););

	return 0;
}
