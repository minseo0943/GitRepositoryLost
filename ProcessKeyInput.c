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
#include "MainMenu.h"
#include "ProcessKeyInput.h"

int key = 0;
Difficulty Diff;
int lastDir = EAST;

void ProcessKeyInput() {
    int key, dir = lastDir;
    for (int i = 0; i < 10; i++, AttackCount++) {
        if (_kbhit() != 0) {
            key = _getch();
            switch (key)
            {
            default:
                dir = lastDir;
                break;
            case KEY_LEFT:
                dir = WEST;
                lastDir = dir;
                PcShiftLeft();
                break;
            case KEY_RIGHT:
                dir = EAST;
                lastDir = dir;
                PcShiftRight();
                break;
            case KEY_UP:
                dir = NORTH;
                lastDir = dir;
                PcShiftUp();
                break;
            case KEY_DOWN:
                dir = SOUTH;
                lastDir = dir;
                PcShiftDown();
                break;
            case W: //일시정지
                PauseGame();
                break;
                //case A:   // A 누르면 Alldie
                //    AllDie();
                //    break;
            }
        }
        //이펙트 이동 속도
        switch (selectedSkill[0] / 100)
        {
        case 1: //권총
            if (AttackCount % 1 == 0) SkillEffectMove();
            break;
        case 2: //산탄총
            if (AttackCount % 3 == 0) SkillEffectMove();
            break;
        case 3: //석궁
            if (AttackCount % 6 == 0) SkillEffectMove();
            break;
        }
        //게임 속도
        Sleep(Diff.speed);
    }
    if (stage == 2) {
        textcolor(WHITE);
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                textcolor(WHITE);
                if (ChainsawTouched[i * 3 + j]-- > 0)textcolor(RED);
                switch (TimeCount % 4)
                {
                case 0:
                    SetCurrentCursorPos(GBOARD_ORIGIN_X + 13 * 2 + j * 32, GBOARD_ORIGIN_Y + 12 + 14 * i);
                    printf("▲▶");
                    SetCurrentCursorPos(GBOARD_ORIGIN_X + 13 * 2 + j * 32, GBOARD_ORIGIN_Y + 13 + 14 * i);
                    printf("◀▼");
                    break;
                case 1:
                    SetCurrentCursorPos(GBOARD_ORIGIN_X + 13 * 2 + j * 32, GBOARD_ORIGIN_Y + 12 + 14 * i);
                    printf("◀▲");
                    SetCurrentCursorPos(GBOARD_ORIGIN_X + 13 * 2 + j * 32, GBOARD_ORIGIN_Y + 13 + 14 * i);
                    printf("▼▶");
                    break;

                case 2:
                    SetCurrentCursorPos(GBOARD_ORIGIN_X + 13 * 2 + j * 32, GBOARD_ORIGIN_Y + 12 + 14 * i);
                    printf("▼◀");
                    SetCurrentCursorPos(GBOARD_ORIGIN_X + 13 * 2 + j * 32, GBOARD_ORIGIN_Y + 13 + 14 * i);
                    printf("▶▲");
                    break;
                case 3:
                    SetCurrentCursorPos(GBOARD_ORIGIN_X + 13 * 2 + j * 32, GBOARD_ORIGIN_Y + 12 + 14 * i);
                    printf("▶▼");
                    SetCurrentCursorPos(GBOARD_ORIGIN_X + 13 * 2 + j * 32, GBOARD_ORIGIN_Y + 13 + 14 * i);
                    printf("▲◀");
                    break;
                }
            }
        }
    }
    TimeCount++;
    drawTime();

    if (stage != 3 && TimeCount == STAGETIME / 2) {
        displayNotice("적이 더 빠르게 생성됩니다.");

        Diff.spawnSpeed /= 2;
    }
    else if (stage != 3 && TimeCount == STAGETIME / 2 + 10) eraseNotice();

    if (stage == 3) {
        //보스전
        SetCurrentCursorPos(128, 20);
        textcolor(GREEN);
        printf("▶ 보스 HP : ");
        for (int i = 0; i <= BossHP / 10; i++)printf("♥");
        for (int i = 0; i < 10 - BossHP / 10; i++)printf("  ");
        int move = 100; //time: move

        //스킬 이펙트 이동
        SkillEffectMove();

        //pc 원거리 스킬
        if (TimeCount % Diff.attackSpeed == 0) {
            for (int i = 0; i < 3; i++) {
                if (selectedSkill[i] == 0) continue;
                AddEffectByDirection(selectedSkill[i], dir);
            }
        }

        //pc 근거리 스킬 공격
        if (TimeCount % Diff.attackSpeed >= Diff.attackSpeed / 2) {
            if (selectedSkill[1] / 100 % 10 == 4) {
                switch (dir)
                {
                case EAST:
                    if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2) AddEffectByDirection(selectedSkill[1], NORTH);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 1) AddEffectByDirection(selectedSkill[1], EAST);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 2) AddEffectByDirection(selectedSkill[1], SOUTH);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 3) AddEffectByDirection(selectedSkill[1], dir);
                    break;
                case NORTH:
                    if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2) AddEffectByDirection(selectedSkill[1], WEST);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 1) AddEffectByDirection(selectedSkill[1], NORTH);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 2) AddEffectByDirection(selectedSkill[1], EAST);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 3) AddEffectByDirection(selectedSkill[1], dir);
                    break;
                case WEST:
                    if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2) AddEffectByDirection(selectedSkill[1], SOUTH);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 1) AddEffectByDirection(selectedSkill[1], WEST);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 2) AddEffectByDirection(selectedSkill[1], NORTH);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 3) AddEffectByDirection(selectedSkill[1], dir);
                    break;
                case SOUTH:
                    if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2) AddEffectByDirection(selectedSkill[1], EAST);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 1) AddEffectByDirection(selectedSkill[1], SOUTH);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 2) AddEffectByDirection(selectedSkill[1], WEST);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 3) AddEffectByDirection(selectedSkill[1], dir);
                    break;
                }
            }
            else AddEffectByDirection(selectedSkill[1], dir);
        }

        //보스 행동 시뮬레이션; bossFlag=0~3 -> 0:Circle, 1:Rect, 2:Waffle
        if (TimeCount % move == 3) { bossFlag = 0; }
        if (TimeCount % move == 9) { DeleteBossSkill(); bossFlag = 1; } //보스 멈춤
        if (TimeCount % move == 50) { DeleteBossSkill(); bossFlag = 2; } //보스 멈춤
        if (TimeCount == TimeCount2 + 1) { DeleteBossSkill(); }
        if (TimeCount % move == 80) { DeleteBossSkill(); bossFlag = 3; }
        if (TimeCount % move == 95) { DeleteBossSkill(); bossFlag = 0; }

        if ((bossFlag == 0 || bossFlag == 3) && TimeCount % 2 == 0) BossMove();
        else if (bossFlag == 1) {
            //스킬 Rect 시전
            if (TimeCount % move == 10) {
                //안쪽 사각형 왼쪽위 오른쪽 아래좌표
                XX1 = BossPoint[0] - 4;
                YY1 = BossPoint[1] - 4;
                XX2 = BossPoint[0] + 10;
                YY2 = BossPoint[1] + 10;
                ShowBossSkill_RectOrbit(XX1, YY1, XX2, YY2);   //궤도
            }
            else if (TimeCount % move == 14) {
                DeleteBossSkill2();
            }
            else if (TimeCount % move == 15) {
                //안쪽 사각형 왼쪽위 오른쪽 아래좌표
                ShowBossSkill_Rect(XX1, YY1, XX2, YY2);
            }
            else if (TimeCount % move > 15 && TimeCount % 2 == 1)
                ShowBossSkill_Rect(XX1, YY1, XX2, YY2);
            GameClear();
        }
        else if (bossFlag == 2) {
            //스킬 Waffle 시전
            if (TimeCount % move == 51) {
                ShowBossSkill_WaffleOrbit();
            }
            else if (TimeCount % move == 54) {
                DeleteBossSkill2();
            }
            else if (TimeCount % move == 55) {
                random = rand() % 4 + 1;
                if (random == 1) x3 = 1; //왼쪽
                else if (random == 2) y3 = 1; //위
                else if (random == 3)  x3 = GBOARD_WIDTH; //오른쪽
                else if (random == 4) y3 = GBOARD_HEIGHT; //아래
                ShowBoss();
            } //TimeCount 5동안 멈춤
            else if (TimeCount % move > 55 && TimeCount % 2 == 0)
            {
                if (random == 1)  ShowBossSkill_Waffle_Left(x3);
                else if (random == 2) ShowBossSkill_Waffle_Up(y3);
                else if (random == 3)  ShowBossSkill_Waffle_Right(x3);
                else if (random == 4) ShowBossSkill_Waffle_Down(y3);
            }
        }
    }
    else {
        //일반전

        //pc 원거리 스킬 공격
        if (TimeCount % Diff.attackSpeed == 0) {
            AddEffectByDirection(selectedSkill[0], dir);
        }

        //pc 근거리 스킬 공격
        if (TimeCount % Diff.attackSpeed >= Diff.attackSpeed / 2) {
            if (selectedSkill[1] / 100 % 10 == 4) {
                switch (dir)
                {
                case EAST:
                    if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2) AddEffectByDirection(selectedSkill[1], NORTH);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 1) AddEffectByDirection(selectedSkill[1], EAST);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 2) AddEffectByDirection(selectedSkill[1], SOUTH);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 3) AddEffectByDirection(selectedSkill[1], dir);
                    break;
                case NORTH:
                    if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2) AddEffectByDirection(selectedSkill[1], WEST);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 1) AddEffectByDirection(selectedSkill[1], NORTH);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 2) AddEffectByDirection(selectedSkill[1], EAST);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 3) AddEffectByDirection(selectedSkill[1], dir);
                    break;
                case WEST:
                    if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2) AddEffectByDirection(selectedSkill[1], SOUTH);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 1) AddEffectByDirection(selectedSkill[1], WEST);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 2) AddEffectByDirection(selectedSkill[1], NORTH);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 3) AddEffectByDirection(selectedSkill[1], dir);
                    break;
                case SOUTH:
                    if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2) AddEffectByDirection(selectedSkill[1], EAST);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 1) AddEffectByDirection(selectedSkill[1], SOUTH);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 2) AddEffectByDirection(selectedSkill[1], WEST);
                    else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 3) AddEffectByDirection(selectedSkill[1], dir);
                    break;
                }
            }
            else AddEffectByDirection(selectedSkill[1], dir);
        }

        //게임판에 있는 요소 이동
        SkillEffectMove();
        if (TimeCount % Diff.npcSpeed == 0) {
            NpcMove();
            RangeNpcMove();
        }
        ProjectileMove();

        SetCurrentCursorPos(133, 15);
        textcolor(WHITE);
        for (int i = 0; i < TimeCount % Diff.attackSpeed; i++)printf("■");
        for (int i = 0; i < Diff.attackSpeed - TimeCount % Diff.attackSpeed - 1; i++)printf("□");




        //나누는 값으로 속도 조절
        if (TimeCount % Diff.spawnSpeed == 0) NpcSpawn();
        if (TimeCount % Diff.spawnSpeed == 0) RangeNpcSpawn();
        if (TimeCount % Diff.projectileSpawnSpeed == 2) RangeNpcAttack();
    }
}
