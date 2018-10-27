#pragma once

#include "Game.hpp"


typedef char color_id;
enum
{
	COL_BLACK,
	COL_DARKBLUE,
	COL_DARKGREEN,
	COL_DARKCYAN,
	COL_DARKRED,
	COL_DARKVIOLET,
	COL_DARKYELLOW,
	COL_GRAY,
	COL_LIGHTGRAY,
	COL_BLUE,
	COL_GREEN,
	COL_CYAN,
	COL_RED,
	COL_VIOLET,
	COL_YELLOW,
	COL_WHITE,
	MASK_INTENSITY = 0x08,
	MASK_RED = 0x04,
	MASK_GREEN = 0x02,
	MASK_BLUE = 0x01
};

const color_id BackColors[][stage::NumAgents] = {
	{COL_BLUE, COL_DARKBLUE}, {COL_RED, COL_DARKRED}
};

void ChangeColor(color_id CharColor, color_id BackColor);