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
#include "InitGameData.h"
#include "ItemData.h"
#include "PCNPC.h"
#include "Skill.h"

int GameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 1];
int score = 0;
int finalScore = 0;
int HP = 5;
int maxHP = 5;
int stage = 0;
COORD pos_time;
COORD pos_stage;
COORD pos_score;
COORD pos_hp;
COORD pos_skillList;
COORD pos_skillSelect;
COORD cursor;
int maxExpScore;

//게임판 영역 지우기; 스테이지 표시 지우기용
void ClearGameBoard() {
    int x, y;
    for (y = 1; y < GBOARD_HEIGHT; y++) {
        for (x = 1; x < GBOARD_WIDTH; x++) {
            if (GameBoardInfo[y][x] == 0) {
                SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + y);
                printf("  ");
            }
        }
    }
}


//보드 전부 지우기
void ClearBoard() {
    system("cls"); //콘솔창 전부 지우는 명령어
}

//게임판 테두리 그리고 게임판 정보를 GameBoardInfo 배열에 저장하는 함수
//테두리: 세로 = 0, GBOARD_HEIGHT / 가로 = 0, GBOARD_WIDTH

void DrawBoard() {
    int x, y;

    textcolor(WHITE);
    textcolor(YELLOW);

    //상단
    for (x = 1; x < GBOARD_WIDTH; x++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y);
        printf("■");
        GameBoardInfo[0][x] = 1;
        
    }

    //하단
    for (x = 1; x < GBOARD_WIDTH * 2; x++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + x, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
        printf("■");
        GameBoardInfo[GBOARD_HEIGHT][x] = 1;
    }

    //우변
    for (y = 0; y <= GBOARD_HEIGHT; y++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2, GBOARD_ORIGIN_Y + y);
        if (y == 0) printf("■");
        else if (y == GBOARD_HEIGHT) printf("■");
        else printf("■");
        GameBoardInfo[y][GBOARD_WIDTH] = 1;
    }

    //좌변
    for (y = 0; y <= GBOARD_HEIGHT; y++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
        if (y == 0) printf("■");
        else if (y == GBOARD_HEIGHT) printf("■");
        else printf("■");
        GameBoardInfo[y][0] = 1;
    }

    //알림창
    //상단
    for (x = 1; x < GBOARD_WIDTH * 2; x++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + x, GBOARD_ORIGIN_Y + GBOARD_HEIGHT + 1);
        printf("─");
    }

    //우변
    for (y = GBOARD_HEIGHT + 1; y <= GBOARD_HEIGHT + 3; y++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2, GBOARD_ORIGIN_Y + y);
        if (y == GBOARD_HEIGHT + 1) printf("┐");
        else if (y == GBOARD_HEIGHT + 3) printf("┘");
        else printf("│");
    }

    //좌변
    for (y = GBOARD_HEIGHT + 1; y <= GBOARD_HEIGHT + 3; y++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
        if (y == GBOARD_HEIGHT + 1) printf("┌");
        else if (y == GBOARD_HEIGHT + 3) printf("└");
        else printf("│");
    }

    //하단
    for (x = 1; x < GBOARD_WIDTH * 2; x++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + x, GBOARD_ORIGIN_Y + GBOARD_HEIGHT + 3);
        printf("─");
    }
}

void displayNotice(char* str) {
    textcolor(WHITE);
    SetCurrentCursorPos(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT + 2);
    printf("▶ %s", str);
}

void eraseNotice() {
    for (int x = 2; x < GBOARD_WIDTH * 2; x++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + x, GBOARD_ORIGIN_Y + GBOARD_HEIGHT + 2);
        printf(" ");
    }
}

void LevelUp() {
    //예외 처리
    if (selectedSkill[0] / 10 % 10 == 3 && selectedSkill[1] / 10 % 10 == 3 && selectedSkill[2] / 10 % 10 == 3) return;

    int inFlag = flag;
    if (inFlag == -1) {
        displayNotice("레벨 업!!! 3가지 스킬 중 하나를 선택하세요. (입력: 숫자 1, 2, 3)");
        SelectNewSkill();
        eraseNotice();
        drawSelectedSkillList();

    }
    else if (score >= maxExpScore) {
        displayNotice("레벨 업!!! 3가지 스킬 중 하나를 선택하세요. (입력: 숫자 1, 2, 3)");
        SelectNewSkill();
        eraseNotice();
        drawSelectedSkillList();

        //요구 exp 증가
        maxExpScore = ((maxExpScore / 3) * 4 + maxExpScore) / 100 * 100;
    }
}

void IncreaseScore() {
    score += 100;
    drawScore();
    LevelUp();
}

//게임판 배열 디버그용
void Debug_GameBoardInfo() {
    int data;
    textcolor(WHITE);
    for (int y = -1; y < GBOARD_HEIGHT + 1; y++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 30, GBOARD_ORIGIN_Y + y);
        if (y == -1) {
            for (int x = 0; x <= GBOARD_WIDTH; x++) {
                printf("%02d", x);
            }
            printf("%d", y);
        }
        else {
            for (int x = 0; x <= GBOARD_WIDTH; x++) {
                data = GameBoardInfo[y][x];
                if (data == 0) textcolor(darkGRAY);
                else textcolor(WHITE);
                if (data == NPCBULLET) {
                    for (int i = 0; i < maxProjectile; i++) {
                        if (ProjectileInfo[i][0] == x && ProjectileInfo[i][1] == y) {
                            printf("%d%d", data, ProjectileInfo[i][2] / 100 % 10);
                            break;
                        }
                    }
                }
                else if (data < 100) printf("%-2d", data);
                else printf("%-2d", data / 10);
            }
            printf("%02d", y);
        }
    }

    textcolor(RED);
    for (int i = 0; i <= skillTop; i++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 30 + skillEffectInfo[i].X * 2, GBOARD_ORIGIN_Y + skillEffectInfo[i].Y);
        printf("#");
    }
}

//UI 좌표 초기화
void initPosUI() {
    pos_time.X = GBOARD_ORIGIN_X + (GBOARD_WIDTH + 3) * 2;
    pos_time.Y = GBOARD_ORIGIN_Y + 1;

    pos_stage.X = GBOARD_ORIGIN_X + 1;
    pos_stage.Y = GBOARD_ORIGIN_Y - 1;

    pos_score.X = GBOARD_ORIGIN_X + (GBOARD_WIDTH + 3) * 2;
    pos_score.Y = GBOARD_ORIGIN_Y + 3;

    pos_hp.X = GBOARD_ORIGIN_X + (GBOARD_WIDTH + 3) * 2;
    pos_hp.Y = GBOARD_ORIGIN_Y + 5;
    
    pos_skillList.X = GBOARD_ORIGIN_X + (GBOARD_WIDTH + 3) * 2;
    pos_skillList.Y = GBOARD_ORIGIN_Y + 9;

    pos_skillSelect.X = GBOARD_ORIGIN_X + (GBOARD_WIDTH + 3) * 2;
    pos_skillSelect.Y = GBOARD_ORIGIN_Y + 16;
}

//인게임 UI 표시
void DrawIngameUI() {
    drawStage(); //스테이지
    drawTime(); //시간
    drawHP(); //체력
    drawScore(); //스코어
    drawSelectedSkillList(); //보유 스킬
}

//스테이지 표시
void drawStage() {
    SetCurrentCursorPos(pos_stage.X, pos_stage.Y);
    textcolor(GRAY);
    printf("< 스테이지 %d >", stage);
}

void drawTime() {
    SetCurrentCursorPos(pos_time.X, pos_time.Y);
    textcolor(WHITE);
    printf("▶ 남은 시간 : %-4d", (STAGETIME - (TimeCount % STAGETIME)) / 6);
}

//스코어 표시
void drawScore() {
    SetCurrentCursorPos(pos_score.X, pos_score.Y);
    textcolor(WHITE);
    printf("▶ 스코어  :  %-6d", score);
}

//체력 출력하고 게임 오버 검사
void drawHP() {
    SetCurrentCursorPos(pos_hp.X, pos_hp.Y);
    textcolor(WHITE);
    printf("▶ 체력  :  ");

    textcolor(RED);
    for (int i = 0; i < HP; i++) printf("♥");
    textcolor(darkRED);
    for (int i = HP; i < maxHP; i++) printf("♡");

    if (GameOver()) ShowMainMenu();
}

//보유한 스킬 정보 출력
void drawSelectedSkillList() {
    int type, level;

    SetCurrentCursorPos(pos_skillList.X, pos_skillList.Y);
    textcolor(WHITE);
    printf("▶ 가진 스킬");
    
    textcolor(GRAY);
    SetCurrentCursorPos(pos_skillList.X, pos_skillList.Y + 1);
    if (selectedSkill[0] == 0) {
        printf("▷ 원거리 : ");
        textcolor(darkGRAY);
        printf("미보유");
    }
    else {
        type = (selectedSkill[0] / 100) % 10;
        level = (selectedSkill[0] / 10) % 10;
        printf("▷ 원거리 : %-16s", skillList[type - 1][level - 1].name);
    }

    textcolor(GRAY);
    SetCurrentCursorPos(pos_skillList.X, pos_skillList.Y + 2);
    if (selectedSkill[1] == 0) {
        printf("▷ 근거리 : ");
        textcolor(darkGRAY);
        printf("미보유");
    }
    else {
        type = (selectedSkill[1] / 100) % 10;
        level = (selectedSkill[1] / 10) % 10;
        printf("▷ 근거리 : %-16s", skillList[type - 1][level - 1].name);
    }

    textcolor(GRAY);
    SetCurrentCursorPos(pos_skillList.X, pos_skillList.Y + 3);
    if (selectedSkill[2] == 0) {
        printf("▷ 패시브 : ");
        textcolor(darkGRAY);
        printf("미보유");
    }
    else {
        type = (selectedSkill[2] / 100) % 10;
        level = (selectedSkill[2] / 10) % 10;
        printf("▷ 패시브 : %-16s", skillList[type - 1][level - 1].name);
    }
}

//스킬 이펙트 그리기
void drawSkillShape(int type, int level) {
    textcolor(GRAY);
    printf("%s", skillList[type - 1][level - 1].e);
}

//PC 그리기
void drawPC() {
    textcolor(WHITE);
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) { 
            GameBoardInfo[PCPoint[1] + y][PCPoint[0] + x] = PC;
            SetCurrentCursorPos((PCPoint[0] + x) * 2 + GBOARD_ORIGIN_X, (PCPoint[1] + y) + GBOARD_ORIGIN_Y);

            switch (lastDir)
            {
            case EAST:
                if (x == 1 && y == 1) textcolor(RED), printf("▶");
                else if (x == 1 && y == 0) textcolor(WHITE), printf("▶");
                else textcolor(YELLOW), printf("■");
                break;
            case WEST:
                if (x == 0 && y == 0) textcolor(RED), printf("◀");
                else if (x == 0 && y == 1) textcolor(WHITE), printf("◀");
                else textcolor(YELLOW), printf("■");
                break;
            case NORTH:
                if (y == 0 && x == 1) textcolor(RED), printf("▲");
                else if (y == 0 && x == 0) textcolor(WHITE), printf("▲");
                else textcolor(YELLOW), printf("■");
                break;
            case SOUTH:
                if (y == 1 && x == 0) textcolor(RED), printf("▼");
                else if (y == 1 && x == 1) textcolor(WHITE), printf("▼");
                else textcolor(YELLOW), printf("■");
                break;
                break;
            }
        }
    }
}

//PC 지우기
void erasePC() {
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            GameBoardInfo[PCPoint[1] + y][PCPoint[0] + x] = 0;
            SetCurrentCursorPos((PCPoint[0] + x) * 2 + GBOARD_ORIGIN_X, (PCPoint[1] + y) + GBOARD_ORIGIN_Y);
            printf("  ");
        }
    }
}

//NPC 그리기
void drawNpcShape() {
    textcolor(darkGREEN);
    printf("●");
}

//RangeNPC 그리기
void drawRangeNpcShape() {
    textcolor(BLUE);
    printf("◆");
}

//StopRangeNPC 그리기
void drawStopRangeNpcShape() {
    textcolor(SKYBLUE);
    printf("◆");
}

void drawItemShape(int num) {
    textcolor(itemList[num].color);
    printf("%s", itemList[num].shape);
}

//게임 오버
int GameOver() {
    if (HP > 0) return 0; //hp가 0보다 크면 return 0

    //기억 업데이트
    UpdateMemoryList();

    //pc 제거
    SetCurrentCursorPos(GBOARD_ORIGIN_X + PCPoint[0] * 2, GBOARD_ORIGIN_Y + PCPoint[1]);
    printf("  ");

    //모든 유닛 제거
    EraseGameBoard();

    //게임 오버 문구 출력 및 ESC 입력으로 메인 메뉴 이동
    int y = -2;
    SetCurrentCursorPos(47, center.Y + y);
    textcolor(darkRED);
    printf("*** GAME OVER ***");

    y += 2;
    int x = 1;
    int n = score;
    while (n != 0) {
        x++;
        n /= 10;
    }
    SetCurrentCursorPos(48 - x / 2, center.Y + y);
    textcolor(WHITE);
    printf("최종 스코어 : %d", score);

    y += 2;
    SetCurrentCursorPos(39, center.Y + y);
    textcolor(darkGRAY);
    printf("(ESC를 눌러 메인 메뉴로 돌아가기)");

    while (1) {
        GetKeyboardInput();
        if (key == ESC) break;
    }

    UpdateScoreboard();
    return 1;
}

//게임 클리어
void GameClear() {
    if (BossHP > 0) return; //BossHP가 0보다 크면 return

     //기억 업데이트
    memoryList[19].check = TRUE;
    UpdateMemoryList();

    //스코어 증가
    score += 5000;
    drawScore();

    //pc 제거
    SetCurrentCursorPos(GBOARD_ORIGIN_X + PCPoint[0] * 2, GBOARD_ORIGIN_Y + PCPoint[1]);
    printf("  ");

    //모든 유닛 제거
    EraseGameBoard();

    //게임 오버 문구 출력 및 ESC 입력으로 메인 메뉴 이동
    int y = -2;
    SetCurrentCursorPos(46, center.Y + y);
    textcolor(SKYBLUE);
    printf("*** GAME CLEAR ***");

    y += 2;
    int x = 1;
    int n = score;
    while (n != 0) {
        x++;
        n /= 10;
    }
    SetCurrentCursorPos(48 - x / 2, center.Y + y);
    textcolor(WHITE);
    printf("최종 스코어 : %d", score);

    y += 2;
    SetCurrentCursorPos(39, center.Y + y);
    textcolor(darkGRAY);
    printf("(ESC를 눌러 메인 메뉴로 돌아가기)");

    while (1) {
        GetKeyboardInput();
        if (key == ESC) break;
    }

    UpdateScoreboard();
    ShowMainMenu();
}

//게임판 모든 유닛 제거
void EraseGameBoard() {
    //각종 npc 및 투사체 제거하고 스코어 복구
    finalScore = score;
    AllDie();
    score = finalScore;

    //pc 제거
    if (stage == 2 || stage == 3) {
        erasePC();
    }

    //스킬 제거
    for (int i = 0; i <= skillTop; i++) {
        GameBoardInfo[skillEffectInfo[i].Y][skillEffectInfo[i].X] = 0;
        SetCurrentCursorPos(GBOARD_ORIGIN_X + skillEffectInfo[i].X * 2, GBOARD_ORIGIN_Y + skillEffectInfo[i].Y);
        printf("  ");
    }
    skillTop = -1;

    //아이템 등 나머지 제거
    for (int y = 1; y < GBOARD_HEIGHT; y++) {
        for (int x = 1; x < GBOARD_WIDTH; x++) {
            if (GameBoardInfo[y][x] != 0) {
                GameBoardInfo[y][x] = 0;
                SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + y);
                printf("  ");
            }
        }
    }
}

//hp 감소
void DecreaseHP() {
    textcolor(RED);
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            GameBoardInfo[PCPoint[1] + y][PCPoint[0] + x] = PC;
            SetCurrentCursorPos((PCPoint[0] + x) * 2 + GBOARD_ORIGIN_X, (PCPoint[1] + y) + GBOARD_ORIGIN_Y);

            switch (lastDir)
            {
            case EAST:
                if (x == 1) printf("▶");
                else printf("■");
                break;
            case WEST:
                if (x == 0) printf("◀");
                else printf("■");
                break;
            case NORTH:
                if (y == 0) printf("▲");
                else printf("■");
                break;
            case SOUTH:
                if (y == 1) printf("▼");
                else printf("■");
                break;
                break;
            }
        }
    }

    Sleep(50);

    drawPC();

    HP--;
    drawHP();
}

//보스 hp 감소
void DecreaseBossHP() {
    //darkRED ShowBoss
    textcolor(darkRED);
    for (int y = 0; y < 7; y++) {
        for (int x = 0; x < 7; x++) {
            SetCurrentCursorPos(BossPoint[0] * 2 + GBOARD_ORIGIN_X + x * 2, BossPoint[1] + GBOARD_ORIGIN_Y + y);
            if (BossModel[y][x] == BossNum) {
                GameBoardInfo[BossPoint[1] + y][BossPoint[0] + x] = BossNum;
                if (y == 2 && (x == 1 || x == 5)) printf("●");
                else if (y == 3 && x == 3) printf("▲");
                else printf("■");
            }
        }
    }

    Sleep(50);

    BossHP--;
    

    ShowBoss();
}

void UpdateScoreboard() {
    for (int i = 0; i < SCOREBOARD__SIZE; i++) {
        if (scoreboard[i] < score) {
            for (int j = SCOREBOARD__SIZE - 1; j > i; j--) {
                scoreboard[j] = scoreboard[j - 1];
            }
            scoreboard[i] = score;
            break;
        }
    }
}

void UpdateMemoryList() {
    if (stage >= 1 && !memoryList[0].check) memoryList[0].check = TRUE;
    if (stage >= 2 && !memoryList[1].check) memoryList[1].check = TRUE;
    if (stage == 3 && !memoryList[2].check) memoryList[2].check = TRUE;
    if (selectedSkill[0] / 100 >= 1 && selectedSkill[0] / 100 <= 3 && selectedSkill[0] % 100 / 10 >= 2 && !memoryList[3].check) memoryList[3].check = TRUE;
    if(selectedSkill[0] ==130 && !memoryList[4].check) memoryList[4].check = TRUE;
    if (selectedSkill[0] == 230 && !memoryList[5].check) memoryList[5].check = TRUE;
    if (selectedSkill[0] == 330 && !memoryList[6].check) memoryList[6].check = TRUE;
    if (selectedSkill[1] / 100 == 4 && !memoryList[7].check) memoryList[7].check = TRUE;
    if (selectedSkill[1] / 100 == 5 && !memoryList[8].check) memoryList[8].check = TRUE;
    if (selectedSkill[1] / 100 == 6 && !memoryList[9].check) memoryList[9].check = TRUE;
    if (selectedSkill[1] == 430 && !memoryList[10].check) memoryList[10].check = TRUE;
    if (selectedSkill[1] == 530 && !memoryList[11].check) memoryList[11].check = TRUE;
    if (selectedSkill[1] == 630 && !memoryList[12].check) memoryList[12].check = TRUE;
    if (selectedSkill[2] / 100 == 7 && !memoryList[13].check) memoryList[13].check = TRUE;
    if (selectedSkill[2] / 100 == 8 && !memoryList[14].check) memoryList[14].check = TRUE;
    if (selectedSkill[2] / 100 == 9 && !memoryList[15].check) memoryList[15].check = TRUE;
    if (score >= 1000 && !memoryList[16].check) memoryList[16].check = TRUE;
    if (score >= 4000 && !memoryList[17].check) memoryList[17].check = TRUE;
    if (score >= 10000 && !memoryList[18].check) memoryList[18].check = TRUE;
}
