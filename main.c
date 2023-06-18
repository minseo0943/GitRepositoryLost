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

//함수
//void FindKeyboardValue(); //키보드 입력값 찾기

//메인 함수
void main() {
	system("mode con cols=180 lines=48"); //콘솔창 크기 지정, 200, 48
	//system("mode con cols=280 lines=47"); //디버그용 크기
	RemoveCursor(); //커서 지우기
	srand((int)time(NULL)); //랜덤 변수 시작

	//키보드 코드 확인
	//FindKeyboardValue();

	//데이터 초기화
	initMemory();
	initSkill();
	initPosUI();
	defineItem();

	//게임판 중앙 위치
	center.X = GBOARD_ORIGIN_X + GBOARD_WIDTH;
	center.Y = GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 - 1;

	//메인 메뉴 호출
	ShowMainMenu();
}

//기타 함수
void FindKeyboardValue() {
	while (1) {
		GetKeyboardInput();
		printf("%d", key);
	}
}

