#pragma once


typedef char intention_action;
enum
{
	IA_MoveAgent,
	IA_RemovePanel
};

typedef char action_id;
enum
{
	None = -1,
	Stay,
	Move_TopLeft,
	Move_Top,
	Move_TopRight,
	Move_Left,
	Move_Right,
	Move_BottomLeft,
	Move_Bottom,
	Move_BottomRight,
	Remove_TopLeft,
	Remove_Top,
	Remove_TopRight,
	Remove_Left,
	Remove_Right,
	Remove_BottomLeft,
	Remove_Bottom,
	Remove_BottomRight
};

struct intention
{
	int DeltaX;
	int DeltaY;
	intention_action Action;
	intention(action_id);
};