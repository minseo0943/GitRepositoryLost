#pragma once
#define maxmob 15
#define maxProjectile 90

#define WALL 1
#define PC 2
#define NPC 3
#define MOVINGRANGENPC 4
#define STOPPEDRANGENPC 5
#define NPCBULLET 9
#define CHAINSAW 8

#define RIGHT 100
#define LEFT 200
#define UP 300
#define DOWN 400

extern int NpcInfo[maxmob][2];
extern int RangeNpcInfo[maxmob][2];
extern int StopRangeNpcInfo[maxmob][3];
extern int PCPoint[2];
extern int ProjectileInfo[maxProjectile][3];


extern int TimeCount;
extern int TimeCount2;
extern int AttackCount;
extern int killCount;
extern int item;
extern int ChainsawTouched[6];
extern COORD pc;

COORD RandomSideXY();
void NpcSpawn();
void NpcMove();
void RangeNpcSpawn();
void RangeNpcMove();
int DistanceSquare(int x1, int y1, int x2, int y2);
void RangeNpcAttack();
void ProjectileMove();
void NpcDie(int Npc_X, int Npc_Y);
void RangeNpcDie(int Npc_X, int Npc_Y);
void StopRangeNpcDie(int Npc_X, int Npc_Y);
void ProjectileExtinction(int x, int y);

void PcShiftNext(int x, int y);
void PcShiftLeft();
void PcShiftDown();
void PcShiftUp();
void PcShiftRight();
void RangeNpcAttackDirectionUpdate();
void AllDie();