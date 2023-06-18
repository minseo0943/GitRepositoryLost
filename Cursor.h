#pragma once
#include <Windows.h>

void RemoveCursor(); 
void SetCurrentCursorPos(int x, int y); 
COORD GetCurrentCursorPos();
COORD ConvertCursorIntoArrCur(COORD cursor);