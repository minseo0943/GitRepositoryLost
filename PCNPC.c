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
#include "PCNPC.h"
#include "Skill.h"
int ChainsawTouched[6] = { 0 };
int NpcInfo[maxmob][2] = { 0 };
int RangeNpcInfo[maxmob][2] = { 0 };
int StopRangeNpcInfo[maxmob][3] = { 0 };
int PCPoint[2] = { 10,10 };
int ProjectileInfo[maxProjectile][3] = { 0 };

int TimeCount = 0;
int AttackCount = 0;
int item = 0; //드랍 아이템 번호
int CHAINSAWindex;
COORD pc; //pc 좌표

COORD RandomSideXY() {
    COORD side;
    int ran;
    while (1) {
        ran = rand() % 4;

        //npc 생성 좌표 지정
        switch (ran)
        {
        default:
            side.X = 1;
            side.Y = 1;
            break;
        case 0: //왼변
            side.X = 1;
            side.Y = rand() % (GBOARD_HEIGHT - 1) + 1;
            break;
        case 1: //우변
            side.X = GBOARD_WIDTH - 1;
            side.Y = rand() % (GBOARD_HEIGHT - 1) + 1;
            break;
        case 2: //상변
            side.X = rand() % (GBOARD_WIDTH - 1) + 1;
            side.Y = 1;
            break;
        case 3: //하변
            side.X = rand() % (GBOARD_WIDTH - 1) + 1;
            side.Y = GBOARD_HEIGHT - 1;
            break;
        }

        if (GameBoardInfo[side.Y][side.X] == 0) {
            /*SetCurrentCursorPos(0, 0);
            printf("%d, %d", side.X, side.Y);*/
            return side;
        }
    }
}

void NpcSpawn()  //랜덤으로 npc 생성 (1~width-1),(1~height-1) 좌표에
{
    COORD side = RandomSideXY();

    for (int i = 0; i < maxmob; i++) //npc최대 수만큼 차면 더 이상 생성 x
    {
        if (NpcInfo[i][0] != 0) continue;

        NpcInfo[i][0] = side.X, NpcInfo[i][1] = side.Y;
        GameBoardInfo[side.Y][side.X] = NPC;

        SetCurrentCursorPos(NpcInfo[i][0] * 2 + GBOARD_ORIGIN_X, NpcInfo[i][1] + GBOARD_ORIGIN_Y);
        drawNpcShape();
        break;
    }
}

void RangeNpcSpawn() // 원거리 npc 생성  테두리 쪽에서 랜덤 생성
{
    COORD side = RandomSideXY();

    for (int i = 0; i < maxmob; i++) //npc최대 수만큼 차면 더 이상 생성 x
    {
        if (RangeNpcInfo[i][0] != 0 || StopRangeNpcInfo[i][0] != 0) continue;

        RangeNpcInfo[i][0] = side.X, RangeNpcInfo[i][1] = side.Y;
        GameBoardInfo[side.Y][side.X] = MOVINGRANGENPC;

        SetCurrentCursorPos(RangeNpcInfo[i][0] * 2 + GBOARD_ORIGIN_X, RangeNpcInfo[i][1] + GBOARD_ORIGIN_Y);
        drawRangeNpcShape();
        break;
    }
}

void NpcMove()   // NPC 이동
{
    int x, y;
    int a = 0, b = 0;    //a,b 는 1과 0의 쌍  
    int ran;
    int PCPointX = 0;
    int PCPointY = 0;

    for (int i = 0; i < maxmob; i++)
    {
        if (NpcInfo[i][0] != 0)
        {
            PCPointX = PCPoint[0];
            PCPointY = PCPoint[1];

            x = NpcInfo[i][0], y = NpcInfo[i][1];  //npc가 이동할 위치 x,y
            SetCurrentCursorPos(NpcInfo[i][0] * 2 + GBOARD_ORIGIN_X, NpcInfo[i][1] + GBOARD_ORIGIN_Y);
            printf("  ");
            GameBoardInfo[NpcInfo[i][1]][NpcInfo[i][0]] = 0;

            if (PCPointX + 1 <= x)PCPointX++;
            if (PCPointY + 1 <= y)PCPointY++;

            if (PCPointX > NpcInfo[i][0])x++;
            else if (PCPointX < NpcInfo[i][0])x--;
            if (PCPointY > NpcInfo[i][1])y++;
            else if (PCPointY < NpcInfo[i][1])y--;

            if (GameBoardInfo[y][x] == PC)
            {
                NpcDie(NpcInfo[i][0], NpcInfo[i][1]);
                // pc와 충돌 -> pc HP 마이너스 시키기
                DecreaseHP();
                continue;
            } 
            //npc, rangenpc, stoprangenpc, wall, life, bomb 충돌 시 랜덤의 방향으로 돌아가기
            else if (GameBoardInfo[y][x] == NPC || GameBoardInfo[y][x] == MOVINGRANGENPC || GameBoardInfo[y][x] == STOPPEDRANGENPC || GameBoardInfo[y][x] == WALL || GameBoardInfo[y][x] == LIFE || GameBoardInfo[y][x] == BOMB || GameBoardInfo[y][x] == CHAINSAW)
            {
                ran = rand() % 4;
                if (ran == 0) { a = 1; b = 0; }
                else if (ran == 1) { a = 0; b = 1; }
                else if (ran == 2) { a = -1; b = 0; }
                else if (ran == 3) { a = 0; b = -1; }
                x = NpcInfo[i][0] + a;
                y = NpcInfo[i][1] + b;

                //npc, rangenpc, stoprangenpc, wall, life, bomb 충돌
                if (GameBoardInfo[y][x] == NPC || GameBoardInfo[y][x] == MOVINGRANGENPC || GameBoardInfo[y][x] == STOPPEDRANGENPC || GameBoardInfo[y][x] == WALL || GameBoardInfo[y][x] == LIFE || GameBoardInfo[y][x] == BOMB || GameBoardInfo[y][x] == CHAINSAW)
                {
                    x = NpcInfo[i][0] + b;
                    y = NpcInfo[i][1] + a;
                }
                if (GameBoardInfo[y][x] == NPC || GameBoardInfo[y][x] == MOVINGRANGENPC || GameBoardInfo[y][x] == STOPPEDRANGENPC || GameBoardInfo[y][x] == WALL || GameBoardInfo[y][x] == LIFE || GameBoardInfo[y][x] == BOMB || GameBoardInfo[y][x] == CHAINSAW)
                {
                    x = NpcInfo[i][0] - a;
                    y = NpcInfo[i][1] - b;
                }
                if (GameBoardInfo[y][x] == NPC || GameBoardInfo[y][x] == MOVINGRANGENPC || GameBoardInfo[y][x] == STOPPEDRANGENPC || GameBoardInfo[y][x] == WALL || GameBoardInfo[y][x] == LIFE || GameBoardInfo[y][x] == BOMB || GameBoardInfo[y][x] == CHAINSAW)
                {
                    x = NpcInfo[i][0] - b;
                    y = NpcInfo[i][1] - a;           //원상복구  아무 방향에 갈 곳 없으면 가만히 있기 
                }
                if (GameBoardInfo[y][x] == NPC || GameBoardInfo[y][x] == MOVINGRANGENPC || GameBoardInfo[y][x] == STOPPEDRANGENPC || GameBoardInfo[y][x] == WALL || GameBoardInfo[y][x] == LIFE || GameBoardInfo[y][x] == BOMB || GameBoardInfo[y][x] == CHAINSAW)
                {
                    x = NpcInfo[i][0];
                    y = NpcInfo[i][1];
                }
            }
            else if (GameBoardInfo[y][x] > 100) {
                //스킬 이펙트와 충돌
                if (GameBoardInfo[y][x] < 300) {
                    if (GameBoardInfo[y][x] < 400) {
                        for (int i = 0; i <= skillTop; i++) {
                            if (skillEffectInfo[i].X == x && skillEffectInfo[i].Y == y) {
                                RemoveSkillEffect(i);
                                break;
                            }
                        }
                    }
                    else {
                        GameBoardInfo[y][x] = 0;
                        SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + y);
                        printf("  ");
                    }
                }
                NpcDie(NpcInfo[i][0], NpcInfo[i][1]);
                continue;
            }
            NpcInfo[i][0] = x;
            NpcInfo[i][1] = y;

            GameBoardInfo[NpcInfo[i][1]][NpcInfo[i][0]] = NPC;
            SetCurrentCursorPos(NpcInfo[i][0] * 2 + GBOARD_ORIGIN_X, NpcInfo[i][1] + GBOARD_ORIGIN_Y);
            drawNpcShape();
        }
    }
}

void RangeNpcMove() //생성되고 한 턴후 부딪히는 거 처리  처음에 바로안멈춤
{
    int x, y;
    int a = 0, b = 0;    //a,b 는 1과 0의 쌍
    int ran;
    int PCPointX = 0;
    int PCPointY = 0;

    for (int i = 0; i < maxmob; i++)
    {
        if (RangeNpcInfo[i][0] != 0)
        {
            PCPointX = PCPoint[0];
            PCPointY = PCPoint[1];

            x = RangeNpcInfo[i][0], y = RangeNpcInfo[i][1];
            SetCurrentCursorPos(RangeNpcInfo[i][0] * 2 + GBOARD_ORIGIN_X, RangeNpcInfo[i][1] + GBOARD_ORIGIN_Y);
            printf("  ");
            GameBoardInfo[RangeNpcInfo[i][1]][RangeNpcInfo[i][0]] = 0;

            if (PCPointX + 1 <= x)PCPointX++;
            if (PCPointY + 1 <= y)PCPointY++;

            if (PCPointX > RangeNpcInfo[i][0])x++;
            else if (PCPointX < RangeNpcInfo[i][0])x--;
            if (PCPointY > RangeNpcInfo[i][1])y++;
            else if (PCPointY < RangeNpcInfo[i][1])y--;

            if (GameBoardInfo[y][x] == PC)
            {
                //RangeNpcDie(RangeNpcInfo[i][0], RangeNpcInfo[i][1]);
                DecreaseHP(); // pc와 충돌 -> pc HP 마이너스 시키기
                continue;
            }
            else if (GameBoardInfo[y][x] == NPC || GameBoardInfo[y][x] == MOVINGRANGENPC || GameBoardInfo[y][x] == STOPPEDRANGENPC || GameBoardInfo[y][x] == WALL || GameBoardInfo[y][x] == LIFE || GameBoardInfo[y][x] == BOMB || GameBoardInfo[y][x] == CHAINSAW)  // npc 가는 위치 막혀있으면 다른 길로 가기    //랜덤으로 구현해놨지만 거리 재서 가장 가까운 곳으로 가기 안되면 가만히 있기
            {
                ran = rand() % 4;
                if (ran == 0) { a = 1; b = 0; }
                else if (ran == 1) { a = 0; b = 1; }
                else if (ran == 2) { a = -1; b = 0; }
                else if (ran == 3) { a = 0; b = -1; }
                x = RangeNpcInfo[i][0] + a;
                y = RangeNpcInfo[i][1] + b;
                if (GameBoardInfo[y][x] == NPC || GameBoardInfo[y][x] == MOVINGRANGENPC || GameBoardInfo[y][x] == STOPPEDRANGENPC || GameBoardInfo[y][x] == WALL || GameBoardInfo[y][x] == LIFE || GameBoardInfo[y][x] == BOMB || GameBoardInfo[y][x] == CHAINSAW)
                {
                    x = RangeNpcInfo[i][0] + b;
                    y = RangeNpcInfo[i][1] + a;
                }
                if (GameBoardInfo[y][x] == NPC || GameBoardInfo[y][x] == MOVINGRANGENPC || GameBoardInfo[y][x] == STOPPEDRANGENPC || GameBoardInfo[y][x] == WALL || GameBoardInfo[y][x] == LIFE || GameBoardInfo[y][x] == BOMB || GameBoardInfo[y][x] == CHAINSAW)
                {
                    x = RangeNpcInfo[i][0] - a;
                    y = RangeNpcInfo[i][1] - b;
                }
                if (GameBoardInfo[y][x] == NPC || GameBoardInfo[y][x] == MOVINGRANGENPC || GameBoardInfo[y][x] == STOPPEDRANGENPC || GameBoardInfo[y][x] == WALL || GameBoardInfo[y][x] == LIFE || GameBoardInfo[y][x] == BOMB || GameBoardInfo[y][x] == CHAINSAW)
                {
                    x = RangeNpcInfo[i][0] - b;
                    y = RangeNpcInfo[i][1] - a;
                }
                if (GameBoardInfo[y][x] == NPC || GameBoardInfo[y][x] == MOVINGRANGENPC || GameBoardInfo[y][x] == STOPPEDRANGENPC || GameBoardInfo[y][x] == WALL || GameBoardInfo[y][x] == LIFE || GameBoardInfo[y][x] == BOMB || GameBoardInfo[y][x] == CHAINSAW)
                {

                    x = RangeNpcInfo[i][0];
                    y = RangeNpcInfo[i][1];           //원상복구
                }
            }
            else if (GameBoardInfo[y][x] > 100) {
                //스킬 이펙트와 충돌
                if (GameBoardInfo[y][x] < 300) {
                    if (GameBoardInfo[y][x] < 400) {
                        for (int i = 0; i <= skillTop; i++) {
                            if (skillEffectInfo[i].X == x && skillEffectInfo[i].Y == y) {
                                RemoveSkillEffect(i);
                                break;
                            }
                        }
                    }
                    else {
                        GameBoardInfo[y][x] = 0;
                        SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + y);
                        printf("  ");
                    }
                }
                RangeNpcDie(RangeNpcInfo[i][0], RangeNpcInfo[i][1]);
                continue;
            }
            RangeNpcInfo[i][0] = x;
            RangeNpcInfo[i][1] = y;

            GameBoardInfo[RangeNpcInfo[i][1]][RangeNpcInfo[i][0]] = MOVINGRANGENPC;
            SetCurrentCursorPos(RangeNpcInfo[i][0] * 2 + GBOARD_ORIGIN_X, RangeNpcInfo[i][1] + GBOARD_ORIGIN_Y);
            drawRangeNpcShape();

            // 거리 10보다 작으면 멈춤 처리(stop 원거리 npc 배열에 추가) 
            if (DistanceSquare(PCPoint[0], PCPoint[1], RangeNpcInfo[i][0], RangeNpcInfo[i][1]) < 10 * 10)
            {
                GameBoardInfo[RangeNpcInfo[i][1]][RangeNpcInfo[i][0]] = STOPPEDRANGENPC;
                SetCurrentCursorPos(RangeNpcInfo[i][0] * 2 + GBOARD_ORIGIN_X, RangeNpcInfo[i][1] + GBOARD_ORIGIN_Y);
                drawStopRangeNpcShape();
                if (StopRangeNpcInfo[i][0] == 0)
                {
                    StopRangeNpcInfo[i][0] = RangeNpcInfo[i][0];
                    StopRangeNpcInfo[i][1] = RangeNpcInfo[i][1];

                    RangeNpcInfo[i][0] = 0;
                    RangeNpcInfo[i][1] = 0;
                    break;
                }
            }
        }
    }
}

void NpcDie(int Npc_X, int Npc_Y) // 그 자리에 npc 죽이고  npc인포 최신화
{
    int ran = rand();
    if (Npc_X == 0 || Npc_X == GBOARD_WIDTH || Npc_Y == 0 || Npc_Y == GBOARD_HEIGHT) return;

    //아이템 드랍
    SetCurrentCursorPos(Npc_X * 2 + GBOARD_ORIGIN_X, Npc_Y + GBOARD_ORIGIN_Y);
    if (ran % 100 < Diff.itemRate) {
        //아이템 그리고 게임판에 추가
        GameBoardInfo[Npc_Y][Npc_X] = DropItem();
    }
    else {
        //칸 비우고 게임판에서 삭제
        printf("  ");
        GameBoardInfo[Npc_Y][Npc_X] = 0;
    }

    for (int i = 0; i < maxmob; i++)
    {
        if (NpcInfo[i][0] == Npc_X && NpcInfo[i][1] == Npc_Y)
        {
            NpcInfo[i][0] = 0;
            break;
        }
    }
}

void RangeNpcDie(int Npc_X, int Npc_Y) // 위와 동일
{
    int ran = rand();
    if (Npc_X == 0 || Npc_X == GBOARD_WIDTH || Npc_Y == 0 || Npc_Y == GBOARD_HEIGHT) return;

    SetCurrentCursorPos(Npc_X * 2 + GBOARD_ORIGIN_X, Npc_Y + GBOARD_ORIGIN_Y);
    
    //아이템 드랍
    if (ran % 100 < Diff.itemRate) {
        //아이템 그리고 게임판에 추가
        GameBoardInfo[Npc_Y][Npc_X] = DropItem();
    }
    else {
        //칸 비우고 게임판에서 삭제
        printf("  ");
        GameBoardInfo[Npc_Y][Npc_X] = 0;
    }

    for (int i = 0; i < maxmob; i++)
    {
        if (RangeNpcInfo[i][0] == Npc_X && RangeNpcInfo[i][1] == Npc_Y)
        {
            RangeNpcInfo[i][0] = 0;
            break;
        }
    }
}

void StopRangeNpcDie(int Npc_X, int Npc_Y) //위와 동일
{
    int ran = rand();
    if (Npc_X == 0 || Npc_X == GBOARD_WIDTH || Npc_Y == 0 || Npc_Y == GBOARD_HEIGHT) return;

    SetCurrentCursorPos(Npc_X * 2 + GBOARD_ORIGIN_X, Npc_Y + GBOARD_ORIGIN_Y);
    
    //아이템 드랍
    if (ran % 100 < Diff.itemRate) {
        //아이템 그리고 게임판에 추가
        GameBoardInfo[Npc_Y][Npc_X] = DropItem();
    }
    else {
        //칸 비우고 게임판에서 삭제
        printf("  ");
        GameBoardInfo[Npc_Y][Npc_X] = 0;
    }

    for (int i = 0; i < maxmob; i++)
    {
        if (StopRangeNpcInfo[i][0] == Npc_X && StopRangeNpcInfo[i][1] == Npc_Y)
        {
            StopRangeNpcInfo[i][0] = 0;
            break;
        }
    }
}

int DistanceSquare(int x1, int y1, int x2, int y2)  //거리 계산
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

void RangeNpcAttack()  //원거리 npc 공격
{
    for (int i = 0; i < maxmob; i++)
    {
        if (StopRangeNpcInfo[i][0] == 0) continue;  //멈춘 원거리 npc 없으면 continue
        
        for (int j = 0; j < maxProjectile; j++)  //투사체 배열에 공간이 남아 있으면
        {
            if (ProjectileInfo[j][0] != 0) continue; //예외 처리

            //공격 방향 업데이트
            RangeNpcAttackDirectionUpdate();

            //투사체 배열에 StopRangeNpc좌표 저장
            ProjectileInfo[j][0] = StopRangeNpcInfo[i][0];
            ProjectileInfo[j][1] = StopRangeNpcInfo[i][1];

            //투사체 방향 저장하고 좌표 업데이트
            switch (StopRangeNpcInfo[i][2])
            {
            default:
                break;
            case UP:
                ProjectileInfo[j][2] = UP;
                ProjectileInfo[j][1]--;
                break;
            case DOWN:
                ProjectileInfo[j][2] = DOWN;
                ProjectileInfo[j][1]++;
                break;
            case LEFT:
                ProjectileInfo[j][2] = LEFT;
                ProjectileInfo[j][0]--;
                break;
            case RIGHT:
                ProjectileInfo[j][2] = RIGHT;
                ProjectileInfo[j][0]++;
                break;
            }

            //투사체 충돌 검사 및 추가 시도
            if (GameBoardInfo[ProjectileInfo[j][1]][ProjectileInfo[j][0]] == 0 || GameBoardInfo[ProjectileInfo[j][1]][ProjectileInfo[j][0]] > 100) {
                //비어있으면 npcbullet 저장하고 그리기
                SetCurrentCursorPos(ProjectileInfo[j][0] * 2 + GBOARD_ORIGIN_X, ProjectileInfo[j][1] + GBOARD_ORIGIN_Y);
                textcolor(darkRED);
                switch (ProjectileInfo[j][2])
                {
                default:
                    break;
                case UP:
                    printf("↑");
                    break;
                case DOWN:
                    printf("↓");
                    break;
                case LEFT:
                    printf("←");
                    break;
                case RIGHT:
                    printf("→");
                    break;
                }

                if (GameBoardInfo[ProjectileInfo[j][1]][ProjectileInfo[j][0]] > 100) {
                    if (GameBoardInfo[ProjectileInfo[j][1]][ProjectileInfo[j][0]] < 300) {
                        if (GameBoardInfo[ProjectileInfo[j][1]][ProjectileInfo[j][0]] < 400) {
                            for (int k = 0; k <= skillTop; k++) {
                                if (skillEffectInfo[k].X == ProjectileInfo[j][0] && skillEffectInfo[k].Y == ProjectileInfo[j][1]) {
                                    RemoveSkillEffect(k);
                                    break;
                                }
                            }
                        }
                    }
                    ProjectileExtinction(ProjectileInfo[j][0], ProjectileInfo[j][1]);
                }
                else GameBoardInfo[ProjectileInfo[j][1]][ProjectileInfo[j][0]] = NPCBULLET;
            }
            else if (GameBoardInfo[ProjectileInfo[j][1]][ProjectileInfo[j][0]] == LIFE || GameBoardInfo[ProjectileInfo[j][1]][ProjectileInfo[j][0]] == BOMB) {
                //아이템일 경우 npcbullet 및 아이템 삭제
                ProjectileExtinction(ProjectileInfo[j][0], ProjectileInfo[j][1]);
            }
            else if (GameBoardInfo[ProjectileInfo[j][1]][ProjectileInfo[j][0]] == PC) {
                //pc일 경우 투사체 삭제하고 hp 감소
                ProjectileExtinction(ProjectileInfo[j][0], ProjectileInfo[j][1]);
                DecreaseHP();
            }
            else if (GameBoardInfo[ProjectileInfo[j][1]][ProjectileInfo[j][0]] == WALL || GameBoardInfo[ProjectileInfo[j][1]][ProjectileInfo[j][0]] == CHAINSAW) {
                //벽일 경우 투사체 삭제
                ProjectileExtinction(ProjectileInfo[j][0], ProjectileInfo[j][1]);
            }
            else {
                //나머지의 경우 (투사체 저장만 하고 그리지 않기) = (넘기기)
            }

            break;
        }
    }
}

void ProjectileMove()   //적과 마주치면 지우고 projectile만 업그레이드, 적이 없으면 gameboardinfo 최신화 and 그리기, pc나 벽과 마주치면 projectile 소멸
{
    for (int i = 0; i < maxProjectile; i++) {
        if (ProjectileInfo[i][0] == 0) continue; //예외 처리

        if (GameBoardInfo[ProjectileInfo[i][1]][ProjectileInfo[i][0]] == NPCBULLET) {
            SetCurrentCursorPos(ProjectileInfo[i][0] * 2 + GBOARD_ORIGIN_X, ProjectileInfo[i][1] + GBOARD_ORIGIN_Y);
            printf("  ");
            GameBoardInfo[ProjectileInfo[i][1]][ProjectileInfo[i][0]] = 0;
        }

        switch (ProjectileInfo[i][2])
        {
        default:
            break;
        case UP:
            ProjectileInfo[i][1]--;
            break;
        case DOWN:
            ProjectileInfo[i][1]++;
            break;
        case LEFT:
            ProjectileInfo[i][0]--;
            break;
        case RIGHT:
            ProjectileInfo[i][0]++;
            break;
        }

        switch (GameBoardInfo[ProjectileInfo[i][1]][ProjectileInfo[i][0]])
        {
        default:
            if (GameBoardInfo[ProjectileInfo[i][1]][ProjectileInfo[i][0]] > 100) {
                //스킬 이펙트와 충돌
                if (GameBoardInfo[ProjectileInfo[i][1]][ProjectileInfo[i][0]] < 300) {
                    if (GameBoardInfo[ProjectileInfo[i][1]][ProjectileInfo[i][0]] < 400) {
                        for (int k = 0; k <= skillTop; k++) {
                            if (skillEffectInfo[k].X == ProjectileInfo[i][0] && skillEffectInfo[k].Y == ProjectileInfo[i][1]) {
                                RemoveSkillEffect(k);
                                break;
                            }
                        }
                    }
                }
                ProjectileExtinction(ProjectileInfo[i][0], ProjectileInfo[i][1]);
                continue;
            }
            else if (GameBoardInfo[ProjectileInfo[i][1]][ProjectileInfo[i][0]] == LIFE || GameBoardInfo[ProjectileInfo[i][1]][ProjectileInfo[i][0]] == BOMB) {
                //아이템과 충돌 시 아이템과 투사체 모두 삭제 -> ProjectileExtinction 함수로 전부 수행함
                ProjectileExtinction(ProjectileInfo[i][0], ProjectileInfo[i][1]);
                continue;
            }
            break;

        case WALL:
            ProjectileExtinction(ProjectileInfo[i][0], ProjectileInfo[i][1]);
            continue;
            break;
        case CHAINSAW:
            ProjectileExtinction(ProjectileInfo[i][0], ProjectileInfo[i][1]);
            continue;
            break;
        case PC:
            DecreaseHP();
            ProjectileExtinction(ProjectileInfo[i][0], ProjectileInfo[i][1]);
            continue;
            break;
        case NPC:
            continue;
            break;
        case MOVINGRANGENPC:
            continue;
            break;
        case STOPPEDRANGENPC:
            continue;
            break;
        }

        GameBoardInfo[ProjectileInfo[i][1]][ProjectileInfo[i][0]] = NPCBULLET;
        SetCurrentCursorPos(ProjectileInfo[i][0] * 2 + GBOARD_ORIGIN_X, ProjectileInfo[i][1] + GBOARD_ORIGIN_Y);
        textcolor(darkRED);
        switch (ProjectileInfo[i][2])
        {
        default:
            break;
        case UP:
            printf("↑");
            break;
        case DOWN:
            printf("↓");
            break;
        case LEFT:
            printf("←");
            break;
        case RIGHT:
            printf("→");
            break;
        }
    }
}

void ProjectileExtinction(int x, int y) // 투사체 소멸
{
    for (int i = 0; i < maxProjectile; i++)
    {
        if (ProjectileInfo[i][0] == x && ProjectileInfo[i][1] == y)
        {
            if (GameBoardInfo[y][x] != 2 && GameBoardInfo[y][x] != WALL&&GameBoardInfo[y][x]!=CHAINSAW)
            {
                SetCurrentCursorPos(ProjectileInfo[i][0] * 2 + GBOARD_ORIGIN_X, ProjectileInfo[i][1] + GBOARD_ORIGIN_Y);
                printf("  ");
                GameBoardInfo[ProjectileInfo[i][1]][ProjectileInfo[i][0]] = 0;
            }
            ProjectileInfo[i][0] = 0;
            ProjectileInfo[i][1] = 0;
            ProjectileInfo[i][2] = 0;
        }
    }
}

void PcShiftNext(int x, int y) {
    //예외 처리
    if (x < 0 || x > GBOARD_WIDTH || y < 0 || y > GBOARD_HEIGHT) return; //게임판 내부를 벗어나면 이동하지 않기
    
    //충돌 처리
    for (int ny = y; ny < y + 2; ny++) {
        for (int nx = x; nx < x + 2; nx++) {
            switch (GameBoardInfo[ny][nx])
            {
            default: //기타; PC, 이펙트 등
                //아이템 사용
                if (GameBoardInfo[ny][nx] == LIFE || GameBoardInfo[ny][nx] == BOMB) UseItem(GameBoardInfo[ny][nx]);
                //스킬 그대로 두고 밑으로 이동
                break;
            case 0: //아무것도 없을 경우 밑으로 이동
                break;
            case WALL: //벽일 경우 함수 종료
                return;
                break;
            case CHAINSAW:
                DecreaseHP();
                if (x < 20)
                {
                    if (y < 20)CHAINSAWindex = 0;
                    else CHAINSAWindex = 3;
                }
                else if(x<40)
                {
                    if (y < 20)CHAINSAWindex = 1;
                    else CHAINSAWindex = 4;
                }
                else
                {
                    if (y < 20)CHAINSAWindex = 2;
                    else CHAINSAWindex = 5;
                }
                ChainsawTouched[CHAINSAWindex] = 4;
                return;
                break;
            case NPC: //npc일 경우 NpcDie 실행하고 hp 감소하고 밑으로 이동
                NpcDie(nx, ny);
                DecreaseHP();
                //아이템 사용
                if (GameBoardInfo[ny][nx] == LIFE || GameBoardInfo[ny][nx] == BOMB) UseItem(GameBoardInfo[ny][nx]);
                break;
            case MOVINGRANGENPC: //npc와 동일
                RangeNpcDie(nx, ny);
                DecreaseHP();
                //아이템 사용
                if (GameBoardInfo[ny][nx] == LIFE || GameBoardInfo[ny][nx] == BOMB) UseItem(GameBoardInfo[ny][nx]);
                break;
            case STOPPEDRANGENPC: //npc와 동일
                StopRangeNpcDie(nx, ny);
                DecreaseHP();
                //아이템 사용
                if (GameBoardInfo[ny][nx] == LIFE || GameBoardInfo[ny][nx] == BOMB) UseItem(GameBoardInfo[ny][nx]);
                break;
            case NPCBULLET: //적 투사체일 경우 투사체 없애고 hp 감소하고 밑으로 이동
                ProjectileExtinction(nx, ny);
                DecreaseHP();
                break;
            case BossNum: //보스일 경우 pc hp 감소하고 종료
                DecreaseHP();
                return;
                break;
            case BossSkillEffect: //보스 스킬일 경우 스킬 없어지고 pc hp 감소하고 밑으로 이동
                DecreaseHP();
                break;
            }
        }
    }

    //pc 이동 구현
    //현재 pc 지우기
    erasePC();

    //다음 pc 그리기
    PCPoint[0] = x;
    PCPoint[1] = y;
    drawPC();
}

void PcShiftLeft() {
    PcShiftNext(PCPoint[0] - 1, PCPoint[1]);
}

void PcShiftRight() {
    PcShiftNext(PCPoint[0] + 1, PCPoint[1]);
}

void PcShiftUp() {
    PcShiftNext(PCPoint[0], PCPoint[1] - 1);
}

void PcShiftDown() {
    PcShiftNext(PCPoint[0], PCPoint[1] + 1);
}

//원거리 npc 공격방향 최신화
void RangeNpcAttackDirectionUpdate() { 
    int PCPointX;
    int PCPointY;

    for (int i = 0; i < maxmob; i++) {
        if (StopRangeNpcInfo[i][0] != 0) {
            PCPointX = PCPoint[0];
            PCPointY = PCPoint[1];

            if (PCPointX < StopRangeNpcInfo[i][0])PCPointX++;
            if (PCPointY < StopRangeNpcInfo[i][0])PCPointY++;

            if ((PCPointX - StopRangeNpcInfo[i][0]) * (PCPointX - StopRangeNpcInfo[i][0]) - (PCPointY - StopRangeNpcInfo[i][1]) * (PCPointY - StopRangeNpcInfo[i][1]) > 0) {
                if (PCPointX > StopRangeNpcInfo[i][0]) StopRangeNpcInfo[i][2] = RIGHT;
                else StopRangeNpcInfo[i][2] = LEFT;
            }
            else {
                if (PCPointY < StopRangeNpcInfo[i][1]) StopRangeNpcInfo[i][2] = UP;
                else StopRangeNpcInfo[i][2] = DOWN;
            }
        }
    }
}

void AllDie()
{ 
    for (int i = 0; i < maxmob; i++) {
        if (GameBoardInfo[NpcInfo[i][1]][NpcInfo[i][0]] == NPC) {
            IncreaseScore();
        }
        NpcDie(NpcInfo[i][0], NpcInfo[i][1]);
    }
    for (int i = 0; i < maxmob; i++) {
        if (GameBoardInfo[RangeNpcInfo[i][1]][RangeNpcInfo[i][0]] == MOVINGRANGENPC) {
            IncreaseScore();
        }
        RangeNpcDie(RangeNpcInfo[i][0], RangeNpcInfo[i][1]);
    }
    for (int i = 0; i < maxmob; i++) {
        if (GameBoardInfo[StopRangeNpcInfo[i][1]][StopRangeNpcInfo[i][0]] == STOPPEDRANGENPC) {
            IncreaseScore();
        }
        StopRangeNpcDie(StopRangeNpcInfo[i][0], StopRangeNpcInfo[i][1]);
    }
    for (int i = 0; i < maxProjectile; i++) {
        ProjectileExtinction(ProjectileInfo[i][0], ProjectileInfo[i][1]);
    }
}
