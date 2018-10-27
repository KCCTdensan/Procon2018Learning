#include "Export.h"


namespace
{
	friend_node *CurrentNode[stage::MaxTurns] = {};
	unsigned char CntTurn;
}

void __stdcall Init(const char *QRCodeString)
{
	CurrentNode[0] = new friend_node(*(new stage(QRCodeString)));
	CntTurn = 0;
}

void __stdcall Deinit()
{
	if (CurrentNode[0] == nullptr)
	{
		return;
	}
	delete CurrentNode[0];
	for (int i = 0; i < stage::MaxTurns; ++i)
	{
		CurrentNode[i] = nullptr;
	}
}

void __stdcall SetNumTurns(int NumTurns)
{
	CurrentNode[CntTurn]->ChangeNumTurns((unsigned char)NumTurns);
}

void __stdcall BestMove(action_id *ID1, action_id *ID2)
{
	int Result[ID_MaxID][ID_MaxID];

	CurrentNode[CntTurn]->Search(NumCallPlay);
	CurrentNode[CntTurn]->Result(Result);
	int Max = 0;
	for (action_id i = 0; i < ID_MaxID; ++i)
	{
		for (action_id j = 0; j < ID_MaxID; ++j)
		{
			if (Max < Result[i][j])
			{
				Max = Result[i][j];
				*ID1 = i;
				*ID2 = j;
			}
		}
	}

	CurrentNode[CntTurn]->ChildNode(*ID1, *ID2)->Search(NumCallPlay);
	CurrentNode[CntTurn]->ChildNode(*ID1, *ID2)->Result(Result);
	Max = 0;
	for (action_id i = 0; i < ID_MaxID; ++i)
	{
		for (action_id j = 0; j < ID_MaxID; ++j)
		{
			if (Max < Result[i][j])
			{
				Max = Result[i][j];
				*ID1 = i;
				*ID2 = j;
			}
		}
	}
}

void __stdcall Move(action_id FriendID1, action_id FriendID2, action_id OpponentID1, action_id OpponentID2)
{
	action_id IDs[][stage::NumAgents] = {
		{FriendID1, FriendID2}, {OpponentID1, OpponentID2}
	};
	CurrentNode[CntTurn + 1] = CurrentNode[CntTurn]->NextNode(IDs);
	CntTurn++;
}