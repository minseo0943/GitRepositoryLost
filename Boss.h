#pragma once
#define BossNum 7
#define BossSkillEffect 6

extern int bossFlag; //보스 정지 나타냄
extern int random;
extern int XX1, YY1, XX2, YY2; //보스모델의 왼쪽위 ,오른쪽아래 좌표
extern int x3; //와플스킬출력할때사용
extern int y3; //와플스킬출력할때사용

//보스 좌표
extern int BossPoint[2];

//보스 모델 7
extern int BossModel[7][7];

//보스 스킬 6
extern int BossSkillModel[3][3];

//보스 HP
extern int BossHP;
extern int BossMaxHP;

void ShowBoss();
void DeleteBoss();
void ShowBossSkill_Rect(int X1, int Y1, int X2, int Y2); //광역스킬1-네모빔
void ShowBossSkill_RectOrbit(int X1, int Y1, int X2, int Y2); //광역스킬1-네모빔궤적
void ShowBossSkill_Waffle_Left(int x1); //광역스킬2-왼쪽와플
void ShowBossSkill_Waffle_Up(int y1); //광역스킬2-위쪽와플
void ShowBossSkill_Waffle_Right(int x1); //광역스킬2-오른쪽와플
void ShowBossSkill_Waffle_Down(int y1); //광역스킬2-아래쪽와플
void ShowBossSkill_WaffleOrbit();//광역스킬2-와플궤적
void BossMove();
void ShowBossSkill_Circle(); //기본스킬-강강술래(보스중에 빙빙 도는거)
void DeleteBossSkill();
void DeleteBossSkill2();