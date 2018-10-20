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
	int t = 0;
	char isContinue;
	stage Stage;
	friend_node *CurrentNode = new friend_node(nullptr, Stage, NumTurn);
	Stage.PrintStage();
	do
	{
		using namespace std;
		int Max = 0;
		action_id UserIntentions[NumTeams][stage::NumAgents] = { { -1,-1 },{ -1,-1 } };
		action_id IntentionIDs[NumTeams][stage::NumAgents] = { {-1,-1},{-1,-1} };
		int Result[ID_MaxID][ID_MaxID];

		cout << "================================================================" << endl;
		cout << "Action" << t << endl;

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
			//CurrentNode->PrintChildNodeInfo();
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
			//CurrentNode->ChildNode(IntentionIDs[Team_1P])->PrintChildNodeInfo();
			cout << "2P-1 : x : " << (int)Intention2P_1.DeltaX << " y : " << (int)Intention2P_1.DeltaY << endl;
			cout << "2P-2 : x : " << (int)Intention2P_2.DeltaX << " y : " << (int)Intention2P_2.DeltaY << endl;
		}
		
		cout << "1 2 3" << endl;
		cout << "4 0 5" << endl;
		cout << "8 7 8" << endl;
		bool Results[NumTeams][stage::NumAgents];
		for (int i  = 0; i < NumTeams; i++)
		{
			cout << "Team" << i+1 << "P:" << endl;
			for (int j = 0; j < stage::NumAgents; j++) 
			{
				
				int UserIntention;
				cout << "Agent" << j+1 << ":" << endl;
				cin >> UserIntention;
				UserIntentions[(i == 0) ? Team_1P : Team_2P][j] = (action_id)UserIntention;
			}
		}
		Stage.CanAction(UserIntentions,Results);
		for (int i = 0; i < NumTeams; i++)
		{
			for (int j = 0; j < stage::NumAgents; j++) 
			{
				if (!Results[i][j]) { UserIntentions[(i == 0) ? Team_1P : Team_2P][j] = (action_id)0; }
			}
		}


		CurrentNode = CurrentNode->UpdateCurrentNode(UserIntentions);

		Stage.Action(UserIntentions);
		std::cout << "Turn" << (int)Stage.GetCntTurn() << endl;

		Stage.PrintStage();
		cout << "1PScore : " << Stage.GetScore1P() << endl;
		cout << "2PScore : " << Stage.GetScore2P() << endl;
		cin >> isContinue;
		t++;
	} while (isContinue == 't');
	delete CurrentNode;
	CurrentNode = nullptr;
}
