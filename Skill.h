#pragma once
#define SKILL_LEVEL 3 //스킬 레벨 크기
#define SKILL_TYPE 9 //스킬 종류 개수
#define EFFECT_SIZE 4 //이펙트 배열 크기
#define SKILL_LEN 16 //스킬 문자열 길이
#define SELECT_SIZE 3 //스킬 선택 개수

#define EAST 1 //방향: 오른쪽
#define NORTH 2 //방향: 위쪽
#define WEST 3 //방향: 왼쪽
#define SOUTH 4 //방향: 아래쪽

typedef struct skill {
	char name[SKILL_LEN]; //스킬 이름
	char e[3]; //스킬 이펙트 = 사용할 특수문자
	//int effect[EFFECT_SIZE][EFFECT_SIZE]; //스킬 이펙트 배열
	int effect[EFFECT_SIZE][EFFECT_SIZE];
	int type;
	int level;
} skill;

typedef struct skillEffect {
	int X, Y;
	int type;
	int level;
	int dir;
	int ef;
} skillEffect;

extern skill skillList[SKILL_TYPE][SKILL_LEVEL];
extern int selectedSkill[SELECT_SIZE];
extern skillEffect skillEffectInfo[GBOARD_HEIGHT * GBOARD_WIDTH];
extern int skillTop;
extern int flag;
extern COORD old_cursor;
extern int old_dir;

void sub_AddEffectByDirection(COORD cursor, int type, int level, int x, int y, int nx, int ny, int dir);
void erase_AddEffectByDirection(COORD cursor, int type, int level, int x, int y, int nx, int ny, int dir);
void AddEffectByDirection(int skillNum, int dir); //skillList[type-1][level-1]을 PC 방향 dir(EAST, NORTH, WEST, SOUTH)에 따라 추가
void RemoveSkillEffect(int idx);
void SkillEffectMove();
void UpdateSkillEffect(int idx);
void SelectNewSkill();
void defineSkill(int type); //스킬1 정의; 기본 방향 = 왼쪽으로 발사 기준; 미완성