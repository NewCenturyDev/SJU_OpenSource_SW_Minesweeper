#include <stdlib.h>
#include <time.h>
#include "init.h"

extern AreaInfo **areaInfo;
extern InitialSetting initSet;

void Initialize(int* init, Position inputPos) {
	/* 함수 설명 */
	//지뢰판을 초기화하기 위한 함수입니다.
	Position randPos;               //랜덤으로 선정된 x, y좌표
	int leftMineToInstall = initSet.num;
	int i;

	for (i = 0; i < leftMineToInstall; ++i) {
		//반복해서 랜덤한 (a, b) 좌표 생성
		randPos.x = rand() % initSet.width;
		randPos.y = rand() % initSet.height;
		if (IsHereMine(randPos) || randPos.x == inputPos.x || randPos.y == inputPos.y) {
			leftMineToInstall++;    //이번 반복횟수를 무효로 하기 위해 1 증가시켜야 함. (continue 하더라도 i는 1 증가해버림. 이를 되돌려주기 위한 조치)
			continue;
		}
		else {
			//(a,b)좌표에 지뢰 매설 및 주변 8개 칸의 안내숫자 1씩 증가
			IncNum(randPos.x - 1, randPos.y - 1);
			IncNum(randPos.x - 1, randPos.y);
			IncNum(randPos.x - 1, randPos.y + 1);
			IncNum(randPos.x, randPos.y - 1);
			SetMine(randPos);
			IncNum(randPos.x, randPos.y + 1);
			IncNum(randPos.x + 1, randPos.y - 1);
			IncNum(randPos.x + 1, randPos.y);
			IncNum(randPos.x + 1, randPos.y + 1);
		}
	}
	*init = true;
	return;
}

void IncNum(int x, int y) {
	//안내숫자 갱신 함수
	//주변 8방향에 대해 반복 호출시 pos 구조체의 빈번한 갱신에 대한 불편함이 있어 불가피하게 pos 구조체 미사용
	Position pos;
	pos.x = x;
	pos.y = y;
	if (IsOut(pos))
		return;
	else
		areaInfo[pos.x][pos.y].mineNum = areaInfo[pos.x][pos.y].mineNum + 1;
	return;
}

void InitSetFromConsoleArg(int argc, char **argv) {
	//프로그램 실행 전 옵션으로 인자를 같이 주었다면 해당 인자로 초기조건 설정하는 함수
	printf("Getting information from argument\n");
	initSet.width = atoi(argv[1]);
	initSet.height = atoi(argv[2]);
	initSet.num = atoi(argv[3]);
	if (argc > 4)
		initSet.seed = atoi(argv[4]);
	else
		initSet.seed = (int)time(NULL);
	return;
}

void GetInitSetFromInput(void) {
	//잘못된 입력이 주어질 때 프로그램이 정지하지 않도록 입력을 임시로 받아줄 변수
	double width;
	double height;
	double num;
	double seed;
	//초기조건 사용자 입력 처리 함수
	printf("Enter width, height, mineage, and seed(random seeds fill in -1)\n");
	scanf_s("%lf %lf %lf %lf", &width, &height, &num, &seed); // 정수형이 아닌 실수형으로 입력받습니다.
	initSet.width = (int)width; // 실수를 입력받은 경우, int로 형 변환하여 initSet.width에 저장
	initSet.height = (int)height; // 실수를 입력받은 경우, int로 형 변환하여 initSet.height에 저장
	initSet.num = (int)num; // 실수를 입력받은 경우, int로 형 변환하여 initSet.num에 저장
	initSet.seed = (int)seed; // 실수를 입력받은 경우, int로 형 변환하여 initSet.seed에 저장
	return;
}

void GetInitSetFromDefault(int select) {
	//미리 지정된 기본 설정값 중 하나를 골라서 플레이할 경우 여기 있는 기본값으로 초기화합니다.
	//하드코딩된 상수들은 윈도우 지뢰찾기의 초급, 중급, 고급, 최고급 난이도의 초기설정값을 그대로 준용했습니다.
	switch (select) {
	case 0: //EASY
		initSet.width = 10;
		initSet.height = 10;
		initSet.num = 10;
		initSet.seed = -1;
		break;
	case 1: //MEDIUM
		initSet.width = 20;
		initSet.height = 20;
		initSet.num = 40;
		initSet.seed = -1;
		break;
	case 2: //HARD
		initSet.width = 30;
		initSet.height = 20;
		initSet.num = 100;
		initSet.seed = -1;
		break;
	case 3: //EXPERT
		initSet.width = 50;
		initSet.height = 50;
		initSet.num = 500;
		initSet.seed = -1;
		break;
	default:
		break;
	}
	return;
}

void SetRandSeed(void) {
	//랜덤시드 설정 함수
	if (initSet.seed == -1)
		//사용자가 초기조건 입력시 시드값으로 -1을 입력된 경우
		initSet.seed = (int)time(NULL);
	return;
}

void AllocNInitVar(void) {
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
