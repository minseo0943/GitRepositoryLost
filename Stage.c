#pragma warning(disable:4996)
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
//��� ����
#include "DrawBoard.h"
#include "Cursor.h"
#include "Color.h"
#include "Stage.h"

void StartStage(int stageNum) {
    ClearGameBoard();
    if (stage == 3) {
        initBossPoint();
        initBossHP(100);
        BossStage();
    }
    //UI ����
    stage = stageNum;


    if (stage == 1) {
        PlaySound(TEXT("��������1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        textcolor(YELLOW);
        SetCurrentCursorPos(15, 9);  puts("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 10); puts("$$$$$$$        $$$$$        $$$$$$$$$$ $$$$$$$$$$$       $$$$$$$$$       $$$$$$$$$$$    $$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 11); puts("$$$$$$  $$$$$$$$$$$$$$4  $$$$$$$$$$$$   $$$$$$$$$  $$$$$$$$$$$$$$$  $$$$$$$$$$$$$$  $$  $$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 12); puts("$$$$$  $$$$$$$$$$$$$$$$  $$$$$$$$$$$  $  $$$$$$$   $$$$$$$$$$$$$$$  $$$$$$$$$$$$$  $$$  $$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 13); puts("$$$$$$      $$$$$$$$$$$  $$$$$$$$$$  $$$  $$$$$$  $$$$$$   $$$$$$$       $$$$$$$$$$$$$  $$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 14); puts("$$$$$$$$$$$  $$$$$$$$$$  $$$$$$$$$         $$$$$$  $$$$$$ $$$$$$$$  $$$$$$$$$$$$$$$$$$  $$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 15); puts("$$$$$$$$$$$$  $$$$$$$$$  $$$$$$$$  $$$$$$$  $$$$$$  $$$$$ $$$$$$$$  $$$$$$$$$$$$$$$$$$  $$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 16); puts("$$$$         $$$$$$$$$$  $$$$$$$  $$$$$$$$$  $$$$$$       $$$$$$$$       $$$$$$$$           $$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 17);  puts("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
        Sleep(2000);
        SetCurrentCursorPos(15, 17); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(15, 16); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(15, 15); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(15, 14); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(15, 13); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(15, 12); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(15, 11); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(15, 10); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(15, 9); puts("                                                                                                 ");
    }
    else if (stage == 2) {
        PlaySound(TEXT("��������2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        EraseGameBoard();
        textcolor(YELLOW);
        SetCurrentCursorPos(15, 9);  puts("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 10); puts("$$$$$$$        $$$$$        $$$$$$$$$$ $$$$$$$$$$$       $$$$$$$$$       $$$$$$$$$$$      $$$$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 11); puts("$$$$$$  $$$$$$$$$$$$$$$  $$$$$$$$$$$$   $$$$$$$$$  $$$$$$$$$$$$$$$  $$$$$$$$$$$$$$  $$$$  $$$$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 12); puts("$$$$$  $$$$$$$$$$$$$$$$  $$$$$$$$$$$  $  $$$$$$$   $$$$$$$$$$$$$$$  $$$$$$$$$$$$$  $$$$  $$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 13); puts("$$$$$$      $$$$$$$$$$$  $$$$$$$$$$  $$$  $$$$$$  $$$$$$   $$$$$$$       $$$$$$$$$$$$$  $$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 14); puts("$$$$$$$$$$$  $$$$$$$$$$  $$$$$$$$$         $$$$$$  $$$$$$ $$$$$$$$  $$$$$$$$$$$$$$$$  $$$$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 15); puts("$$$$$$$$$$$$  $$$$$$$$$  $$$$$$$$  $$$$$$$  $$$$$$  $$$$$ $$$$$$$$  $$$$$$$$$$$$$$$  $$$$$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 16); puts("$$$$         $$$$$$$$$$  $$$$$$$  $$$$$$$$$  $$$$$$       $$$$$$$$       $$$$$$$$           $$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 17);  puts("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(15, 19); puts("                                   ***���Ͱ� �������ϴ�***                                  ");
        Sleep(2000);
        SetCurrentCursorPos(15, 9); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(15, 10); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(15, 11); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(15, 12); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(15, 13); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(15, 14); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(15, 15); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(15, 16); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(15, 17); puts("                                                                                                 ");
        SetCurrentCursorPos(15, 19); puts("                                                                                                 ");

    }
    else {
        PlaySound(TEXT("����1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        ClearBoard();
        EraseGameBoard();
        DrawBoard();
        textcolor(YELLOW);
        EraseGameBoard();
        SetCurrentCursorPos(18, 9);  puts("  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(18, 10); puts("  $$$$$$$$       $$$$$$$$$$$$$$$          $$$$$$$$$$$$$$$        $$$$$$$$$$        $$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(18, 11); puts("  $$$$$$$$  $$$$  $$$$$$$$$$$$$  $$$$$$$$  $$$$$$$$$$$$$  $$$$$$$$$$$$$$$$  $$$$$$$$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(18, 12); puts("  $$$$$$$$  $$$$$  $$$$$$$$$$$  $$$$$$$$$$  $$$$$$$$$$$  $$$$$$$$$$$$$$$$  $$$$$$$$$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(18, 13); puts("  $$$$$$$$        $$$$$$$$$$$  $$$$$$$$$$$$  $$$$$$$$$$      $$$$$$$$$$$$$      $$$$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(18, 14); puts("  $$$$$$$$  $$$$$   $$$$$$$$$$  $$$$$$$$$$  $$$$$$$$$$$$$$$$$  $$$$$$$$$$$$$$$$  $$$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(18, 15); puts("  $$$$$$$$  $$$$$$$  $$$$$$$$$$  $$$$$$$$  $$$$$$$$$$$$$$$$$$$  $$$$$$$$$$$$$$$$  $$$$$$$$");
        Sleep(100);
        SetCurrentCursorPos(18, 16); puts("  $$$$$$$$           $$$$$$$$$$$          $$$$$$$$$$$$         $$$$$$$$$         $$$$$$$$$");
        SetCurrentCursorPos(18, 17); puts("  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
        Sleep(2000);
        SetCurrentCursorPos(18, 9); puts("                                                                                                         ");
        Sleep(100);
        SetCurrentCursorPos(18, 10); puts("                                                                                                         ");
        Sleep(100);
        SetCurrentCursorPos(18, 11); puts("                                                                                                         ");
        Sleep(100);
        SetCurrentCursorPos(18, 12); puts("                                                                                                         ");
        Sleep(100);
        SetCurrentCursorPos(18, 13); puts("                                                                                                         ");
        Sleep(100);
        SetCurrentCursorPos(18, 14); puts("                                                                                                         ");
        Sleep(100);
        SetCurrentCursorPos(18, 15); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(18, 16); puts("                                                                                                 ");
        Sleep(100);
        SetCurrentCursorPos(18, 17); puts("                                                                                                         ");
    }
    //��ų �����
    //selectedSkill[0] = 130;
    //selectedSkill[1] = 530;

    //������ �ʱ�ȭ
    EraseGameBoard();
    //UI ���
    DrawIngameUI();

    //PC ��ġ ���ӿ� �߰�
    init_PCPoint();
    SetCurrentCursorPos(GBOARD_ORIGIN_X + PCPoint[0] * 2, GBOARD_ORIGIN_Y + PCPoint[1]);
    drawPC();

    //������ �׸���
    DrawBoard();

    //�������� ����
    if (stage == 3) {
        initBossPoint();
        initBossHP(100);
        BossStage();
    }
    else if (stage == 2) {
        textcolor(darkGRAY);
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                SetCurrentCursorPos(GBOARD_ORIGIN_X + 13 * 2 + j * 32, GBOARD_ORIGIN_Y + 12 + 14 * i);
                printf("�㢺");
                SetCurrentCursorPos(GBOARD_ORIGIN_X + 13 * 2 + j * 32, GBOARD_ORIGIN_Y + 13 + 14 * i);
                printf("����");
                GameBoardInfo[12 + 14 * i][13 + j * 16] = CHAINSAW;
                GameBoardInfo[12 + 14 * i][14 + j * 16] = CHAINSAW;
                GameBoardInfo[13 + 14 * i][14 + j * 16] = CHAINSAW;
                GameBoardInfo[13 + 14 * i][13 + j * 16] = CHAINSAW;

            }
        }
        NormalStage();
    }
    else
        NormalStage();



}

void NormalStage() {
    flag = -1;
    if (stage == 2) {
        Diff.npcSpeed = 2;
        Diff.spawnSpeed /= 2;
    }
    else Diff.npcSpeed = 3;

    LevelUp();
    while (TimeCount < STAGETIME) {
        ProcessKeyInput();

        //�������� �����
        //Debug_GameBoardInfo();
    }
}

void BossStage() {
    flag = -1;
    LevelUp();
    bossFlag = 0;

    while (1) {
        ProcessKeyInput();

        //�������� �����
        //Debug_GameBoardInfo();
    }
}

void PauseGame() {
    int key;
    while (1) {
        if (_kbhit() != 0) {
            key = _getch();
            if (key == W) break;
        }
    }
}
