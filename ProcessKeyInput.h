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

//���̵� ����ü
typedef struct Difficulty {
	//�ӵ�; �������� ����
	int speed; //���� �ӵ�
	int spawnSpeed; //�� ���� �ӵ�
	int projectileSpawnSpeed; //�� ���Ÿ� ���� ���� �ӵ�
	int attackSpeed; //pc ��ų �ߵ� �ӵ�
	int itemRate; //������ ��� Ȯ��; �������� ����; 0~100
	int npcSpeed; //npc �ӵ�; �������� ����
} Difficulty;

extern int key; //Ű���� Ű���� �ƽ�Ű�ڵ�
extern Difficulty Diff; //���̵� ���� ����ü ����
extern int lastDir; //�ֱ� dir

void ProcessKeyInput();
