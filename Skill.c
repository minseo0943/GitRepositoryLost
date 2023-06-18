#pragma warning(disable:4996)
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
//��� ����
#include "DrawBoard.h"
#include "Cursor.h"
#include "Color.h"
#include "Stage.h"
#include "MainMenu.h"
#include "ProcessKeyInput.h"
#include "PCNPC.h"
#include "Skill.h"

skill skillList[SKILL_TYPE][SKILL_LEVEL]; //��ų ���
int selectedSkill[SELECT_SIZE]; //������ ��ų ��ȣ(110, 120, 130, 210, ...)
skillEffect skillEffectInfo[GBOARD_HEIGHT * GBOARD_WIDTH]; //��ų ����Ʈ ��ǥ ���� �迭
int skillTop = -1; //skillEffectInfo �ִ� �ε���
int flag;
COORD old_cursor; //�������� ���� pc ��ġ
int old_dir; //�������� ���� pc ����

void sub_AddEffectByDirection(COORD cursor, int type, int level, int x, int y, int nx, int ny, int dir) {
	COORD arrCur;
	//���� ó�� 
	if (skillList[type - 1][level - 1].effect[nx][ny] == 0) return; //��ų����Ʈ�� ���� ��� �ѱ��
	if (cursor.X + x < GBOARD_ORIGIN_X - 1 || cursor.X + x > GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 || cursor.Y + y < GBOARD_ORIGIN_Y || cursor.Y + y > GBOARD_ORIGIN_Y + GBOARD_HEIGHT) return; //Ŀ���� ������ ���θ� ����� �ѱ��

	arrCur = ConvertCursorIntoArrCur(cursor);

	//�浹 �˻� �� ����Ʈ ��ȣ�� ������ �迭�� ����
	if (type == 1 || type == 2 || type == 3) {
		switch (GameBoardInfo[arrCur.Y + y][arrCur.X + x])
		{
		default:
			//��Ÿ
			//������ ��ų ����Ʈ ��ǥ�� �迭�� ����
			skillTop++;
			skillEffectInfo[skillTop].X = arrCur.X + x;
			skillEffectInfo[skillTop].Y = arrCur.Y + y;
			skillEffectInfo[skillTop].dir = dir;
			skillEffectInfo[skillTop].level = level;
			skillEffectInfo[skillTop].type = type;
			skillEffectInfo[skillTop].ef = skillList[type - 1][level - 1].effect[nx][ny];
			break;
		case 0:
			//������� ��� ����Ʈ ����ϰ� �����ǿ� �߰��ϰ� ��ų����Ʈ�迭�� ��ǥ ����
			//����Ʈ ���
			SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
			drawSkillShape(type, level);
			//�����ǿ� �߰�
			GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
			//������ ��ų ����Ʈ ��ǥ�� �迭�� ����
			skillTop++;
			skillEffectInfo[skillTop].X = arrCur.X + x;
			skillEffectInfo[skillTop].Y = arrCur.Y + y;
			skillEffectInfo[skillTop].dir = dir;
			skillEffectInfo[skillTop].level = skillList[type - 1][level - 1].level;
			skillEffectInfo[skillTop].type = skillList[type - 1][level - 1].type;
			skillEffectInfo[skillTop].ef = skillList[type - 1][level - 1].effect[nx][ny];
			break;
		case WALL:
			//���̸� �������� �ʰ� �ѱ��
			break;
		case CHAINSAW:
			break;
		case NPC:
			//npc�� npcdie�ϰ� �ѱ��
			NpcDie(arrCur.X + x, arrCur.Y + y);
			IncreaseScore();
			if (type == 3) {
				//����Ʈ ���
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//�����ǿ� �߰�
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				//������ ��ų ����Ʈ ��ǥ�� �迭�� ����
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
			//npc�� ����
			RangeNpcDie(arrCur.X + x, arrCur.Y + y);
			IncreaseScore();
			if (type == 3) {
				//����Ʈ ���
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//�����ǿ� �߰�
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				//������ ��ų ����Ʈ ��ǥ�� �迭�� ����
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
			//npc�� ����
			StopRangeNpcDie(arrCur.X + x, arrCur.Y + y);
			IncreaseScore();
			if (type == 3) {
				//����Ʈ ���
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//�����ǿ� �߰�
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				//������ ��ų ����Ʈ ��ǥ�� �迭�� ����
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
			//npc�� ����
			ProjectileExtinction(arrCur.X + x, arrCur.Y + y);
			if (type == 3) {
				//����Ʈ ���
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//�����ǿ� �߰�
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				//������ ��ų ����Ʈ ��ǥ�� �迭�� ����
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
			//�������� �ʰ� ���� hp ����
			DecreaseBossHP();
			break;
		}
	}
	//��
	else if (type == 4) {
		if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2) {
			//���
			switch (GameBoardInfo[arrCur.Y + y][arrCur.X + x])
			{
			default:
				//��Ÿ
				break;
			case 0:
				//������� ��� ����Ʈ ����ϰ� �����ǿ� �߰��ϰ� ��ų����Ʈ�迭�� ��ǥ ����
				//����Ʈ ���
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//�����ǿ� �߰�
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				break;
			case WALL:
				//���̸� �������� �ʰ� �ѱ��
				break;
			case CHAINSAW:
				break;
			case NPC:
				//npc�� npcdie�ϰ� �ѱ��
				NpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case MOVINGRANGENPC:
				//npc�� ����
				RangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case STOPPEDRANGENPC:
				//npc�� ����
				StopRangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case NPCBULLET:
				//npc�� ����
				ProjectileExtinction(arrCur.X + x, arrCur.Y + y);
				break;
			case BossNum:
				//����Ʈ ������� �ʰ� ���� hp ����
				DecreaseBossHP();
				break;
			case BossSkillEffect:
				//����Ʈ ������� �ʰ� ��������Ʈ ����
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = 0;
				break;
			}

			//�̹� pc ��ǥ �� ���� ���
			old_cursor = cursor;
			old_dir = dir;
		}
		else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 1) {
			//���� ����Ʈ �����
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

			//���
			switch (GameBoardInfo[arrCur.Y + y][arrCur.X + x])
			{
			default:
				//��Ÿ
				break;
			case 0:
				//������� ��� ����Ʈ ����ϰ� �����ǿ� �߰��ϰ� ��ų����Ʈ�迭�� ��ǥ ����
				//����Ʈ ���
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//�����ǿ� �߰�
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				break;
			case WALL:
				//���̸� �������� �ʰ� �ѱ��
				break;
			case CHAINSAW:
				break;
			case NPC:
				//npc�� npcdie�ϰ� �ѱ��
				NpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case MOVINGRANGENPC:
				//npc�� ����
				RangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case STOPPEDRANGENPC:
				//npc�� ����
				StopRangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case NPCBULLET:
				//npc�� ����
				ProjectileExtinction(arrCur.X + x, arrCur.Y + y);
				break;
			case BossNum:
				//����Ʈ ������� �ʰ� ���� hp ����
				DecreaseBossHP();
				break;
			case BossSkillEffect:
				//����Ʈ ������� �ʰ� ��������Ʈ ����
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = 0;
				break;
			}

			//�̹� pc ��ǥ �� ���� ���
			old_cursor = cursor;
			old_dir = dir;
		}
		else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 2) {
			//���� ����Ʈ �����
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

			//���
			switch (GameBoardInfo[arrCur.Y + y][arrCur.X + x])
			{
			default:
				//��Ÿ
				break;
			case 0:
				//������� ��� ����Ʈ ����ϰ� �����ǿ� �߰��ϰ� ��ų����Ʈ�迭�� ��ǥ ����
				//����Ʈ ���
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//�����ǿ� �߰�
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				break;
			case WALL:
				//���̸� �������� �ʰ� �ѱ��
				break;
			case CHAINSAW:
				break;
			case NPC:
				//npc�� npcdie�ϰ� �ѱ��
				NpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case MOVINGRANGENPC:
				//npc�� ����
				RangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case STOPPEDRANGENPC:
				//npc�� ����
				StopRangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case NPCBULLET:
				//npc�� ����
				ProjectileExtinction(arrCur.X + x, arrCur.Y + y);
				break;
			case BossNum:
				//����Ʈ ������� �ʰ� ���� hp ����
				DecreaseBossHP();
				break;
			case BossSkillEffect:
				//����Ʈ ������� �ʰ� ��������Ʈ ����
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = 0;
				break;
			}

			//�̹� pc ��ǥ �� ���� ���
			old_cursor = cursor;
			old_dir = dir;
		}
		else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 3) {
			//���� ����Ʈ �����
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
	//�溮
	else if (type == 5) {
		if (TimeCount % (Diff.attackSpeed * 2) == Diff.attackSpeed / 2) {
			//���
			switch (GameBoardInfo[arrCur.Y + y][arrCur.X + x])
			{
			default:
				//��Ÿ
				break;
			case 0:
				//������� ��� ����Ʈ ����ϰ� �����ǿ� �߰��ϰ� ��ų����Ʈ�迭�� ��ǥ ����
				//����Ʈ ���
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//�����ǿ� �߰�
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				break;
			case WALL:
				//���̸� �������� �ʰ� �ѱ��
				break;
			case CHAINSAW:
				break;
			case NPC:
				//npc�� npcdie�ϰ� �ѱ��
				NpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case MOVINGRANGENPC:
				//npc�� ����
				RangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case STOPPEDRANGENPC:
				//npc�� ����
				StopRangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case NPCBULLET:
				//npc�� ����
				ProjectileExtinction(arrCur.X + x, arrCur.Y + y);
				break;
			case BossNum:
				//����Ʈ ������� �ʰ� ���� hp ����
				DecreaseBossHP();
				break;
			case BossSkillEffect:
				//����Ʈ ������� �ʰ� ��������Ʈ ����
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = 0;
				break;
			}
		}
	}
	//��ġ
	else if (type == 6) {
		if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2) {
			//���
			switch (GameBoardInfo[arrCur.Y + y][arrCur.X + x])
			{
			default:
				//��Ÿ
				break;
			case 0:
				//������� ��� ����Ʈ ����ϰ� �����ǿ� �߰��ϰ� ��ų����Ʈ�迭�� ��ǥ ����
				//����Ʈ ���
				SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
				drawSkillShape(type, level);
				//�����ǿ� �߰�
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = skillList[type - 1][level - 1].effect[nx][ny] + dir;
				break;
			case WALL:
				//���̸� �������� �ʰ� �ѱ��
				break;
			case CHAINSAW:
				break;
			case NPC:
				//npc�� npcdie�ϰ� �ѱ��
				NpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case MOVINGRANGENPC:
				//npc�� ����
				RangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case STOPPEDRANGENPC:
				//npc�� ����
				StopRangeNpcDie(arrCur.X + x, arrCur.Y + y);
				IncreaseScore();
				break;
			case NPCBULLET:
				//npc�� ����
				ProjectileExtinction(arrCur.X + x, arrCur.Y + y);
				break;
			case BossNum:
				//����Ʈ ������� �ʰ� ���� hp ����
				DecreaseBossHP();
				break;
			case BossSkillEffect:
				//����Ʈ ������� �ʰ� ��������Ʈ ����
				GameBoardInfo[arrCur.Y + y][arrCur.X + x] = 0;
				break;
			}

			//�̹� pc ��ǥ �� ���� ���
			old_cursor = cursor;
			old_dir = dir;
		}
		else if (TimeCount % Diff.attackSpeed == Diff.attackSpeed / 2 + 1) {
			//���� ����Ʈ �����
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
	//���� ó��
	if (skillList[type - 1][level - 1].effect[nx][ny] == 0) return; //��ų����Ʈ�� ���� ��� �ѱ��
	if (cursor.X + x < GBOARD_ORIGIN_X - 1 || cursor.X + x > GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 || cursor.Y + y < GBOARD_ORIGIN_Y || cursor.Y + y > GBOARD_ORIGIN_Y + GBOARD_HEIGHT) return; //Ŀ���� ������ ���θ� ����� �ѱ��
	
	COORD arrCur = ConvertCursorIntoArrCur(cursor);

	//���� ���� ����Ʈ �����
	if (GameBoardInfo[arrCur.Y + y][arrCur.X + x] >= 400) {
		GameBoardInfo[arrCur.Y + y][arrCur.X + x] = 0;
		SetCurrentCursorPos(cursor.X + x * 2, cursor.Y + y);
		printf("  ");
	}
}

void AddEffectByDirection(int skillNum, int dir) {
	if (skillNum == 0) return; //��ų�� ������ ����

	COORD cursor;
	int type = (skillNum / 100) % 10;
	int level = (skillNum / 10) % 10;

	if (type < 1 || type > 9 || level < 1 || level > 3) return; //���� ó��

	pc.X = PCPoint[0];
	pc.Y = PCPoint[1];

	flag = 0;

	//���� ����Ʈ �׸���
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

	//�����
	/*SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 4, 25);
	printf("���%d", TimeCount);
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + (x + 2) * 4, 26 + y);
			printf("%03d,", skillList[type - 1][level - 1].effect[y][x]);
		}
	}*/
}

//skillEffectInfo[idx] ����
void RemoveSkillEffect(int idx) {
	//���� ó��
	if (idx == 0 && skillEffectInfo[idx].X == 0 && skillEffectInfo[idx].Y == 0) return;

	if (GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] > 100) {
		//����Ʈ �����
		SetCurrentCursorPos(GBOARD_ORIGIN_X + skillEffectInfo[idx].X * 2, GBOARD_ORIGIN_Y + skillEffectInfo[idx].Y);
		printf("  ");
		//�����ǿ��� ����
		GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] = 0;
	}

	//��ų����Ʈ�迭���� ����
	skillEffectInfo[idx].X = 0, skillEffectInfo[idx].Y = 0;

	int cnt = 0;
	for (int i = 0; i <= skillTop; i++) {
		while (i + cnt <= skillTop) {
			if (skillEffectInfo[i + cnt].X == 0 && skillEffectInfo[i + cnt].Y == 0) {
				//0 ����
				cnt++;
				skillTop--;
			}
			else break;
		}
		skillEffectInfo[i] = skillEffectInfo[i + cnt];
	}

	//flag ��ȯ
	if (cnt != 0) flag = 1;
}

//�߻�� ��ų ������Ʈ
void SkillEffectMove() {
	for (int i = 0; i <= skillTop; i++) {
		//��ų ��ǥ ������Ʈ
		flag = 0; //skillEffectInfo[i]�� �����ϸ� flag=1
		UpdateSkillEffect(i);
		if (flag == 1) i--; //���� ����Ʈ�� �����ϰ� �迭�� �� ĭ �������� ������Ƿ� ���� ĭ�� ���� ����Ʈ�� �����Ƿ� ���� ����Ʈ�� �����ϱ� ���� i ����

		//��ų ����Ʈ ��ü ���� �׸���
		for (int i = 0; i <= skillTop; i++) {
			if (GameBoardInfo[skillEffectInfo[i].Y][skillEffectInfo[i].X] == PC || GameBoardInfo[skillEffectInfo[i].Y][skillEffectInfo[i].X] == LIFE || GameBoardInfo[skillEffectInfo[i].Y][skillEffectInfo[i].X] == BOMB || GameBoardInfo[skillEffectInfo[i].Y][skillEffectInfo[i].X] / 100 == 5) continue;
			
			GameBoardInfo[skillEffectInfo[i].Y][skillEffectInfo[i].X] = skillEffectInfo[i].ef;
			SetCurrentCursorPos(GBOARD_ORIGIN_X + skillEffectInfo[i].X * 2, GBOARD_ORIGIN_Y + skillEffectInfo[i].Y);
			drawSkillShape(skillEffectInfo[i].type, skillEffectInfo[i].level);
		}
	}
}

//���� ��ų ����Ʈ �׸���
void UpdateSkillEffect(int idx) {
	int ef = GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X]; //��ų ��ȣ
	int type = skillEffectInfo[idx].type; //��ų Ÿ��
	int level = skillEffectInfo[idx].level; //��ų ����
	int dir = skillEffectInfo[idx].dir; //��ų ����
	COORD next;
	next.X = skillEffectInfo[idx].X, next.Y = skillEffectInfo[idx].Y;

	if (type == 1 || type == 2 || type == 3) {
		//��ų �迭 �ֽ�ȭ
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
		default: //��Ÿ; PC, ����Ʈ, ������ ��
		   //����Ʈ �����
			if (GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] != PC && GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] != LIFE && GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] != BOMB && GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] / 100 != 5) {
				GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] = 0;
				SetCurrentCursorPos(GBOARD_ORIGIN_X + skillEffectInfo[idx].X * 2, GBOARD_ORIGIN_Y + skillEffectInfo[idx].Y);
				printf("  ");
			}
			//��ų ��ǥ�� �̵� ����
			skillEffectInfo[idx].X = next.X, skillEffectInfo[idx].Y = next.Y;
			break;
		case 0:
			//����Ʈ �����
			if (GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] != PC && GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] != LIFE && GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] != BOMB && GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] / 100 != 5) {
				GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] = 0;
				SetCurrentCursorPos(GBOARD_ORIGIN_X + skillEffectInfo[idx].X * 2, GBOARD_ORIGIN_Y + skillEffectInfo[idx].Y);
				printf("  ");
			}
			//�ƹ��͵� ���� �� ������ �̵� ��ġ�� ��ų �̸� ����, �迭�� ��ǥ ����, ��ų �׸���
			skillEffectInfo[idx].X = next.X, skillEffectInfo[idx].Y = next.Y;
			break;
		case WALL: //���̸� ��ų ����
			RemoveSkillEffect(idx);
			break;
		case CHAINSAW:
			RemoveSkillEffect(idx);
			break;
		case NPC: //���̸� �� �����ϰ� ��ų ����
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
		case MOVINGRANGENPC: //���̸� �� �����ϰ� ��ų ����
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
		case STOPPEDRANGENPC: //���̸� �� �����ϰ� ��ų ����
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
		case NPCBULLET: //���̸� �� �����ϰ� ��ų ����
			ProjectileExtinction(next.X, next.Y);
			if (type == 3) {
				GameBoardInfo[skillEffectInfo[idx].Y][skillEffectInfo[idx].X] = 0;
				SetCurrentCursorPos(GBOARD_ORIGIN_X + skillEffectInfo[idx].X * 2, GBOARD_ORIGIN_Y + skillEffectInfo[idx].Y);
				printf("  ");
				skillEffectInfo[idx].X = next.X, skillEffectInfo[idx].Y = next.Y;
			}
			else RemoveSkillEffect(idx);
			break;
		case BossNum: //������ pc ��ų �������� ���� hp ����
			RemoveSkillEffect(idx);
			DecreaseBossHP();
			break;
		case BossSkillEffect: //���� ��ų�̸� ���� ��ų �������� pc ��ų ������
			GameBoardInfo[next.Y][next.X] = 0;
			SetCurrentCursorPos(GBOARD_ORIGIN_X + next.X * 2, GBOARD_ORIGIN_Y + next.Y);
			printf("  ");
			RemoveSkillEffect(idx);
			break;
		}
	}
}

//���� ��ų 3�� �� �ϳ� ����
void SelectNewSkill() {
	int s[3], l[3];
	int ran1, ran2, ran3;

	flag = 0;

	SetCurrentCursorPos(pos_skillSelect.X, pos_skillSelect.Y);
	textcolor(YELLOW);
	printf("�ڡڡ� ��ų ���� �ڡڡ�");

	//��ų ���� UI ����
	if (selectedSkill[0] == 0 && selectedSkill[1] == 0 && selectedSkill[2] == 0) {
		for (int i = 1; i <= 3; i++) {
			SetCurrentCursorPos(pos_skillSelect.X, pos_skillSelect.Y + i);
			textcolor(WHITE);
			printf("%d) ���Ÿ� : ", i);
			textcolor(SKYBLUE);
			printf("%s", skillList[i - 1][0].name);
		}

		//Ű �Է¹޾Ƽ� ��ų ����
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
				printf("%d) ���Ÿ� : ", i);
				if (l[i - 1] <= 3) {
					textcolor(SKYBLUE);
					printf("%s", skillList[s[i - 1] - 1][l[i - 1] - 1].name);
				}
				else {
					textcolor(darkGRAY);
					printf("�ְ� ���� ����");
				}
				break;
			case 2:
				textcolor(GRAY);
				printf("%d) �ٰŸ� : ", i);
				if (l[i - 1] <= 3) {
					textcolor(SKYBLUE);
					printf("%s", skillList[s[i - 1] - 1][l[i - 1] - 1].name);
				}
				else {
					textcolor(darkGRAY);
					printf("�ְ� ���� ����");
				}
				break;
			case 3:
				textcolor(GRAY);
				printf("%d) �нú� : ", i);
				if (l[i - 1] <= 3) {
					textcolor(SKYBLUE);
					printf("%s", skillList[s[i - 1] - 1][l[i - 1] - 1].name);
				}
				else {
					textcolor(darkGRAY);
					printf("�ְ� ���� ����");
				}
				break;
			}
		}
		
		//Ű �Է¹޾Ƽ� ��ų ����
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

	//��ų ���� UI �����
	for (int i = 0; i <= 3; i++) {
		SetCurrentCursorPos(pos_skillSelect.X, pos_skillSelect.Y + i);
		printf("                                  ");
	}
}

//��ų ����; 1~3: ���Ÿ� / 4~6: �ٰŸ� / 7~9: �нú�
void defineSkill(int type) {
	int ef = 0;
	char effect[3];
	int* p;

	//type: 1~3=���Ÿ�, 4~6=�ٰŸ�, 7~9=�нú�
	switch (type)
	{
	default:
		break;
	case 1: //����
		strcpy(effect, "��"); //��ų ����Ʈ ����
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			ef = type * 100 + i * 10; //��ų ����Ʈ ��ȣ; 3�ڸ� ����
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "����");
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
				strcpy(skillList[type - 1][i - 1].name, "�ڵ�����");
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
				strcpy(skillList[type - 1][i - 1].name, "�����");
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
	case 2: //��ź��
		strcpy(effect, "��"); //��ų ����Ʈ ����
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			ef = type * 100 + i * 10; //��ų ����Ʈ ��ȣ; 3�ڸ� ����
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "�����׼� ��ź��");
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
				strcpy(skillList[type - 1][i - 1].name, "����跲 ��ź��");
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
				strcpy(skillList[type - 1][i - 1].name, "�����׼� ��ź��");
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
	case 3: //Ȱ
		strcpy(effect, "��"); //��ų ����Ʈ ����
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			ef = type * 100 + i * 10; //��ų ����Ʈ ��ȣ; 3�ڸ� ����
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "�ܱ�");
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
				strcpy(skillList[type - 1][i - 1].name, "���");
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
				strcpy(skillList[type - 1][i - 1].name, "���");
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
	case 4: //��
		strcpy(effect, "��"); //��ų ����Ʈ ����
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			ef = type * 100 + i * 10; //��ų ����Ʈ ��ȣ; 3�ڸ� ����
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "�ܰ�");
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
				strcpy(skillList[type - 1][i - 1].name, "���");
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
				strcpy(skillList[type - 1][i - 1].name, "���");
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
	case 5: //�溮
		strcpy(effect, "��"); //��ų ����Ʈ ����
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			ef = type * 100 + i * 10; //��ų ����Ʈ ��ȣ; 3�ڸ� ����
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "���� �溮");
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
				strcpy(skillList[type - 1][i - 1].name, "���� �溮");
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
			case 3: //���ڷ� pc�� ���δ� ����
				strcpy(skillList[type - 1][i - 1].name, "���� �溮");
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
	case 6: //������ġ
		strcpy(effect, "��"); //��ų ����Ʈ ����
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			ef = type * 100 + i * 10; //��ų ����Ʈ ��ȣ; 3�ڸ� ����
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "�Ѽ� ������ġ");
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
				strcpy(skillList[type - 1][i - 1].name, "��� ������ġ");
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
				strcpy(skillList[type - 1][i - 1].name, "���� ������ġ");
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
	case 7: //�ִ� ü�� ����
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "�ִ� ü�� ���� ����");
				//maxHP++;
				break;
			case 2:
				strcpy(skillList[type - 1][i - 1].name, "�ִ� ü�� ���� ����");
				//maxHP++;
				break;
			case 3:
				strcpy(skillList[type - 1][i - 1].name, "�ִ� ü�� ũ�� ����");
				//maxHP += 2;
				break;
			}
		}
		break;
	case 8: //���� �ӵ� ����
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "���� �ӵ� ���� ����");
				//Diff.attackSpeed--;
				break;
			case 2:
				strcpy(skillList[type - 1][i - 1].name, "���� �ӵ� ���� ����");
				//Diff.attackSpeed--;
				break;
			case 3:
				strcpy(skillList[type - 1][i - 1].name, "���� �ӵ� ũ�� ����");
				//Diff.attackSpeed -= 2;
				break;
			}
		}
		break;
	case 9: //������ ȹ�� Ȯ�� ����
		for (int i = 1; i <= SKILL_LEVEL; i++) {
			skillList[type - 1][i - 1].type = type;
			skillList[type - 1][i - 1].level = i;

			switch (i)
			{
			case 1:
				strcpy(skillList[type - 1][i - 1].name, "������ Ȯ�� ���� ����");
				//Diff.itemRate += 10;
				break;
			case 2:
				strcpy(skillList[type - 1][i - 1].name, "������ Ȯ�� ���� ����");
				//Diff.itemRate += 10;
				break;
			case 3:
				strcpy(skillList[type - 1][i - 1].name, "������ Ȯ�� ũ�� ����");
				//Diff.itemRate += 20;
				break;
			}
		}
		break;
	}
}
