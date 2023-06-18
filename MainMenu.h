#pragma once
#include <stdbool.h>
#define SCOREBOARD__SIZE 10 //스코어보드 크기
#define MEMORY_SIZE 20 //기억 목록 크기
#define MEMORY_LEN 100 //기억 문자열 길이

typedef struct memory {
	char locked[MEMORY_LEN]; //잠긴 기억: 해금 조건
	char unlocked[MEMORY_LEN]; //해금된 기억
	bool check; //기억이 해금되었는지 체크: TRUE=해금, FALSE=미해금
} memory;

extern int scoreboard[SCOREBOARD__SIZE]; //스코어보드
extern memory memoryList[MEMORY_SIZE]; //기억 목록
extern COORD center; //게임판 중앙 좌표

void GetKeyboardInput(); //키보드 입력받는 함수
void ShowMainMenu(); //메인 메뉴 출력
void StartGame(); //1. 게임 시작; 컨셉과 프레임만 구현
void DisplayScoreBoard(); //2. 스코어보드
void DisplayMemory(); //3. 기억 목록; 프레임만 구현
void DisplayGameRule(); //4. 게임 방법; 프레임만 구현
void ExitGame(); //5. 게임 종료