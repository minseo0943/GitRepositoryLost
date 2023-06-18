#pragma once
#define GBOARD_WIDTH 60 //게임판 가로 크기
#define GBOARD_HEIGHT 40 //게임판 세로 크기
#define GBOARD_ORIGIN_X 4 //기본 좌측 상단 시작 X좌표
#define GBOARD_ORIGIN_Y 2 //기본 좌측 상단 시작 Y좌표

//[0 ~ GBOARD_HEIGHT][0 ~ GBOARD_WIDTH]
//테두리: 0, GBOARD_HEIGHT, 0, GBOARD_WIDTH
extern int GameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 1];
extern int score; //점수
extern int finalScore; //최종 점수
extern int HP; //체력
extern int maxHP; //최대 체력
extern int stage; //스테이지 번호
extern COORD pos_time; //시간 위치
extern COORD pos_stage; //스테이지 번호 위치
extern COORD pos_score; //스코어 위치
extern COORD pos_hp; //체력 위치
extern COORD pos_skillList; //얻은 스킬 위치
extern COORD pos_skillSelect; //스킬 선택창 위치
extern COORD cursor; //커서
extern int maxExpScore; //레벨업에 필요한 경험치 스코어

void ClearBoard();
void DrawBoard();
void displayNotice(char* str);
void eraseNotice();
void LevelUp();
void IncreaseScore();
void Debug_GameBoardInfo();
void initPosUI();
void DrawIngameUI();
void drawStage();
void drawTime();
void drawScore();
void drawHP();
void drawSelectedSkillList();
void ClearGameBoard();

void drawSkillShape(int type, int level);
void drawPC();
void erasePC();
void drawNpcShape();
void drawItemShape(int num);
void drawRangeNpcShape();
void drawStopRangeNpcShape();

int GameOver();
void GameClear();
void EraseGameBoard();
void DecreaseHP();
void DecreaseBossHP();
void UpdateScoreboard();
void UpdateMemoryList();