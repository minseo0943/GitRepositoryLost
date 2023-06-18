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


void initMemory() {
	memory* p;

	for (int i = 0; i < MEMORY_SIZE; i++) {
		p = memoryList + i;
		p->check = FALSE;

		switch (i)
		{
		default:
			break;
		case 0:
			strcpy(p->locked, "스테이지1 입장 시 해금");
			strcpy(p->unlocked, "던전을 탐색하던 도중 길을 잃었다. 던전 출입구를 찾아 탈출해야 한다.");
			break;
		case 1:
			strcpy(p->locked, "스테이지2 입장 시 해금");
			strcpy(p->unlocked, "몰려오는 몬스터를 피하다가 던전 깊은 곳으로 들어왔다.");
			break;
		case 2:
			strcpy(p->locked, "보스 스테이지 입장 시 해금");
			strcpy(p->unlocked, "던전 출입구를 보스가 지키고 있다. 보스를 처치해야 탈출할 수 있다.");
			break;
		case 3:
			strcpy(p->locked, "원거리 무기 첫 강화 시 해금");
			strcpy(p->unlocked, "생존을 위해 지니고 있던 원거리 무기를 강화했다.");
			break;
		case 4:
			strcpy(p->locked, "권총 최대 강화 시 해금");
			strcpy(p->unlocked, "권총을 기관총으로 강화했다.");
			break;
		case 5:
			strcpy(p->locked, "레버액션 산탄총 최대 강화 시 해금");
			strcpy(p->unlocked, "레버액션 산탄총을 펌프액션 산탄총으로 강화했다.");
			break;
		case 6:
			strcpy(p->locked, "단궁 최대 강화 시 해금");
			strcpy(p->unlocked, "단궁을 대궁으로 강화했다.");
			break;
		case 7:
			strcpy(p->locked, "단검 획득 시 해금");
			strcpy(p->unlocked, "가까운 몬스터를 처치할 단검을 제작했다.");
			break;
		case 8:
			strcpy(p->locked, "소형 방벽 획득 시 해금");
			strcpy(p->unlocked, "몬스터의 공격을 막아줄 소형 방벽을 제작했다.");
			break;
		case 9:
			strcpy(p->locked, "한손 전투망치 획득 시 해금");
			strcpy(p->unlocked, "가까운 몬스터를 처치할 한손 전투망치를 제작했다.");
			break;
		case 10:
			strcpy(p->locked, "단검 최대 강화 시 해금");
			strcpy(p->unlocked, "단검을 대검으로 강화했다.");
			break;
		case 11:
			strcpy(p->locked, "소형 방벽 최대 강화 시 해금");
			strcpy(p->unlocked, "소형 방벽을 대형 방벽으로 강화했다.");
			break;
		case 12:
			strcpy(p->locked, "한손 전투망치 최대 강화 시 해금");
			strcpy(p->unlocked, "한손 전투망치를 대형 전투망치로 강화했다.");
			break;
		case 13:
			strcpy(p->locked, "최대 체력 증가 획득 시 해금");
			strcpy(p->unlocked, "체력을 강화하여 몬스터의 공격을 더 많이 버틸 수 있게 되었다.");
			break;
		case 14:
			strcpy(p->locked, "공격 속도 증가 획득 시 해금");
			strcpy(p->unlocked, "무기로 더 빠르게 공격할 수 있게 되었다.");
			break;
		case 15:
			strcpy(p->locked, "아이템 획득 확률 증가 획득 시 해금");
			strcpy(p->unlocked, "처치한 몬스터로부터 유용한 아이템을 더 많이 찾을 수 있게 되었다.");
			break;
		case 16:
			strcpy(p->locked, "스코어 1000 이상 달성 시 해금");
			strcpy(p->unlocked, "던전에서 살아남는 법을 터득했다.");
			break;
		case 17:
			strcpy(p->locked, "스코어 4000 이상 달성 시 해금");
			strcpy(p->unlocked, "던전에서 살아남기 위한 노하우가 생겼다.");
			break;
		case 18:
			strcpy(p->locked, "스코어 10000 이상 달성 시 해금");
			strcpy(p->unlocked, "던전 생존 전문가가 되었다.");
			break;
		case 19:
			strcpy(p->locked, "보스 처치 시 해금");
			strcpy(p->unlocked, "보스를 처치하고 던전에서 탈출했다!");
			break;
		}
	}
}

void initSkill() {
	initData_SkillEffectInfo();
	initData_SelectedSkill();

	//게임판 배열에 저장할 스킬 종류별 이펙트 번호 = SKILL_TYPE*100 + SKILL_LEVEL*10 = 110,120,130,210,220,230,...
	//방향을 자리수로 표시: 스킬 110 >>>  왼쪽)111  위쪽)112  오른쪽)113  아래쪽)114
	for (int i = 1; i <= SKILL_TYPE; i++) {
		defineSkill(i);
	}
}

void initData_Variable() {
	//score 초기화
	score = 0;
	finalScore = 0;
	maxExpScore = 500;
	//lastDir 초기화
	lastDir = NORTH;
}

void initDifficulty(int speed, int spawnSpeed, int projectileSpawnSpeed, int attackSpeed, int itemRate) {
	//난이도 설정
	Diff.speed = speed;
	Diff.spawnSpeed = spawnSpeed;
	Diff.projectileSpawnSpeed = projectileSpawnSpeed;
	Diff.attackSpeed = attackSpeed;
	Diff.itemRate = itemRate;
}

void initHP(int hp) {
	//HP 설정
	HP = hp;
	maxHP = HP;
}

void initData_SkillEffectInfo() {
	//skillEffectInfo 초기화
	for (int i = 0; i <= skillTop; i++) {
		skillEffectInfo[i].X = 0, skillEffectInfo[i].Y = 0;
		skillEffectInfo[i].dir = 0;
		skillEffectInfo[i].ef = 0;
		skillEffectInfo[i].level = 0;
		skillEffectInfo[i].type = 0;
	} 
	skillTop = -1;
}

void initData_SelectedSkill() {
	//SselectedSkill 초기화
	for (int i = 0; i < 3; i++) selectedSkill[i] = 0;
}

void initData_Time() {
	//TimeCount 초기화
	TimeCount = 0;
}

void init_PCPoint() {
	//PC 위치 중앙으로 초기화
	pc = ConvertCursorIntoArrCur(center);

	if (stage == 3) {
		//보스전 위치
		pc.Y += 8;
	}

	PCPoint[0] = pc.X;
	PCPoint[1] = pc.Y;
	
	GameBoardInfo[pc.Y][pc.X] = PC;
}

void initData_GameBoardInfo() {
	//GameBoardInfo 초기화
	for (int y = 1; y < GBOARD_HEIGHT; y++) {
		for (int x = 1; x < GBOARD_WIDTH; x++) {
			GameBoardInfo[y][x] = 0;
		}
	}
}

void initBossHP(int hp) {
	//HP 설정
	BossHP = hp;
	BossMaxHP = BossHP;
}

void initBossPoint() {
	BossPoint[0] = 28;
	BossPoint[1] = 10;
}

//NpcInfo, RangeNpcInfo, StopRangeNpcInfo, ProjectileInfo 초기화 함수 작성하기
