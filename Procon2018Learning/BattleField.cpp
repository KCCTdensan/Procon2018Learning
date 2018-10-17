#include "BattleField.hpp"
#include <iostream>


void battle_field::DeepenNode(action_id Action1, action_id Action2)
{
	node *ChildNode = CurrentNode->Deepen(Action1, Action2);

	delete CurrentNode;
	CurrentNode = ChildNode;
}

battle_field::battle_field()
{
	CurrentNode = new node(nullptr, Stage, Team_1P);
}

battle_field::~battle_field()
{
	delete CurrentNode;
	CurrentNode = nullptr;
}

void battle_field::Battle(int NumTurn)
{
	Stage.PrintStage();
	for (int i = 0; i < NumTurn; ++i)
	{
		using namespace std;
#ifdef _DEBUG
		cout << "================================================" << endl;
		cout << "Action" << i << endl;
#endif

		int Result[ID_MaxID][ID_MaxID];
		CurrentNode->Search(node::NumCallPlay, Result);

		int Max = 0;
		action_id IntentionIDs[NumTeams][stage::NumAgents] = {
			{-1,-1},{-1,-1}
		};
		for(action_id i = 0; i < ID_MaxID; ++i)
		{
			for(action_id j = 0; j < ID_MaxID; ++j)
			{
				if(Max < Result[i][j])
				{
					Max = Result[i][j];
					IntentionIDs[Team_1P][0] = i;
					IntentionIDs[Team_1P][1] = j;
				}
			}
		}
		DeepenNode(IntentionIDs[Team_1P][0], IntentionIDs[Team_1P][1]);

		{
			intention Intention1P_1 = IntentionIDs[Team_1P][0];
			intention Intention1P_2 = IntentionIDs[Team_1P][1];
			cout << "1P-1  x : " << (int)Intention1P_1.DeltaX << " y : " << (int)Intention1P_1.DeltaY << endl;
			cout << "1P-2  x : " << (int)Intention1P_2.DeltaX << " y : " << (int)Intention1P_2.DeltaY << endl;
		}

		CurrentNode->Search(node::NumCallPlay, Result);
		Max = 0;
		for(action_id i = 0; i < ID_MaxID; ++i)
		{
			for(action_id j = 0; j < ID_MaxID; ++j)
			{
				if(Max < Result[i][j])
				{
					Max = Result[i][j];
					IntentionIDs[Team_2P][0] = i;
					IntentionIDs[Team_2P][1] = j;
				}
			}
		}
		DeepenNode(IntentionIDs[Team_2P][0], IntentionIDs[Team_2P][1]);

		Stage.Action(IntentionIDs);

		{
			intention Intention2P_1 = IntentionIDs[Team_2P][0];
			intention Intention2P_2 = IntentionIDs[Team_2P][1];
			cout << "2P-1 : x : " << (int)Intention2P_1.DeltaX << " y : " << (int)Intention2P_1.DeltaY << endl;
			cout << "2P-2 : x : " << (int)Intention2P_2.DeltaX << " y : " << (int)Intention2P_2.DeltaY << endl;
		}
		Stage.PrintStage();
	}
}
