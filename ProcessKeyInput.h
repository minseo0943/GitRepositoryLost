#pragma once
#define KEY_UP 72
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_DOWN 80
#define A 97
#define W 119

#define ONE 49
#define TWO 50
#define THREE 51
#define FOUR 52
#define FIVE 53

#define ESC 27

//난이도 구조체
typedef struct Difficulty {
	//속도; 낮을수록 빠름
	int speed; //게임 속도
	int spawnSpeed; //적 스폰 속도
	int projectileSpawnSpeed; //적 원거리 공격 스폰 속도
	int attackSpeed; //pc 스킬 발동 속도
	int itemRate; //아이템 드랍 확률; 높을수록 좋음; 0~100
	int npcSpeed; //npc 속도; 높을수록 느림
} Difficulty;

extern int key; //키보드 키값의 아스키코드
extern Difficulty Diff; //난이도 저장 구조체 변수
extern int lastDir; //최근 dir

void ProcessKeyInput();
