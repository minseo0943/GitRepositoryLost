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
int item = 0; //��� ������ ��ȣ
int CHAINSAWindex;
COORD pc; //pc ��ǥ

COORD RandomSideXY() {
    COORD side;
    int ran;
    while (1) {
        ran = rand() % 4;

        //npc ���� ��ǥ ����
        switch (ran)
        {
        default:
            side.X = 1;
            side.Y = 1;
            break;
        case 0: //�޺�
            side.X = 1;
            side.Y = rand() % (GBOARD_HEIGHT - 1) + 1;
            break;
        case 1: //�캯
            side.X = GBOARD_WIDTH - 1;
            side.Y = rand() % (GBOARD_HEIGHT - 1) + 1;
            break;
        case 2: //��
            side.X = rand() % (GBOARD_WIDTH - 1) + 1;
            side.Y = 1;
            break;
        case 3: //�Ϻ�
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

void NpcSpawn()  //�������� npc ���� (1~width-1),(1~height-1) ��ǥ��
{
    COORD side = RandomSideXY();

    for (int i = 0; i < maxmob; i++) //npc�ִ� ����ŭ ���� �� �̻� ���� x
    {
        if (NpcInfo[i][0] != 0) continue;

        NpcInfo[i][0] = side.X, NpcInfo[i][1] = side.Y;
        GameBoardInfo[side.Y][side.X] = NPC;

        SetCurrentCursorPos(NpcInfo[i][0] * 2 + GBOARD_ORIGIN_X, NpcInfo[i][1] + GBOARD_ORIGIN_Y);
        drawNpcShape();
        break;
    }
}

void RangeNpcSpawn() // ���Ÿ� npc ����  �׵θ� �ʿ��� ���� ����
{
    COORD side = RandomSideXY();

    for (int i = 0; i < maxmob; i++) //npc�ִ� ����ŭ ���� �� �̻� ���� x
    {
        if (RangeNpcInfo[i][0] != 0 || StopRangeNpcInfo[i][0] != 0) continue;

        RangeNpcInfo[i][0] = side.X, RangeNpcInfo[i][1] = side.Y;
        GameBoardInfo[side.Y][side.X] = MOVINGRANGENPC;

        SetCurrentCursorPos(RangeNpcInfo[i][0] * 2 + GBOARD_ORIGIN_X, RangeNpcInfo[i][1] + GBOARD_ORIGIN_Y);
        drawRangeNpcShape();
        break;
    }
}

void NpcMove()   // NPC �̵�
{
    int x, y;
    int a = 0, b = 0;    //a,b �� 1�� 0�� ��  
    int ran;
    int PCPointX = 0;
    int PCPointY = 0;

    for (int i = 0; i < maxmob; i++)
    {
        if (NpcInfo[i][0] != 0)
        {
            PCPointX = PCPoint[0];
            PCPointY = PCPoint[1];

            x = NpcInfo[i][0], y = NpcInfo[i][1];  //npc�� �̵��� ��ġ x,y
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
                // pc�� �浹 -> pc HP ���̳ʽ� ��Ű��
                DecreaseHP();
                continue;
            } 
            //npc, rangenpc, stoprangenpc, wall, life, bomb �浹 �� ������ �������� ���ư���
            else if (GameBoardInfo[y][x] == NPC || GameBoardInfo[y][x] == MOVINGRANGENPC || GameBoardInfo[y][x] == STOPPEDRANGENPC || GameBoardInfo[y][x] == WALL || GameBoardInfo[y][x] == LIFE || GameBoardInfo[y][x] == BOMB || GameBoardInfo[y][x] == CHAINSAW)
            {
                ran = rand() % 4;
                if (ran == 0) { a = 1; b = 0; }
                else if (ran == 1) { a = 0; b = 1; }
                else if (ran == 2) { a = -1; b = 0; }
                else if (ran == 3) { a = 0; b = -1; }
                x = NpcInfo[i][0] + a;
                y = NpcInfo[i][1] + b;

                //npc, rangenpc, stoprangenpc, wall, life, bomb �浹
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
                    y = NpcInfo[i][1] - a;           //���󺹱�  �ƹ� ���⿡ �� �� ������ ������ �ֱ� 
                }
                if (GameBoardInfo[y][x] == NPC || GameBoardInfo[y][x] == MOVINGRANGENPC || GameBoardInfo[y][x] == STOPPEDRANGENPC || GameBoardInfo[y][x] == WALL || GameBoardInfo[y][x] == LIFE || GameBoardInfo[y][x] == BOMB || GameBoardInfo[y][x] == CHAINSAW)
                {
                    x = NpcInfo[i][0];
                    y = NpcInfo[i][1];
                }
            }
            else if (GameBoardInfo[y][x] > 100) {
                //��ų ����Ʈ�� �浹
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

void RangeNpcMove() //�����ǰ� �� ���� �ε����� �� ó��  ó���� �ٷξȸ���
{
    int x, y;
    int a = 0, b = 0;    //a,b �� 1�� 0�� ��
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
                DecreaseHP(); // pc�� �浹 -> pc HP ���̳ʽ� ��Ű��
                continue;
            }
            else if (GameBoardInfo[y][x] == NPC || GameBoardInfo[y][x] == MOVINGRANGENPC || GameBoardInfo[y][x] == STOPPEDRANGENPC || GameBoardInfo[y][x] == WALL || GameBoardInfo[y][x] == LIFE || GameBoardInfo[y][x] == BOMB || GameBoardInfo[y][x] == CHAINSAW)  // npc ���� ��ġ ���������� �ٸ� ��� ����    //�������� �����س����� �Ÿ� �缭 ���� ����� ������ ���� �ȵǸ� ������ �ֱ�
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
                    y = RangeNpcInfo[i][1];           //���󺹱�
                }
            }
            else if (GameBoardInfo[y][x] > 100) {
                //��ų ����Ʈ�� �浹
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

            // �Ÿ� 10���� ������ ���� ó��(stop ���Ÿ� npc �迭�� �߰�) 
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

void NpcDie(int Npc_X, int Npc_Y) // �� �ڸ��� npc ���̰�  npc���� �ֽ�ȭ
{
    int ran = rand();
    if (Npc_X == 0 || Npc_X == GBOARD_WIDTH || Npc_Y == 0 || Npc_Y == GBOARD_HEIGHT) return;

    //������ ���
    SetCurrentCursorPos(Npc_X * 2 + GBOARD_ORIGIN_X, Npc_Y + GBOARD_ORIGIN_Y);
    if (ran % 100 < Diff.itemRate) {
        //������ �׸��� �����ǿ� �߰�
        GameBoardInfo[Npc_Y][Npc_X] = DropItem();
    }
    else {
        //ĭ ���� �����ǿ��� ����
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

void RangeNpcDie(int Npc_X, int Npc_Y) // ���� ����
{
    int ran = rand();
    if (Npc_X == 0 || Npc_X == GBOARD_WIDTH || Npc_Y == 0 || Npc_Y == GBOARD_HEIGHT) return;

    SetCurrentCursorPos(Npc_X * 2 + GBOARD_ORIGIN_X, Npc_Y + GBOARD_ORIGIN_Y);
    
    //������ ���
    if (ran % 100 < Diff.itemRate) {
        //������ �׸��� �����ǿ� �߰�
        GameBoardInfo[Npc_Y][Npc_X] = DropItem();
    }
    else {
        //ĭ ���� �����ǿ��� ����
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

void StopRangeNpcDie(int Npc_X, int Npc_Y) //���� ����
{
    int ran = rand();
    if (Npc_X == 0 || Npc_X == GBOARD_WIDTH || Npc_Y == 0 || Npc_Y == GBOARD_HEIGHT) return;

    SetCurrentCursorPos(Npc_X * 2 + GBOARD_ORIGIN_X, Npc_Y + GBOARD_ORIGIN_Y);
    
    //������ ���
    if (ran % 100 < Diff.itemRate) {
        //������ �׸��� �����ǿ� �߰�
        GameBoardInfo[Npc_Y][Npc_X] = DropItem();
    }
    else {
        //ĭ ���� �����ǿ��� ����
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

int DistanceSquare(int x1, int y1, int x2, int y2)  //�Ÿ� ���
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

void RangeNpcAttack()  //���Ÿ� npc ����
{
    for (int i = 0; i < maxmob; i++)
    {
        if (StopRangeNpcInfo[i][0] == 0) continue;  //���� ���Ÿ� npc ������ continue
        
        for (int j = 0; j < maxProjectile; j++)  //����ü �迭�� ������ ���� ������
        {
            if (ProjectileInfo[j][0] != 0) continue; //���� ó��

            //���� ���� ������Ʈ
            RangeNpcAttackDirectionUpdate();

            //����ü �迭�� StopRangeNpc��ǥ ����
            ProjectileInfo[j][0] = StopRangeNpcInfo[i][0];
            ProjectileInfo[j][1] = StopRangeNpcInfo[i][1];

            //����ü ���� �����ϰ� ��ǥ ������Ʈ
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

            //����ü �浹 �˻� �� �߰� �õ�
            if (GameBoardInfo[ProjectileInfo[j][1]][ProjectileInfo[j][0]] == 0 || GameBoardInfo[ProjectileInfo[j][1]][ProjectileInfo[j][0]] > 100) {
                //��������� npcbullet �����ϰ� �׸���
                SetCurrentCursorPos(ProjectileInfo[j][0] * 2 + GBOARD_ORIGIN_X, ProjectileInfo[j][1] + GBOARD_ORIGIN_Y);
                textcolor(darkRED);
                switch (ProjectileInfo[j][2])
                {
                default:
                    break;
                case UP:
                    printf("��");
                    break;
                case DOWN:
                    printf("��");
                    break;
                case LEFT:
                    printf("��");
                    break;
                case RIGHT:
                    printf("��");
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
                //�������� ��� npcbullet �� ������ ����
                ProjectileExtinction(ProjectileInfo[j][0], ProjectileInfo[j][1]);
            }
            else if (GameBoardInfo[ProjectileInfo[j][1]][ProjectileInfo[j][0]] == PC) {
                //pc�� ��� ����ü �����ϰ� hp ����
                ProjectileExtinction(ProjectileInfo[j][0], ProjectileInfo[j][1]);
                DecreaseHP();
            }
            else if (GameBoardInfo[ProjectileInfo[j][1]][ProjectileInfo[j][0]] == WALL || GameBoardInfo[ProjectileInfo[j][1]][ProjectileInfo[j][0]] == CHAINSAW) {
                //���� ��� ����ü ����
                ProjectileExtinction(ProjectileInfo[j][0], ProjectileInfo[j][1]);
            }
            else {
                //�������� ��� (����ü ���常 �ϰ� �׸��� �ʱ�) = (�ѱ��)
            }

            break;
        }
    }
}

void ProjectileMove()   //���� ����ġ�� ����� projectile�� ���׷��̵�, ���� ������ gameboardinfo �ֽ�ȭ and �׸���, pc�� ���� ����ġ�� projectile �Ҹ�
{
    for (int i = 0; i < maxProjectile; i++) {
        if (ProjectileInfo[i][0] == 0) continue; //���� ó��

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
                //��ų ����Ʈ�� �浹
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
                //�����۰� �浹 �� �����۰� ����ü ��� ���� -> ProjectileExtinction �Լ��� ���� ������
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
            printf("��");
            break;
        case DOWN:
            printf("��");
            break;
        case LEFT:
            printf("��");
            break;
        case RIGHT:
            printf("��");
            break;
        }
    }
}

void ProjectileExtinction(int x, int y) // ����ü �Ҹ�
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
    //���� ó��
    if (x < 0 || x > GBOARD_WIDTH || y < 0 || y > GBOARD_HEIGHT) return; //������ ���θ� ����� �̵����� �ʱ�
    
    //�浹 ó��
    for (int ny = y; ny < y + 2; ny++) {
        for (int nx = x; nx < x + 2; nx++) {
            switch (GameBoardInfo[ny][nx])
            {
            default: //��Ÿ; PC, ����Ʈ ��
                //������ ���
                if (GameBoardInfo[ny][nx] == LIFE || GameBoardInfo[ny][nx] == BOMB) UseItem(GameBoardInfo[ny][nx]);
                //��ų �״�� �ΰ� ������ �̵�
                break;
            case 0: //�ƹ��͵� ���� ��� ������ �̵�
                break;
            case WALL: //���� ��� �Լ� ����
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
            case NPC: //npc�� ��� NpcDie �����ϰ� hp �����ϰ� ������ �̵�
                NpcDie(nx, ny);
                DecreaseHP();
                //������ ���
                if (GameBoardInfo[ny][nx] == LIFE || GameBoardInfo[ny][nx] == BOMB) UseItem(GameBoardInfo[ny][nx]);
                break;
            case MOVINGRANGENPC: //npc�� ����
                RangeNpcDie(nx, ny);
                DecreaseHP();
                //������ ���
                if (GameBoardInfo[ny][nx] == LIFE || GameBoardInfo[ny][nx] == BOMB) UseItem(GameBoardInfo[ny][nx]);
                break;
            case STOPPEDRANGENPC: //npc�� ����
                StopRangeNpcDie(nx, ny);
                DecreaseHP();
                //������ ���
                if (GameBoardInfo[ny][nx] == LIFE || GameBoardInfo[ny][nx] == BOMB) UseItem(GameBoardInfo[ny][nx]);
                break;
            case NPCBULLET: //�� ����ü�� ��� ����ü ���ְ� hp �����ϰ� ������ �̵�
                ProjectileExtinction(nx, ny);
                DecreaseHP();
                break;
            case BossNum: //������ ��� pc hp �����ϰ� ����
                DecreaseHP();
                return;
                break;
            case BossSkillEffect: //���� ��ų�� ��� ��ų �������� pc hp �����ϰ� ������ �̵�
                DecreaseHP();
                break;
            }
        }
    }

    //pc �̵� ����
    //���� pc �����
    erasePC();

    //���� pc �׸���
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

//���Ÿ� npc ���ݹ��� �ֽ�ȭ
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
