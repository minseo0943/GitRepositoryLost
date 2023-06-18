#pragma warning(disable:4996)
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
//헤더 파일
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
    //UI 설정
    stage = stageNum;


    if (stage == 1) {
        PlaySound(TEXT("스테이지1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
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
        PlaySound(TEXT("스테이지2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
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
        SetCurrentCursorPos(15, 19); puts("                                   ***몬스터가 빨라집니다***                                  ");
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
        PlaySound(TEXT("보스1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
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
    //스킬 디버그
    //selectedSkill[0] = 130;
    //selectedSkill[1] = 530;

    //게임판 초기화
    EraseGameBoard();
    //UI 출력
    DrawIngameUI();

    //PC 위치 게임에 추가
    init_PCPoint();
    SetCurrentCursorPos(GBOARD_ORIGIN_X + PCPoint[0] * 2, GBOARD_ORIGIN_Y + PCPoint[1]);
    drawPC();

    //게임판 그리기
    DrawBoard();

    //스테이지 시작
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
                printf("▲▶");
                SetCurrentCursorPos(GBOARD_ORIGIN_X + 13 * 2 + j * 32, GBOARD_ORIGIN_Y + 13 + 14 * i);
                printf("◀▼");
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

        //마지막에 디버그
        //Debug_GameBoardInfo();
    }
}

void BossStage() {
    flag = -1;
    LevelUp();
    bossFlag = 0;

    while (1) {
        ProcessKeyInput();

        //마지막에 디버그
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
