#pragma warning(disable: 4996)
#include "print.h"

extern AreaInfo **areaInfo;
extern InitialSetting initSet;


void PrintMineField(int isCheatEnable) {
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

void PrintInitSet(void) {
	//설정된 초기설정값 출력 함수
	printf(" > Width  : %d\n", initSet.width);
	printf(" > Height : %d\n", initSet.height);
	printf(" > Mines  : %d\n", initSet.num);
	printf(" > Seed   : %d\n\n", initSet.seed);
	return;
}

void PrintRow(void) {
	//페이지 구분을 위한 줄
	printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
}

void WipeScreen(void) {
	//콘솔화면을 갱신하는 함수입니다.
	//ifdef의 사용이 필요하여 불가피하게 전처리기를 사용하였습니다.
#ifdef _WIN32   //윈도우의 경우 cls 명령을 사용합니다.
	system("cls");
#else   //리눅스의 경우 clear 명령을 사용합니다.
	system("clear");
#endif
	return;
}

void ProcessGameResult(int result) {
	//게임 승리 또는 패배시의 작업을 처리하는 함수
	switch (result) {
	case 1: //게임 패배
		WipeScreen();
		Gameover();
		PrintMineField(false);
		printf("\n");
		printf("   ≪ You lose ! ≫\n");

		return;
	case 2: //게임 승리
		WipeScreen();
		Gameover();
		PrintMineField(false);
		printf("\n");
		printf("   ≪ You win ! ≫\n");
		return;
	default:        //게임 계속 진행
		return;
	}
}

void HelpScreen(void) {
	//도움말 화면으로, 기본 조작 방법을 설명한다.
	char anyKey;
	printf("              How To Play\n\n");
	printf("            ⑴ Main Screen\n");
	printf("            If you want to play this game, press '1' and 'enter'\n");
	printf("            If you want to know how to play this game, press '2' and 'enter'\n");
	printf("            If you want to quit, press '3' and 'enter'\n\n");
	printf("            ⑵ Option Screen\n");
	printf("            NOTE: If you give some options before run this program,\n");
	printf("            Title and Mine Screen will not appear, and the program will show Game Screen directly\n");
	printf("            (Game will be Pre - Initialized with the Console - Option you given)\n\n");
	printf("            If you want to play EASY level, press '1' and 'enter'\n");
	printf("            If you want to play MEDIUM level, press '2' and 'enter'\n");
	printf("            If you want to play HARD level, press '3' and 'enter'\n");
	printf("            If you want to play EXPERT level, press '4' and 'enter'\n");
	printf("            If you want to play with your own setting, press '5' and 'enter'\n\n");
	printf("            ⑶ Game Screen\n");
	printf("            Input Format : [coordinate X], [coordinate Y], [Command]\n");
	printf("            [coordinate X], [coordinate Y] is the position which you want to apply the command\n");
	printf("            If you want to dig the coordinate, use command '0'\n");
	printf("            If you want to remove MEMO which is marked at the coordinate, use command '1'\n");
	printf("            If you want to MEMO 'S' at the coordinate, use command '2'\n");
	printf("            If you want to MEMO 'X' at the coordinate, use command '3'\n");
	printf("            If you want to MEMO '?' at the coordinate, use command '4'\n");
	printf("            If you want to use CHEAT, use command '9'\n\n");
	printf("            PRESS ANY KEY TO RETURN TO TITLE\n");
	getchar();      //옵션 선택시 입력된 버퍼에 남아있는 개행문자 제거
	scanf_s("%c", &anyKey, 1);
	WipeScreen();
	return;
}

bool TitleScreen(void) {
	//타이틀 화면으로 게임 시작, 도움말, 게임 종료 중 하나의 메뉴를 선택할 수 있다.
	bool willStart = false;
	int select = 1;

	printf("    ┏━━━━━━  CLS MineSweeper ━━━━━━┓\n");
	printf("    ┃                              ┃\n");
	printf("    ┃                              ┃\n");
	printf("    ┃           ① PLAY            ┃\n");
	printf("    ┃           ② HELP            ┃\n");
	printf("    ┃           ③ EXIT            ┃\n");
	printf("    ┃                              ┃\n");
	printf("    ┃                              ┃\n");
	printf("    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");

	do {
		printf("       Select number to continue : ");
		scanf_s("%d", &select);

		if (select != 1 && select != 2 && select != 3) {
			printf("\n        |  ※ Wrong input ※ |\n");
			printf("          | Please enter again |\n\n");
		}
	} while (select != 1 && select != 2 && select != 3);

	PrintRow();

	switch (select) {

		//게임을 시작합니다
	case 1:
		WipeScreen();
		willStart = true;
		return willStart;

		//도움말 화면으로 이동합니다
	case 2:
		WipeScreen();
		HelpScreen();
		return willStart;

		//게임을 종료합니다
	case 3:
		WipeScreen();
		exit(0);

	default:
		WipeScreen();
		return willStart;
	}
}

int OptionScreen(void) {
	//옵션 화면으로, 게임의 난이도를 선택할 수 있다.
	int select;

	printf("              STAGE LEVEL\n\n");
	printf("            ⑴ EASY\n");
	printf("            ⑵ MEDIUM\n");
	printf("            ⑶ HARD\n");
	printf("            ⑷ EXPERT\n");
	printf("            ⑸ CUSTOM LEVEL\n\n");

	do {
		printf("       Select number to continue :  ");
		scanf_s("%d", &select);

		if (select != 1 && select != 2 && select != 3 && select != 4 && select != 5) {
			printf("\n        |  ※ Wrong input ※ |\n");
			printf("          | Please enter again |\n\n");
		}
	} while (select != 1 && select != 2 && select != 3 && select != 4 && select != 5);

	PrintRow();

	//0 부터 4 까지 차례로 EASY, MEDIUM, HARD, EXPERT, CUSTOM을 뜻함
	switch (select - 1) {
	case 0:
		WipeScreen();
		return 0;
	case 1:
		WipeScreen();
		return 1;
	case 2:
		WipeScreen();
		return 2;
	case 3:
		WipeScreen();
		return 3;
	case 4:
		WipeScreen();
		return 4;
	default:
		WipeScreen();
		return 0;
	}
}
