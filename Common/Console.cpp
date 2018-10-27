#include "Console.hpp"
#include <Windows.h>


void ChangeColor(color_id CharColor, color_id BackColor)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD attr = 0;
	if (CharColor & MASK_INTENSITY)
	{
		attr |= FOREGROUND_INTENSITY;
	}
	if (CharColor & MASK_RED)
	{
		attr |= FOREGROUND_RED;
	}
	if (CharColor & MASK_GREEN)
	{
		attr |= FOREGROUND_GREEN;
	}
	if (CharColor & MASK_BLUE)
	{
		attr |= FOREGROUND_BLUE;
	}
	if (BackColor & MASK_INTENSITY)
	{
		attr |= BACKGROUND_INTENSITY;
	}
	if (BackColor & MASK_RED)
	{
		attr |= BACKGROUND_RED;
	}
	if (BackColor & MASK_GREEN)
	{
		attr |= BACKGROUND_GREEN;
	}
	if (BackColor & MASK_BLUE)
	{
		attr |= BACKGROUND_BLUE;
	}
	SetConsoleTextAttribute(hConsole, attr);
}
