#include "Export.h"


namespace
{
	friend_node *CurrentNode[stage::MaxTurns] = {};
}

void __stdcall Init(const char *QRCodeString)
{
	CurrentNode[0] = new friend_node(*(new stage(QRCodeString)));
}

void AI_API Deinit()
{
	for (int i = 0; i < stage::MaxTurns; ++i)
	{
		if (CurrentNode[i] == nullptr)
		{
			continue;
		}
		delete CurrentNode[i];
		CurrentNode[i] = nullptr;
	}
}

