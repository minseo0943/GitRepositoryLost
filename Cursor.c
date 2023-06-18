#include <Windows.h>
#include "Cursor.h"
#include "DrawBoard.h"

//커서 제거 함수
void RemoveCursor()
{
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

//커서 위치 지정 함수
void SetCurrentCursorPos(int x, int y)
{
    COORD position = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

//현재 커서 위치 반환하는 함수
COORD GetCurrentCursorPos()
{
    COORD curPoint;
    CONSOLE_SCREEN_BUFFER_INFO curInfo;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curPoint.X = curInfo.dwCursorPosition.X;
    curPoint.Y = curInfo.dwCursorPosition.Y;

    return curPoint;
}

//커서의 좌표를 게임판 배열 위치로 변환하여 반환
COORD ConvertCursorIntoArrCur(COORD cursor) {
    COORD arrCur;
    arrCur.X = (cursor.X - GBOARD_ORIGIN_X) / 2;
    arrCur.Y = cursor.Y - GBOARD_ORIGIN_Y;
    return arrCur;
}