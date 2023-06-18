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
#include "PCNPC.h"
#include "Skill.h"

skill skillList[SKILL_TYPE][SKILL_LEVEL]; //스킬 목록
int selectedSkill[SELECT_SIZE]; //보유한 스킬 번호(110, 120, 130, 210, ...)
skillEffect skillEffectInfo[GBOARD_HEIGHT * GBOARD_WIDTH]; //스킬 이펙트 좌표 저장 배열
int skillTop = -1; //skillEffectInfo 최대 인덱스
int flag;
COORD old_cursor; //근접공격 이전 pc 위치
int old_dir; //근접공격 이전 pc 방향

void sub_AddEffectByDirection(COORD cursor, int type, int level, int x, int y, int nx, int ny, int dir) {
	COORD arrCur;
	//예외 처리 
	if (skillList[type - 1][level - 1].effect[nx][ny] == 0) return; //스킬이펙트가 없을 경우 넘기기
	if (cursor.X + x < GBOARD_ORIGIN_X - 1 || cursor.X + x > GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 || cursor.Y + y < GBOARD_ORIGIN_Y || cursor.Y + y > GBOARD_ORIGIN_Y + GBOARD_HEIGHT) return; //커서가 게임판 내부를 벗어나면 넘기기

	arrCur = ConvertCursorIntoArrCur(cursor);

	//충돌 검사 및 이펙트 번호를 게임판 배열에 저장
	if (type == 1 || type == 2 || type == 3) {
		switch (GameBoardInfo[arrCur.Y + y][arrCur.X + x])
		{
		default:
			//기타
			//게임판 스킬 이펙트 좌표를 배열에 저장
			skillTop++;
			skillEffectInfo[skillTop].X = arrCur.X + x;
			skillEffectInfo[skillTop].Y = arrCur.Y + y;
			skillEffectInfo[skillTop].dir = dir;
			skillEffectInfo[skillTop].level = level;
			skillEffectInfo[skillTop].type = type;
			skillEffectInfo[skillTop].ef = skillList[type - 1][level - 1].effect[nx][ny];
			break;
		case 0:
			//비어있을 경우 이펙트 출력하고 게임판에 추가하고 스킬이펙트배열에 좌표 저장
			//이펙트 출력
			SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
			drawSkillShape(type, level);
			//게임판에 추가
			GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
			//게임판 스킬 이펙트 좌표를 배열에 저장
			skillTop++;
			skillEffectInfo[skillTop].X = arrCur.X + x;
			skillEffectInfo[skillTop].Y = arrCur.Y + y;
			skillEffectInfo[skillTop].dir = dir;
			skillEffectInfo[skillTop].level = skillList[type - 1][level - 1].level;
			skillEffectInfo[skillTop].type = skillList[type - 1][level - 1].type;
			skillEffectInfo[skillTop].ef = skillList[type - 1][level - 1].effect[nx][ny];
			break;
		case WALL:
			//벽이면 생성하지 않고 넘기기
			break;
		case CHAINSAW:
			break;
		case NPC:
			//npc면 npcdie하고 넘기기
			NpcDie(arrCur.X + x, arrCur.Y + y);
			IncreaseScore();
			if (type == 3) {
				//이펙트 출력
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//게임판에 추가
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				//게임판 스킬 이펙트 좌표를 배열에 저장
				skillTop++;
				skillEffectInfo[skillTop].X = arrCur.X + x;
				skillEffectInfo[skillTop].Y = arrCur.Y + y;
				skillEffectInfo[skillTop].dir = dir;
				skillEffectInfo[skillTop].level = skillList[type - 1][level - 1].level;
				skillEffectInfo[skillTop].type = skillList[type - 1][level - 1].type;
				skillEffectInfo[skillTop].ef = skillList[type - 1][level - 1].effect[nx][ny];
			}
			break;
		case MOVINGRANGENPC:
			//npc와 동일
			RangeNpcDie(arrCur.X + x, arrCur.Y + y);
			IncreaseScore();
			if (type == 3) {
				//이펙트 출력
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//게임판에 추가
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				//게임판 스킬 이펙트 좌표를 배열에 저장
				skillTop++;
				skillEffectInfo[skillTop].X = arrCur.X + x;
				skillEffectInfo[skillTop].Y = arrCur.Y + y;
				skillEffectInfo[skillTop].dir = dir;
				skillEffectInfo[skillTop].level = skillList[type - 1][level - 1].level;
				skillEffectInfo[skillTop].type = skillList[type - 1][level - 1].type;
				skillEffectInfo[skillTop].ef = skillList[type - 1][level - 1].effect[nx][ny];
			}
			break;
		case STOPPEDRANGENPC:
			//npc와 동일
			StopRangeNpcDie(arrCur.X + x, arrCur.Y + y);
			IncreaseScore();
			if (type == 3) {
				//이펙트 출력
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//게임판에 추가
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				//게임판 스킬 이펙트 좌표를 배열에 저장
				skillTop++;
				skillEffectInfo[skillTop].X = arrCur.X + x;
				skillEffectInfo[skillTop].Y = arrCur.Y + y;
				skillEffectInfo[skillTop].dir = dir;
				skillEffectInfo[skillTop].level = skillList[type - 1][level - 1].level;
				skillEffectInfo[skillTop].type = skillList[type - 1][level - 1].type;
				skillEffectInfo[skillTop].ef = skillList[type - 1][level - 1].effect[nx][ny];
			}
			break;
		case NPCBULLET:
			//npc와 동일
			ProjectileExtinction(arrCur.X + x, arrCur.Y + y);
			if (type == 3) {
				//이펙트 출력
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//게임판에 추가
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				//게임판 스킬 이펙트 좌표를 배열에 저장
				skillTop++;
				skillEffectInfo[skillTop].X = arrCur.X + x;
				skillEffectInfo[skillTop].Y = arrCur.Y + y;
				skillEffectInfo[skillTop].dir = dir;
				skillEffectInfo[skillTop].level = skillList[type - 1][level - 1].level;
				skillEffectInfo[skillTop].type = skillList[type - 1][level - 1].type;
				skillEffectInfo[skillTop].ef = skillList[type - 1][level - 1].effect[nx][ny];
			}
			break;
		case BossNum:
			//생성하지 않고 보스 hp 감소
			DecreaseBossHP();
			break;
		}
	}
	//검
	else if (type == 4) {
		if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2) {
			//출력
			switch (GameBoardInfo[arrCur.Y + y][arrCur.X + x])
			{
			default:
				//기타
				break;
			case 0:
				//비어있을 경우 이펙트 출력하고 게임판에 추가하고 스킬이펙트배열에 좌표 저장
				//이펙트 출력
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//게임판에 추가
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				break;
			case WALL:
				//벽이면 생성하지 않고 넘기기
				break;
			case CHAINSAW:
				break;
			case NPC:
				//npc면 npcdie하고 넘기기
				NpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case MOVINGRANGENPC:
				//npc와 동일
				RangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case STOPPEDRANGENPC:
				//npc와 동일
				StopRangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case NPCBULLET:
				//npc와 동일
				ProjectileExtinction(arrCur.X + x, arrCur.Y + y);
				break;
			case BossNum:
				//이펙트 출력하지 않고 보스 hp 감소
				DecreaseBossHP();
				break;
			case BossSkillEffect:
				//이펙트 출력하지 않고 보스이펙트 제거
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = 0;
				break;
			}

			//이번 pc 좌표 및 방향 백업
			old_cursor = cursor;
			old_dir = dir;
		}
		else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 1) {
			//이전 이펙트 지우기
			if (flag == 0) {
				switch (old_dir)
				{
				case EAST:
					for (int y = EFFECT_SIZE - 1; y >= 0; y--) {
						for (int x = EFFECT_SIZE - 1; x >= 0; x--) {
							erase_AddEffectByDirection(old_cursor, type, level, x, y, EFFECT_SIZE - 1 - y, EFFECT_SIZE - 1 - x, old_dir);
						}
					}
					break;
				case WEST:
					for (int y = 0; y < EFFECT_SIZE; y++) {
						for (int x = 0; x < EFFECT_SIZE; x++) {
							erase_AddEffectByDirection(old_cursor, type, level, x, y, y, x, old_dir);
						}
					}
					break;
				case NORTH:
					for (int y = 0; y < EFFECT_SIZE; y++) {
						for (int x = EFFECT_SIZE - 1; x >= 0; x--) {
							erase_AddEffectByDirection(old_cursor, type, level, x, y, EFFECT_SIZE - 1 - x, y, old_dir);
						}
					}
					break;
				case SOUTH:
					for (int y = EFFECT_SIZE - 1; y >= 0; y--) {
						for (int x = 0; x < EFFECT_SIZE; x++) {
							erase_AddEffectByDirection(old_cursor, type, level, x, y, x, EFFECT_SIZE - 1 - y, old_dir);
						}
					}
					break;
				}
				flag = 1;
			}

			//출력
			switch (GameBoardInfo[arrCur.Y + y][arrCur.X + x])
			{
			default:
				//기타
				break;
			case 0:
				//비어있을 경우 이펙트 출력하고 게임판에 추가하고 스킬이펙트배열에 좌표 저장
				//이펙트 출력
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//게임판에 추가
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				break;
			case WALL:
				//벽이면 생성하지 않고 넘기기
				break;
			case CHAINSAW:
				break;
			case NPC:
				//npc면 npcdie하고 넘기기
				NpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case MOVINGRANGENPC:
				//npc와 동일
				RangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case STOPPEDRANGENPC:
				//npc와 동일
				StopRangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case NPCBULLET:
				//npc와 동일
				ProjectileExtinction(arrCur.X + x, arrCur.Y + y);
				break;
			case BossNum:
				//이펙트 출력하지 않고 보스 hp 감소
				DecreaseBossHP();
				break;
			case BossSkillEffect:
				//이펙트 출력하지 않고 보스이펙트 제거
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = 0;
				break;
			}

			//이번 pc 좌표 및 방향 백업
			old_cursor = cursor;
			old_dir = dir;
		}
		else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 2) {
			//이전 이펙트 지우기
			if (flag == 0) {
				switch (old_dir)
				{
				case EAST:
					for (int y = EFFECT_SIZE - 1; y >= 0; y--) {
						for (int x = EFFECT_SIZE - 1; x >= 0; x--) {
							erase_AddEffectByDirection(old_cursor, type, level, x, y, EFFECT_SIZE - 1 - y, EFFECT_SIZE - 1 - x, old_dir);
						}
					}
					break;
				case WEST:
					for (int y = 0; y < EFFECT_SIZE; y++) {
						for (int x = 0; x < EFFECT_SIZE; x++) {
							erase_AddEffectByDirection(old_cursor, type, level, x, y, y, x, old_dir);
						}
					}
					break;
				case NORTH:
					for (int y = 0; y < EFFECT_SIZE; y++) {
						for (int x = EFFECT_SIZE - 1; x >= 0; x--) {
							erase_AddEffectByDirection(old_cursor, type, level, x, y, EFFECT_SIZE - 1 - x, y, old_dir);
						}
					}
					break;
				case SOUTH:
					for (int y = EFFECT_SIZE - 1; y >= 0; y--) {
						for (int x = 0; x < EFFECT_SIZE; x++) {
							erase_AddEffectByDirection(old_cursor, type, level, x, y, x, EFFECT_SIZE - 1 - y, old_dir);
						}
					}
					break;
				}
				flag = 1;
			}

			//출력
			switch (GameBoardInfo[arrCur.Y + y][arrCur.X + x])
			{
			default:
				//기타
				break;
			case 0:
				//비어있을 경우 이펙트 출력하고 게임판에 추가하고 스킬이펙트배열에 좌표 저장
				//이펙트 출력
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//게임판에 추가
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				break;
			case WALL:
				//벽이면 생성하지 않고 넘기기
				break;
			case CHAINSAW:
				break;
			case NPC:
				//npc면 npcdie하고 넘기기
				NpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case MOVINGRANGENPC:
				//npc와 동일
				RangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case STOPPEDRANGENPC:
				//npc와 동일
				StopRangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case NPCBULLET:
				//npc와 동일
				ProjectileExtinction(arrCur.X + x, arrCur.Y + y);
				break;
			case BossNum:
				//이펙트 출력하지 않고 보스 hp 감소
				DecreaseBossHP();
				break;
			case BossSkillEffect:
				//이펙트 출력하지 않고 보스이펙트 제거
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = 0;
				break;
			}

			//이번 pc 좌표 및 방향 백업
			old_cursor = cursor;
			old_dir = dir;
		}
		else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 3) {
			//이전 이펙트 지우기
			if (flag == 0) {
				switch (old_dir)
				{
				case EAST:
					for (int y = EFFECT_SIZE - 1; y >= 0; y--) {
						for (int x = EFFECT_SIZE - 1; x >= 0; x--) {
							erase_AddEffectByDirection(old_cursor, type, level, x, y, EFFECT_SIZE - 1 - y, EFFECT_SIZE - 1 - x, old_dir);
						}
					}
					break;
				case WEST:
					for (int y = 0; y < EFFECT_SIZE; y++) {
						for (int x = 0; x < EFFECT_SIZE; x++) {
							erase_AddEffectByDirection(old_cursor, type, level, x, y, y, x, old_dir);
						}
					}
					break;
				case NORTH:
					for (int y = 0; y < EFFECT_SIZE; y++) {
						for (int x = EFFECT_SIZE - 1; x >= 0; x--) {
							erase_AddEffectByDirection(old_cursor, type, level, x, y, EFFECT_SIZE - 1 - x, y, old_dir);
						}
					}
					break;
				case SOUTH:
					for (int y = EFFECT_SIZE - 1; y >= 0; y--) {
						for (int x = 0; x < EFFECT_SIZE; x++) {
							erase_AddEffectByDirection(old_cursor, type, level, x, y, x, EFFECT_SIZE - 1 - y, old_dir);
						}
					}
					break;
				}
				flag = 1;
			}
		}
	}
	//방벽
	else if (type == 5) {
		if (TimeCount % (Diff.attackSpeed * 2) == Diff.attackSpeed / 2) {
			//출력
			switch (GameBoardInfo[arrCur.Y + y][arrCur.X + x])
			{
			default:
				//기타
				break;
			case 0:
				//비어있을 경우 이펙트 출력하고 게임판에 추가하고 스킬이펙트배열에 좌표 저장
				//이펙트 출력
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//게임판에 추가
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				break;
			case WALL:
				//벽이면 생성하지 않고 넘기기
				break;
			case CHAINSAW:
				break;
			case NPC:
				//npc면 npcdie하고 넘기기
				NpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case MOVINGRANGENPC:
				//npc와 동일
				RangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case STOPPEDRANGENPC:
				//npc와 동일
				StopRangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case NPCBULLET:
				//npc와 동일
				ProjectileExtinction(arrCur.X + x, arrCur.Y + y);
				break;
			case BossNum:
				//이펙트 출력하지 않고 보스 hp 감소
				DecreaseBossHP();
				break;
			case BossSkillEffect:
				//이펙트 출력하지 않고 보스이펙트 제거
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = 0;
				break;
			}
		}
	}
	//망치
	else if (type == 6) {
		if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2) {
			//출력
			switch (GameBoardInfo[arrCur.Y + y][arrCur.X + x])
			{
			default:
				//기타
				break;
			case 0:
				//비어있을 경우 이펙트 출력하고 게임판에 추가하고 스킬이펙트배열에 좌표 저장
				//이펙트 출력
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//게임판에 추가
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				break;
			case WALL:
				//벽이면 생성하지 않고 넘기기
				break;
			case CHAINSAW:
				break;
			case NPC:
				//npc면 npcdie하고 넘기기
				NpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case MOVINGRANGENPC:
				//npc와 동일
				RangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case STOPPEDRANGENPC:
				//npc와 동일
				StopRangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case NPCBULLET:
				//npc와 동일
				ProjectileExtinction(arrCur.X + x, arrCur.Y + y);
				break;
			case BossNum:
				//이펙트 출력하지 않고 보스 hp 감소
				DecreaseBossHP();
				break;
			case BossSkillEffect:
				//이펙트 출력하지 않고 보스이펙트 제거
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = 0;
				break;
			}

			//이번 pc 좌표 및 방향 백업
			old_cursor = cursor;
			old_dir = dir;
		}
		else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 1) {
			//이전 이펙트 지우기
			if (flag == 0) {
				switch (old_dir)
				{
				case EAST:
					for (int y = EFFECT_SIZE - 1; y >= 0; y--) {
						for (int x = EFFECT_SIZE - 1; x >= 0; x--) {
							erase_AddEffectByDirection(old_cursor, type, level, x, y, EFFECT_SIZE - 1 - y, EFFECT_SIZE - 1 - x, old_dir);
						}
					}
					break;
				case WEST:
					for (int y = 0; y < EFFECT_SIZE; y++) {
						for (int x = 0; x < EFFECT_SIZE; x++) {
							erase_AddEffectByDirection(old_cursor, type, level, x, y, y, x, old_dir);
						}
					}
					break;
				case NORTH:
					for (int y = 0; y < EFFECT_SIZE; y++) {
						for (int x = EFFECT_SIZE - 1; x >= 0; x--) {
							erase_AddEffectByDirection(old_cursor, type, level, x, y, EFFECT_SIZE - 1 - x, y, old_dir);
						}
					}
					break;
				case SOUTH:
					for (int y = EFFECT_SIZE - 1; y >= 0; y--) {
						for (int x = 0; x < EFFECT_SIZE; x++) {
							erase_AddEffectByDirection(old_cursor, type, level, x, y, x, EFFECT_SIZE - 1 - y, old_dir);
						}
					}
					break;
				}
				flag = 1;
			}
		}
	}
}

void erase_AddEffectByDirection(COORD cursor, int type, int level, int x, int y, int nx, int ny, int dir) {
	//예외 처리
	if (skillList[type - 1][level - 1].effect[nx][ny] == 0) return; //스킬이펙트가 없을 경우 넘기기
	if (cursor.X + x < GBOARD_ORIGIN_X - 1 || cursor.X + x > GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 || cursor.Y + y < GBOARD_ORIGIN_Y || cursor.Y + y > GBOARD_ORIGIN_Y + GBOARD_HEIGHT) return; //커서가 게임판 내부를 벗어나면 넘기기
	
	COORD arrCur = ConvertCursorIntoArrCur(cursor);

	//이전 근접 이펙트 지우기
	if (GameBoardInfo[arrCur.Y + y][arrCur.X + x] >= 400) {
		GameBoardInfo[arrCur.Y + y][arrCur.X + x] = 0;
		SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
		printf("  ");
	}
}

void AddEffectByDirection(int skillNum, int dir) {
	if (skillNum == 0) return; //스킬이 없으면 리턴

	COORD cursor;
	int type = (skillNum / 100) % 10;
	int level = (skillNum / 10) % 10;

	if (type < 1 || type > 9 || level < 1 || level > 3) return; //예외 처리

	pc.X = PCPoint[0];
	pc.Y = PCPoint[1];

	flag = 0;

	//현재 이펙트 그리기
	switch (dir)
	{
	default:
		break;
	case EAST:
		cursor.X = GBOARD_ORIGIN_X + (pc.X + EFFECT_SIZE - 2) * 2;
		cursor.Y = GBOARD_ORIGIN_Y + pc.Y - 1;
		SetCurrentCursorPos(cursor.X, cursor.Y);
		for (int y = EFFECT_SIZE - 1; y >= 0; y--) {
			for (int x = EFFECT_SIZE - 1; x >= 0; x--) {
				sub_AddEffectByDirection(cursor, type, level, x, y, EFFECT_SIZE - 1 - y, EFFECT_SIZE - 1 - x, dir);
			}
		}
		break;
	case WEST:
		cursor.X = GBOARD_ORIGIN_X + (pc.X - EFFECT_SIZE) * 2;
		cursor.Y = GBOARD_ORIGIN_Y + pc.Y - 1;
		SetCurrentCursorPos(cursor.X, cursor.Y);
		for (int y = 0; y < EFFECT_SIZE; y++) {
			for (int x = 0; x < EFFECT_SIZE; x++) {
				sub_AddEffectByDirection(cursor, type, level, x, y, y, x, dir);
			}
		}
		break;
	case NORTH:
		cursor.X = GBOARD_ORIGIN_X + (pc.X - 1) * 2;
		cursor.Y = GBOARD_ORIGIN_Y + pc.Y - 4;
		SetCurrentCursorPos(cursor.X, cursor.Y);
		for (int y = 0; y < EFFECT_SIZE; y++) {
			for (int x = EFFECT_SIZE - 1; x >= 0; x--) {
				sub_AddEffectByDirection(cursor, type, level, x, y, EFFECT_SIZE - 1 - x, y, dir);
			}
		}
		break;
	case SOUTH:
		cursor.X = GBOARD_ORIGIN_X + (pc.X - 1) * 2;
		cursor.Y = GBOARD_ORIGIN_Y + pc.Y + 2;
		SetCurrentCursorPos(cursor.X, cursor.Y);
		for (int y = EFFECT_SIZE - 1; y >= 0; y--) {
			for (int x = 0; x < EFFECT_SIZE; x++) {
				sub_AddEffectByDirection(cursor, type, level, x, y, x, EFFECT_SIZE - 1 - y, dir);
			}
		}
		break;
	}

	//디버그
	/*SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 4, 25);
	printf("출력%d", TimeCount);
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + (x + 2) * 4, 26 + y);
			printf("%03d,", skillList[type - 1][level - 1].effect[y][x]);
		}
	}*/
}

//skillEffectInfo[idx] 제거
void RemoveSkillEffect(int idx) {
	//예외 처리
	if (idx == 0 && skillEffectInfo[idx].X == 0 && skillEffectInfo[idx].Y == 0) return;

	if (GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] > 100) {
		//이펙트 지우기
		SetCurrentCursorPos(GBOARD_ORIGIN_X + skillEffectInfo[idx].X * 2, GBOARD_ORIGIN_Y + skillEffectInfo[idx].Y);
		printf("  ");
		//게임판에서 삭제
		GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] = 0;
	}

	//스킬이펙트배열에서 삭제
	skillEffectInfo[idx].X = 0, skillEffectInfo[idx].Y = 0;

	int cnt = 0;
	for (int i = 0; i <= skillTop; i++) {
		while (i + cnt <= skillTop) {
			if (skillEffectInfo[i + cnt].X == 0 && skillEffectInfo[i + cnt].Y == 0) {
				//0 제거
				cnt++;
				skillTop--;
			}
			else break;
		}
		skillEffectInfo[i] = skillEffectInfo[i + cnt];
	}

	//flag 반환
	if (cnt != 0) flag = 1;
}

//발사된 스킬 업데이트
void SkillEffectMove() {
	for (int i = 0; i <= skillTop; i++) {
		//스킬 좌표 업데이트
		flag = 0; //skillEffectInfo[i]를 삭제하면 flag=1
		UpdateSkillEffect(i);
		if (flag == 1) i--; //현재 이펙트를 삭제하고 배열을 한 칸 왼쪽으로 당겼으므로 현재 칸에 다음 이펙트가 있으므로 다음 이펙트에 접근하기 위해 i 감소

		//스킬 이펙트 전체 새로 그리기
		for (int i = 0; i <= skillTop; i++) {
			if (GameBoardInfo[skillEffectInfo[i].Y][skillEffectInfo[i].X] == PC || GameBoardInfo[skillEffectInfo[i].Y][skillEffectInfo[i].X] == LIFE || GameBoardInfo[skillEffectInfo[i].Y][skillEffectInfo[i].X] == BOMB || GameBoardInfo[skillEffectInfo[i].Y][skillEffectInfo[i].X] / 100 == 5) continue;
			
			GameBoardInfo[skillEffectInfo[i].Y][skillEffectInfo[i].X] = skillEffectInfo[i].ef;
			SetCurrentCursorPos(GBOARD_ORIGIN_X + skillEffectInfo[i].X * 2, GBOARD_ORIGIN_Y + skillEffectInfo[i].Y);
			drawSkillShape(skillEffectInfo[i].type, skillEffectInfo[i].level);
		}
	}
}

//다음 스킬 이펙트 그리기
void UpdateSkillEffect(int idx) {
	int ef = GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X]; //스킬 번호
	int type = skillEffectInfo[idx].type; //스킬 타입
	int level = skillEffectInfo[idx].level; //스킬 레벨
	int dir = skillEffectInfo[idx].dir; //스킬 방향
	COORD next;
	next.X = skillEffectInfo[idx].X, next.Y = skillEffectInfo[idx].Y;

	if (type == 1 || type == 2 || type == 3) {
		//스킬 배열 최신화
		switch (dir)
		{
		case EAST:
			next.X++;
			break;
		case WEST:
			next.X--;
			break;
		case NORTH:
			next.Y--;
			break;
		case SOUTH:
			next.Y++;
			break;
		}

		switch (GameBoardInfo[next.Y][next.X])
		{
		default: //기타; PC, 이펙트, 아이템 등
		   //이펙트 지우기
			if (GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] != PC && GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] != LIFE && GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] != BOMB && GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] / 100 != 5) {
				GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] = 0;
				SetCurrentCursorPos(GBOARD_ORIGIN_X + skillEffectInfo[idx].X * 2, GBOARD_ORIGIN_Y + skillEffectInfo[idx].Y);
				printf("  ");
			}
			//스킬 좌표만 이동 저장
			skillEffectInfo[idx].X = next.X, skillEffectInfo[idx].Y = next.Y;
			break;
		case 0:
			//이펙트 지우기
			if (GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] != PC && GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] != LIFE && GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] != BOMB && GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] / 100 != 5) {
				GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] = 0;
				SetCurrentCursorPos(GBOARD_ORIGIN_X + skillEffectInfo[idx].X * 2, GBOARD_ORIGIN_Y + skillEffectInfo[idx].Y);
				printf("  ");
			}
			//아무것도 없을 때 게임판 이동 위치에 스킬 이름 저장, 배열에 좌표 저장, 스킬 그리기
			skillEffectInfo[idx].X = next.X, skillEffectInfo[idx].Y = next.Y;
			break;
		case WALL: //벽이면 스킬 삭제
			RemoveSkillEffect(idx);
			break;
		case CHAINSAW:
			RemoveSkillEffect(idx);
			break;
		case NPC: //적이면 적 제거하고 스킬 삭제
			NpcDie(next.X, next.Y);
			if (type == 3) {
				GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] = 0;
				SetCurrentCursorPos(GBOARD_ORIGIN_X + skillEffectInfo[idx].X * 2, GBOARD_ORIGIN_Y + skillEffectInfo[idx].Y);
				printf("  ");
				skillEffectInfo[idx].X = next.X, skillEffectInfo[idx].Y = next.Y;
			}
			else RemoveSkillEffect(idx);
			IncreaseScore();
			break;
		case MOVINGRANGENPC: //적이면 적 제거하고 스킬 삭제
			RangeNpcDie(next.X, next.Y);
			if (type == 3) {
				GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] = 0;
				SetCurrentCursorPos(GBOARD_ORIGIN_X + skillEffectInfo[idx].X * 2, GBOARD_ORIGIN_Y + skillEffectInfo[idx].Y);
				printf("  ");
				skillEffectInfo[idx].X = next.X, skillEffectInfo[idx].Y = next.Y;
			}
			else RemoveSkillEffect(idx);
			IncreaseScore();
			break;
		case STOPPEDRANGENPC: //적이면 적 제거하고 스킬 삭제
			StopRangeNpcDie(next.X, next.Y);
			if (type == 3) {
				GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] = 0;
				SetCurrentCursorPos(GBOARD_ORIGIN_X + skillEffectInfo[idx].X * 2, GBOARD_ORIGIN_Y + skillEffectInfo[idx].Y);
				printf("  ");
				skillEffectInfo[idx].X = next.X, skillEffectInfo[idx].Y = next.Y;
			}
			else RemoveSkillEffect(idx);
			IncreaseScore();
			break;
		case NPCBULLET: //적이면 적 제거하고 스킬 삭제
			ProjectileExtinction(next.X, next.Y);
			if (type == 3) {
				GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] = 0;
				SetCurrentCursorPos(GBOARD_ORIGIN_X + skillEffectInfo[idx].X * 2, GBOARD_ORIGIN_Y + skillEffectInfo[idx].Y);
				printf("  ");
				skillEffectInfo[idx].X = next.X, skillEffectInfo[idx].Y = next.Y;
			}
			else RemoveSkillEffect(idx);
			break;
		case BossNum: //보스면 pc 스킬 없어지고 보스 hp 감소
			RemoveSkillEffect(idx);
			DecreaseBossHP();
			break;
		case BossSkillEffect: //보스 스킬이면 보스 스킬 없어지고 pc 스킬 없어짐
			GameBoardInfo[next.Y][next.X] = 0;
			SetCurrentCursorPos(GBOARD_ORIGIN_X + next.X * 2, GBOARD_ORIGIN_Y + next.Y);
			printf("  ");
			RemoveSkillEffect(idx);
			break;
		}
	}
}

//랜덤 스킬 3개 중 하나 선택
void SelectNewSkill() {
	int s[3], l[3];
	int ran1, ran2, ran3;

	flag = 0;

	SetCurrentCursorPos(pos_skillSelect.X, pos_skillSelect.Y);
	textcolor(YELLOW);
	printf("★★★ 스킬 선택 ★★★");

	//스킬 선택 UI 띄우기
	if (selectedSkill[0] == 0 && selectedSkill[1] == 0 && selectedSkill[2] == 0) {
		for (int i = 1; i <= 3; i++) {
			SetCurrentCursorPos(pos_skillSelect.X, pos_skillSelect.Y + i);
			textcolor(WHITE);
			printf("%d) 원거리 : ", i);
			textcolor(SKYBLUE);
			printf("%s", skillList[i - 1][0].name);
		}

		//키 입력받아서 스킬 선택
		while (flag == 0) {
			if (_kbhit() != 0) {
				key = _getch();
				switch (key)
				{
				case ONE:
					selectedSkill[0] = 110, flag = 1;
					break;
				case TWO:
					selectedSkill[0] = 210, flag = 1;
					break;
				case THREE:
					selectedSkill[0] = 310, flag = 1;
					break;
				}
			}
		}
	}
	else {
		ran1 = rand() % 3 + 1;
		ran2 = rand() % 3 + 4;
		ran3 = rand() % 3 + 7;

		if (selectedSkill[0] == 0) s[0] = ran1, l[0] = 1;
		else s[0] = selectedSkill[0] / 100 % 10, l[0] = selectedSkill[0] / 10 % 10 + 1;
		if (selectedSkill[1] == 0) s[1] = ran2, l[1] = 1;
		else s[1] = selectedSkill[1] / 100 % 10, l[1] = selectedSkill[1] / 10 % 10 + 1;
		if (selectedSkill[2] == 0) s[2] = ran3, l[2] = 1;
		else s[2] = selectedSkill[2] / 100 % 10, l[2] = selectedSkill[2] / 10 % 10 + 1;

		for (int i = 1; i <= 3; i++) {
			SetCurrentCursorPos(pos_skillSelect.X, pos_skillSelect.Y + i);
			switch (i)
			{
			case 1:
				textcolor(GRAY);
				printf("%d) 원거리 : ", i);
				if (l[i - 1] <= 3) {
					textcolor(SKYBLUE);
					printf("%s", skillList[s[i - 1] - 1][l[i - 1] - 1].name);
				}
				else {
					textcolor(darkGRAY);
					printf("최고 레벨 도달");
				}
				break;
			case 2:
				textcolor(GRAY);
				printf("%d) 근거리 : ", i);
				if (l[i - 1] <= 3) {
					textcolor(SKYBLUE);
					printf("%s", skillList[s[i - 1] - 1][l[i - 1] - 1].name);
				}
				else {
					textcolor(darkGRAY);
					printf("최고 레벨 도달");
				}
				break;
			case 3:
				textcolor(GRAY);
				printf("%d) 패시브 : ", i);
				if (l[i - 1] <= 3) {
					textcolor(SKYBLUE);
					printf("%s", skillList[s[i - 1] - 1][l[i - 1] - 1].name);
				}
				else {
					textcolor(darkGRAY);
					printf("최고 레벨 도달");
				}
				break;
			}
		}
		
		//키 입력받아서 스킬 선택
		while (flag == 0) {
			if (_kbhit() != 0) {
				key = _getch();
				switch (key)
				{
				case ONE:
					if (l[0] <= 3) selectedSkill[0] = s[0] * 100 + l[0] * 10, flag = 1;
					break;
				case TWO:
					if (l[1] <= 3) selectedSkill[1] = s[1] * 100 + l[1] * 10, flag = 1;
					break;
				case THREE:
					if (l[2] <= 3) {
						selectedSkill[2] = s[2] * 100 + l[2] * 10, flag = 1;
						switch (selectedSkill[2])
						{
						case 710:
							maxHP++;
							HP++;
							drawHP();
							break;
						case 720:
							maxHP += 2;
							HP += 2;
							drawHP();
							break;
						case 730:
							maxHP += 3;
							HP += 3;
							drawHP();
							break;
						case 810:
							Diff.attackSpeed -= 2;
							SetCurrentCursorPos(0, 0);
							printf("%d", Diff.attackSpeed);
							break;
						case 820:
							Diff.attackSpeed -= 2;
							SetCurrentCursorPos(0, 0);
							printf("%d", Diff.attackSpeed);
							break;
						case 830:
							Diff.attackSpeed -= 2;
							SetCurrentCursorPos(0, 0);
							printf("%d", Diff.attackSpeed);
							break;
						case 910:
							Diff.itemRate += 5;
							break;
						case 920:
							Diff.itemRate += 5;
							break;
						case 930:
							Diff.itemRate += 10;
							break;
						}
					}
					break;
				}
			}
		}
	}

	//스킬 선택 UI 지우기
	for (int i = 0; i <= 3; i++) {
		SetCurrentCursorPos(pos_skillSelect.X, pos_skillSelect.Y + i);
		printf("                                  ");
	}
}

//스킬 정의; 1~3: 원거리 / 4~6: 근거리 / 7~9: 패시브
void defineSkill(int type) {
	int ef = 0;
	char effect[3];
	int* p;

	//type: 1~3=원거리, 4~6=근거리, 7~9=패시브
	switch (type)
	{
	default:
		break;
	case 1: //권총
		strcpy(effect, "⊙"); //스킬 이펙트 지정
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			ef = type * 100 + i * 10; //스킬 이펙트 번호; 3자리 정수
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "권총");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				break;
			case 2:
				strcpy(skillList[type - 1][i - 1].name, "자동권총");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = 0, p[1] = ef, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				break;
			case 3:
				strcpy(skillList[type - 1][i - 1].name, "기관총");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = ef, p[1] = ef, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = ef, p[1] = ef, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				break;
			}
		}
		break;
	case 2: //산탄총
		strcpy(effect, "∴"); //스킬 이펙트 지정
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			ef = type * 100 + i * 10; //스킬 이펙트 번호; 3자리 정수
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "레버액션 산탄총");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				break;
			case 2:
				strcpy(skillList[type - 1][i - 1].name, "더블배럴 산탄총");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = ef;
				break;
			case 3:
				strcpy(skillList[type - 1][i - 1].name, "펌프액션 산탄총");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = 0, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = 0, p[1] = 0, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = 0, p[1] = 0, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = ef, p[3] = ef;
				break;
			}
		}
		break;
	case 3: //활
		strcpy(effect, "□"); //스킬 이펙트 지정
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			ef = type * 100 + i * 10; //스킬 이펙트 번호; 3자리 정수
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "단궁");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = 0, p[1] = 0, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				break;
			case 2:
				strcpy(skillList[type - 1][i - 1].name, "장궁");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = ef, p[1] = ef, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				break;
			case 3:
				strcpy(skillList[type - 1][i - 1].name, "대궁");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = ef, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = ef, p[1] = ef, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = 0, p[1] = ef, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				break;
			}
		}
		break;
	case 4: //검
		strcpy(effect, "◈"); //스킬 이펙트 지정
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			ef = type * 100 + i * 10; //스킬 이펙트 번호; 3자리 정수
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "단검");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = 0, p[1] = 0, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				break;
			case 2:
				strcpy(skillList[type - 1][i - 1].name, "장검");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = ef, p[1] = ef, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				break;
			case 3:
				strcpy(skillList[type - 1][i - 1].name, "대검");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = ef, p[1] = ef, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = ef, p[1] = ef, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				break;
			}
		}
		break;
	case 5: //방벽
		strcpy(effect, "▣"); //스킬 이펙트 지정
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			ef = type * 100 + i * 10; //스킬 이펙트 번호; 3자리 정수
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "소형 방벽");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				break;
			case 2:
				strcpy(skillList[type - 1][i - 1].name, "중형 방벽");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = ef;
				break;
			case 3: //ㄷ자로 pc를 감싸는 형태
				strcpy(skillList[type - 1][i - 1].name, "대형 방벽");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = 0, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = 0, p[1] = 0, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = 0, p[1] = 0, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = ef, p[3] = ef;
				break;
			}
		}
		break;
	case 6: //전투망치
		strcpy(effect, "▨"); //스킬 이펙트 지정
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			ef = type * 100 + i * 10; //스킬 이펙트 번호; 3자리 정수
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "한손 전투망치");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = 0, p[1] = 0, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = 0, p[1] = 0, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				break;
			case 2:
				strcpy(skillList[type - 1][i - 1].name, "양손 전투망치");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = 0, p[1] = ef, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = 0, p[1] = ef, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = 0, p[1] = ef, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = 0, p[1] = 0, p[2] = 0, p[3] = 0;
				break;
			case 3:
				strcpy(skillList[type - 1][i - 1].name, "대형 전투망치");
				strcpy(skillList[type - 1][i - 1].e, effect);
				p = skillList[type - 1][i - 1].effect[0];
				p[0] = ef, p[1] = ef, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[1];
				p[0] = ef, p[1] = ef, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[2];
				p[0] = ef, p[1] = ef, p[2] = ef, p[3] = ef;
				p = skillList[type - 1][i - 1].effect[3];
				p[0] = ef, p[1] = ef, p[2] = ef, p[3] = ef;
				break;
			}
		}
		break;
	case 7: //최대 체력 증가
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "최대 체력 조금 증가");
				//maxHP++;
				break;
			case 2:
				strcpy(skillList[type - 1][i - 1].name, "최대 체력 보통 증가");
				//maxHP++;
				break;
			case 3:
				strcpy(skillList[type - 1][i - 1].name, "최대 체력 크게 증가");
				//maxHP += 2;
				break;
			}
		}
		break;
	case 8: //공격 속도 증가
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "공격 속도 조금 증가");
				//Diff.attackSpeed--;
				break;
			case 2:
				strcpy(skillList[type - 1][i - 1].name, "공격 속도 보통 증가");
				//Diff.attackSpeed--;
				break;
			case 3:
				strcpy(skillList[type - 1][i - 1].name, "공격 속도 크게 증가");
				//Diff.attackSpeed -= 2;
				break;
			}
		}
		break;
	case 9: //아이템 획득 확률 증가
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "아이템 확률 조금 증가");
				//Diff.itemRate += 10;
				break;
			case 2:
				strcpy(skillList[type - 1][i - 1].name, "아이템 확률 보통 증가");
				//Diff.itemRate += 10;
				break;
			case 3:
				strcpy(skillList[type - 1][i - 1].name, "아이템 확률 크게 증가");
				//Diff.itemRate += 20;
				break;
			}
		}
		break;
	}
}
