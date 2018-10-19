#include "BattleField.hpp"
#include <iostream>


battle_field::battle_field()
{

}

battle_field::~battle_field()
{

}

void battle_field::Battle(int NumTurn)
{
	friend_node *CurrentNode = new friend_node(nullptr, Stage, NumTurn);
	Stage.PrintStage();
	for (int i = 0; i < NumTurn; ++i)
	{
		using namespace std;
		int Max = 0;
		action_id IntentionIDs[NumTeams][stage::NumAgents] = { {-1,-1},{-1,-1} };
		int Result[ID_MaxID][ID_MaxID];

#ifdef _DEBUG
		cout << "================================================" << endl;
		cout << "Action" << i << endl;
#endif

		CurrentNode->Search(node::NumCallPlay);
		CurrentNode->Result(Result);

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

		{
			intention Intention1P_1 = IntentionIDs[Team_1P][0];
			intention Intention1P_2 = IntentionIDs[Team_1P][1];
			CurrentNode->PrintChildNodeInfo();
			cout << "1P-1  x : " << (int)Intention1P_1.DeltaX << " y : " << (int)Intention1P_1.DeltaY << endl;
			cout << "1P-2  x : " << (int)Intention1P_2.DeltaX << " y : " << (int)Intention1P_2.DeltaY << endl;
		}

		CurrentNode->ChildNode(IntentionIDs[Team_1P])->Search(node::NumCallPlay);
		CurrentNode->ChildNode(IntentionIDs[Team_1P])->Result(Result);
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
		
		{
			intention Intention2P_1 = IntentionIDs[Team_2P][0];
			intention Intention2P_2 = IntentionIDs[Team_2P][1];
			CurrentNode->ChildNode(IntentionIDs[Team_1P])->PrintChildNodeInfo();
			cout << "2P-1 : x : " << (int)Intention2P_1.DeltaX << " y : " << (int)Intention2P_1.DeltaY << endl;
			cout << "2P-2 : x : " << (int)Intention2P_2.DeltaX << " y : " << (int)Intention2P_2.DeltaY << endl;
		}
		
		CurrentNode = CurrentNode->UpdateCurrentNode(IntentionIDs);
		Stage.Action(IntentionIDs);

		Stage.PrintStage();
		CurrentNode->PrintStage();
		cout << "1PScore : " << Stage.GetScore1P() << endl;
		cout << "2PScore : " << Stage.GetScore2P() << endl;
	}
	delete CurrentNode;
	CurrentNode = nullptr;
}
