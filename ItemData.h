#pragma once
#define ITEMNUM 2
#define LIFE 21
#define BOMB 22

//아이템 구조체
typedef struct Item {
	char shape[3]; //아이템 출력 모양
	int num; //아이템 저장 번호
	int color; //아이템 색상
} Item;

extern Item itemList[ITEMNUM];

void defineItem();
void UseItem(int num);
int DropItem();
void Bomb(int x, int y);