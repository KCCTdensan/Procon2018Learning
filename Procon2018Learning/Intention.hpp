#pragma once


typedef char action_id;
enum
{
	ID_None = -1,
	ID_Stay,
	ID_TopLeft,
	ID_Top,
	ID_TopRight,
	ID_Left,
	ID_Right,
	ID_BottomLeft,
	ID_Bottom,
	ID_BottomRight,
	ID_MaxID
};

struct intention
{
	char DeltaX;
	char DeltaY;

	intention();
	intention(action_id);
};