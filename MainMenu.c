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
//��� ����
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

int scoreboard[SCOREBOARD__SIZE]; //���ھ��
memory memoryList[MEMORY_SIZE]; //��� ���

COORD center; //������ �߾� ��ǥ

void GetKeyboardInput() {
	key = 0;
	key = _getch();
}

void ShowMainMenu() {
	ClearBoard();
	textcolor(WHITE);
	PlaySound(TEXT("���θ޴�.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); //�Ҹ����� ������Ʈ ������ ������ �Ҹ� ��
	//SetCurrentCursorPos(startX, startY);
	textcolor(YELLOW);
	puts("                                                     @@          @@");
	puts("                                                  (@@              @@)");
	puts("                    @                 :#            @@@@@@0@@0@@@@@@            #:                 @");
	puts("                   (##@@#:      (#@#<--(@@@@      _-@@            @@-_      @@@@)-->#@#)     :#@@##)");
	puts("                        (@@@@@@@@@         @@@@@@@                    @@@@@@@         @@@@@@@@@)\n");
	textcolor(GRAY);
	puts("                          ����������           ������������      ��������������    ��������������   ");
	puts("                          �� ��������          ������������  ������   ����������������������    ������������������     ");
	puts("                            ������          ��������       ������  ��������           �Ŧ��� ������ �Ŧ���       ");
	puts("                            ������          ������        ������  �Ţ���                ������      ");
	puts("                            ������          ������        ������   �Ţ���������          ������      ");
	puts("                            ������          ������        ������         ������         ������        ");
	puts("                            ������          ������        ������          ������        ������      ");
	puts("                            ������   ����     �Ţ���       ������        ��������        ������     ");
	puts("                          ��������������       �Ţ�������������     ��������������        ������       ");
	puts("                          �Ŧ�����������������������        �Ŧ�������������������      �Ŧ�������������������         �Ŧ�������         ");
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

	//�޴� ����
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
	//������ �ʱ�ȭ
	ClearBoard();
	stage = 1;
	//���� ���� �� ������ �ʱ�ȭ
	initData_Variable();
	initData_SkillEffectInfo();
	initData_SelectedSkill();
	
	//���̵� �ʱ�ȭ
	initDifficulty(12, 20, 20, 10, 10);

	//HP �ʱ�ȭ
	initHP(5);
	initBossHP(100); 

	//�������� ȣ��
	for (int i = 1; i <= 3; i++) {
		//�������� ���� �� ������ �ʱ�ȭ
		initData_Time();
		initData_GameBoardInfo();

		//�������� ����
		StartStage(i);
	}
}

void DisplayScoreBoard() {
	int startX = 6, startY = 4;
	int i;

	ClearBoard();
	SetCurrentCursorPos(2, 1);
	puts("ESC) ���ư���");

	//���ھ�� ���
	SetCurrentCursorPos(startX + 1, startY);
	puts("**********************");
	startY++;
	SetCurrentCursorPos(startX + 1, startY);
	puts("***** ���ھ�� *****");
	startY++;
	SetCurrentCursorPos(startX + 1, startY);
	puts("**********************");
	startY++;
	SetCurrentCursorPos(startX, startY);
	puts("����������������������������������������������");
	startY++;
	SetCurrentCursorPos(startX, startY);
	puts("��  ���� ��    ���ھ�   ��");

	for (i = 1; i <= 10; i++) {
		if (i == 1) {
			SetCurrentCursorPos(startX, startY + i * 2 - 1);
			puts("����������������������������������������������");
		}
		else {
			SetCurrentCursorPos(startX, startY + i * 2 - 1);
			puts("��       ��             ��");
		}
		SetCurrentCursorPos(startX, startY + i * 2);
		printf("��   %2d  ��   %-10d��", i, scoreboard[i - 1]);
	}
	SetCurrentCursorPos(startX, startY + i * 2 - 1);
	puts("����������������������������������������������");

	//���ư���
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
	puts("ESC) ���ư���");

	//��� ��� ���
	//���ھ�� ���
	SetCurrentCursorPos(startX + 1, startY);
	puts("******************************************************************************");
	startY++;
	SetCurrentCursorPos(startX + 1, startY);
	puts("********************************** ����� **********************************");
	startY++;
	SetCurrentCursorPos(startX + 1, startY);
	puts("******************************************************************************");

	for (i = 1; i <= MEMORY_SIZE; i++) {
		if (i == 1) {
			textcolor(WHITE);
			SetCurrentCursorPos(startX, startY + i * 2 - 1);
			puts("��������������������������������������������������������������������������������������������������������������������������������������������������������������");
		}
		else {
			textcolor(WHITE);
			SetCurrentCursorPos(startX, startY + i * 2 - 1);
			puts("��������������������������������������������������������������������������������������������������������������������������������������������������������������");
		}
		SetCurrentCursorPos(startX, startY + i * 2);
		if (memoryList[i - 1].check) {
			textcolor(WHITE);
			printf("��");
			textcolor(YELLOW);
			printf("    %-70s   ", memoryList[i - 1].unlocked);
			textcolor(WHITE);
			printf("��");
		}
		else {
			textcolor(WHITE);
			printf("��");
			textcolor(darkGRAY);
			printf("    %-70s   ", memoryList[i - 1].locked);
			textcolor(WHITE);
			printf("��");
		}
	}
	textcolor(WHITE);
	SetCurrentCursorPos(startX, startY + i * 2 - 1);
	puts("��������������������������������������������������������������������������������������������������������������������������������������������������������������");

	//���ư���
	while (1) {
		GetKeyboardInput();
		if (key == ESC) ShowMainMenu();
	}
}

void DisplayGameRule() {
	int startX = 6, startY = 4;

	ClearBoard();
	SetCurrentCursorPos(2, 1);
	puts("ESC) ���ư���");
	SetCurrentCursorPos(startX, startY);
	puts("�� �� �� �� �� Ű�� ������ �� �ֽ��ϴ�.");
	SetCurrentCursorPos(startX, startY + 2);
	puts("�� ���� �ӵ��� ���� �ڵ����� �����մϴ�.");
	SetCurrentCursorPos(startX, startY + 4);
	puts("�� ���� óġ�ϸ� ����ġ�� ��� ���� Ȯ���� �������� �����ϴ�.");
	SetCurrentCursorPos(startX, startY + 6);
	printf("�� ������ ");
	textcolor(RED);
	printf("��");
	textcolor(WHITE);
	printf("�� ȹ���ϸ� HP�� 1 ȸ��, ");
	textcolor(darkGRAY);
	printf("��");
	textcolor(WHITE);
	printf("�� ȹ���ϸ� ��� ���� óġ�մϴ�.");
	SetCurrentCursorPos(startX, startY + 8);
	puts("�� ���� �̻� ����ġ�� ȹ���ϸ� ���ο� ��ų�� ȹ���� �� �ֽ��ϴ�.");
	SetCurrentCursorPos(startX, startY + 10);
	puts("�� ������ óġ�ϸ� Game Clear, �� ���� HP�� ��� �����Ǹ� Game Over�Դϴ�.");

	while (1) {
		GetKeyboardInput();
		if (key == ESC) ShowMainMenu();
	}
}

void ExitGame() {
	ClearBoard();
	SetCurrentCursorPos(7, 4);
	puts("*** ������ �����մϴ�. ***\n\n\n");
	exit(1);
}
