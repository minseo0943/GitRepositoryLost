#pragma warning(disable:4996)
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
#pragma comment (lib, "winmm.lib")
#include <mmsystem.h>
//헤더 파일
#include "DrawBoard.h"
#include "Cursor.h"
#include "Color.h"
#include "Stage.h"
#include "MainMenu.h"
#include "ProcessKeyInput.h"
#include "InitGameData.h"
#include "ItemData.h"
#include "PCNPC.h"
#include "Skill.h"

int scoreboard[SCOREBOARD__SIZE]; //스코어보드
memory memoryList[MEMORY_SIZE]; //기억 목록

COORD center; //게임판 중앙 좌표

void GetKeyboardInput() {
	key = 0;
	key = _getch();
}

void ShowMainMenu() {
	ClearBoard();
	textcolor(WHITE);
	PlaySound(TEXT("메인메뉴.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); //소리파일 프로젝트 폴더에 넣으면 소리 남
	//SetCurrentCursorPos(startX, startY);
	textcolor(YELLOW);
	puts("                                                     @@          @@");
	puts("                                                  (@@              @@)");
	puts("                    @                 :#            @@@@@@0@@0@@@@@@            #:                 @");
	puts("                   (##@@#:      (#@#<--(@@@@      _-@@            @@-_      @@@@)-->#@#)     :#@@##)");
	puts("                        (@@@@@@@@@         @@@@@@@                    @@@@@@@         @@@@@@@@@)\n");
	textcolor(GRAY);
	puts("                          ∏∏∏∏┓           ∏∏∏∏∏┓      ∏∏∏∏∏∏┓    ∏∏∏∏∏∏┓   ");
	puts("                          ┖ ∏∏┌┚          ∏∏┌───  ∏∏┓   ∏∏∏┌──────┚    ∏┌─∏∏┌─∏│     ");
	puts("                            ∏∏│          ∏∏┌┚       ∏∏┓  ∏∏┌┚           ┖─┚ ∏∏│ ┖─┚       ");
	puts("                            ∏∏│          ∏∏│        ∏∏│  ┖∏∏                ∏∏│      ");
	puts("                            ∏∏│          ∏∏│        ∏∏│   ┖∏∏∏∏┓          ∏∏│      ");
	puts("                            ∏∏│          ∏∏│        ∏∏│         ∏∏┓         ∏∏│        ");
	puts("                            ∏∏│          ∏∏│        ∏∏│          ∏∏┓        ∏∏│      ");
	puts("                            ∏∏│   ∏┓     ┖∏∏       ∏∏┚        ∏∏∏│        ∏∏│     ");
	puts("                          ∏∏∏∏∏∏│       ┖∏∏∏∏∏┌┚     ∏∏∏∏∏┌┚        ∏∏│       ");
	puts("                          ┖━━━━━━━━━━━┚        ┖━────────┚      ┖─────────┚         ┖───┚         ");
	textcolor(YELLOW);
	puts("            @@)               @@                      * HIT & RUN *                     @@               (@@");
	puts("           @     @@         @@@)                                                        (@@@         @@     @");
	puts("          @    _@  @       @          @@@@@@    _____@@#<<(==)>>#@@_____    @@@@@@          @       @  @_    @");
	puts("          (#@@#@       @@@@:@@@@@@@@@@  || @@@@@  | @ << (-==-) >> @ |  @@@@@ ||  @@@@@@@@@@:@@@@       @#@@#)");
	puts("               @@@@@@@@              @@@@@   |  @@@@ --<< (==) >>-- @@@@  |   @@@@@              @@@@@@@@");
	puts("                                      @   @###@@@  @@@@_        _@@@@  @@@###@   @");
	puts("                                                       @-  ()  -@");
	puts("                                                        @------@");
	puts("                                                         @@__@@\n\n\n");
	textcolor(WHITE);
	puts("                                                     1. Game Start \n");
	puts("                                                     2. Scoreboard \n");
	puts("                                                     3. Memory \n");
	puts("                                                     4. How to Play \n");
	puts("                                                     5. Exit \n");

	//메뉴 선택
	while (1) {
		GetKeyboardInput();
		switch (key)
		{
		default:
			break;
		case ONE:
			StartGame();
			break;
		case TWO:
			DisplayScoreBoard();
			break;
		case THREE:
			DisplayMemory();
			break;
		case FOUR:
			DisplayGameRule();
			break;
		case FIVE:
			ExitGame();
			break;
		}
	}
}

void StartGame() {
	//게임판 초기화
	ClearBoard();
	stage = 1;
	//게임 시작 전 데이터 초기화
	initData_Variable();
	initData_SkillEffectInfo();
	initData_SelectedSkill();
	
	//난이도 초기화
	initDifficulty(12, 20, 20, 10, 10);

	//HP 초기화
	initHP(5);
	initBossHP(100); 

	//스테이지 호출
	for (int i = 1; i <= 3; i++) {
		//스테이지 시작 전 데이터 초기화
		initData_Time();
		initData_GameBoardInfo();

		//스테이지 시작
		StartStage(i);
	}
}

void DisplayScoreBoard() {
	int startX = 6, startY = 4;
	int i;

	ClearBoard();
	SetCurrentCursorPos(2, 1);
	puts("ESC) 돌아가기");

	//스코어보드 출력
	SetCurrentCursorPos(startX + 1, startY);
	puts("**********************");
	startY++;
	SetCurrentCursorPos(startX + 1, startY);
	puts("***** 스코어보드 *****");
	startY++;
	SetCurrentCursorPos(startX + 1, startY);
	puts("**********************");
	startY++;
	SetCurrentCursorPos(startX, startY);
	puts("┌───────┬─────────────┐");
	startY++;
	SetCurrentCursorPos(startX, startY);
	puts("│  순위 │    스코어   │");

	for (i = 1; i <= 10; i++) {
		if (i == 1) {
			SetCurrentCursorPos(startX, startY + i * 2 - 1);
			puts("├───────┼─────────────┤");
		}
		else {
			SetCurrentCursorPos(startX, startY + i * 2 - 1);
			puts("│       │             │");
		}
		SetCurrentCursorPos(startX, startY + i * 2);
		printf("│   %2d  │   %-10d│", i, scoreboard[i - 1]);
	}
	SetCurrentCursorPos(startX, startY + i * 2 - 1);
	puts("└───────┴─────────────┘");

	//돌아가기
	while (1) {
		GetKeyboardInput();
		if (key == ESC) ShowMainMenu();
	}
}

void DisplayMemory() {
	int startX = 6, startY = 4;
	int i;

	ClearBoard();
	SetCurrentCursorPos(2, 1);
	puts("ESC) 돌아가기");

	//기억 목록 출력
	//스코어보드 출력
	SetCurrentCursorPos(startX + 1, startY);
	puts("******************************************************************************");
	startY++;
	SetCurrentCursorPos(startX + 1, startY);
	puts("********************************** 기억목록 **********************************");
	startY++;
	SetCurrentCursorPos(startX + 1, startY);
	puts("******************************************************************************");

	for (i = 1; i <= MEMORY_SIZE; i++) {
		if (i == 1) {
			textcolor(WHITE);
			SetCurrentCursorPos(startX, startY + i * 2 - 1);
			puts("┌─────────────────────────────────────────────────────────────────────────────┐");
		}
		else {
			textcolor(WHITE);
			SetCurrentCursorPos(startX, startY + i * 2 - 1);
			puts("├─────────────────────────────────────────────────────────────────────────────┤");
		}
		SetCurrentCursorPos(startX, startY + i * 2);
		if (memoryList[i - 1].check) {
			textcolor(WHITE);
			printf("│");
			textcolor(YELLOW);
			printf("    %-70s   ", memoryList[i - 1].unlocked);
			textcolor(WHITE);
			printf("│");
		}
		else {
			textcolor(WHITE);
			printf("│");
			textcolor(darkGRAY);
			printf("    %-70s   ", memoryList[i - 1].locked);
			textcolor(WHITE);
			printf("│");
		}
	}
	textcolor(WHITE);
	SetCurrentCursorPos(startX, startY + i * 2 - 1);
	puts("└─────────────────────────────────────────────────────────────────────────────┘");

	//돌아가기
	while (1) {
		GetKeyboardInput();
		if (key == ESC) ShowMainMenu();
	}
}

void DisplayGameRule() {
	int startX = 6, startY = 4;

	ClearBoard();
	SetCurrentCursorPos(2, 1);
	puts("ESC) 돌아가기");
	SetCurrentCursorPos(startX, startY);
	puts("◆ ↑ ↓ ← → 키로 움직일 수 있습니다.");
	SetCurrentCursorPos(startX, startY + 2);
	puts("◇ 공격 속도에 따라 자동으로 공격합니다.");
	SetCurrentCursorPos(startX, startY + 4);
	puts("◆ 적을 처치하면 경험치를 얻고 일정 확률로 아이템을 떨굽니다.");
	SetCurrentCursorPos(startX, startY + 6);
	printf("◇ 아이템 ");
	textcolor(RED);
	printf("♥");
	textcolor(WHITE);
	printf("를 획득하면 HP를 1 회복, ");
	textcolor(darkGRAY);
	printf("♣");
	textcolor(WHITE);
	printf("를 획득하면 모든 적을 처치합니다.");
	SetCurrentCursorPos(startX, startY + 8);
	puts("◆ 일정 이상 경험치를 획득하면 새로운 스킬을 획득할 수 있습니다.");
	SetCurrentCursorPos(startX, startY + 10);
	puts("◇ 보스를 처치하면 Game Clear, 그 전에 HP가 모두 소진되면 Game Over입니다.");

	while (1) {
		GetKeyboardInput();
		if (key == ESC) ShowMainMenu();
	}
}

void ExitGame() {
	ClearBoard();
	SetCurrentCursorPos(7, 4);
	puts("*** 게임을 종료합니다. ***\n\n\n");
	exit(1);
}
