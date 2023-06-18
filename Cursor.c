#include <Windows.h>
#include "Cursor.h"
#include "DrawBoard.h"

//Ŀ�� ���� �Լ�
void RemoveCursor()
{
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

//Ŀ�� ��ġ ���� �Լ�
void SetCurrentCursorPos(int x, int y)
{
    COORD position = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

//���� Ŀ�� ��ġ ��ȯ�ϴ� �Լ�
COORD GetCurrentCursorPos()
{
    COORD curPoint;
    CONSOLE_SCREEN_BUFFER_INFO curInfo;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curPoint.X = curInfo.dwCursorPosition.X;
    curPoint.Y = curInfo.dwCursorPosition.Y;

    return curPoint;
}

//Ŀ���� ��ǥ�� ������ �迭 ��ġ�� ��ȯ�Ͽ� ��ȯ
COORD ConvertCursorIntoArrCur(COORD cursor) {
    COORD arrCur;
    arrCur.X = (cursor.X - GBOARD_ORIGIN_X) / 2;
    arrCur.Y = cursor.Y - GBOARD_ORIGIN_Y;
    return arrCur;
}