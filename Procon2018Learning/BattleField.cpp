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
	//stage Stage(QRtext);
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

		Stages[t] = Stage;
		CurrentNodes[t] = CurrentNode;

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
			cout << "ID:[ " << intoCard((int)IntentionIDs[Team_1P][0]) << " ]" << endl;
			cout << "1P-2  x : " << (int)Intention1P_2.DeltaX << " y : " << (int)Intention1P_2.DeltaY << endl;
			cout << "ID:[ " << intoCard((int)IntentionIDs[Team_1P][1]) << " ]" << endl;
		}
		if (CurrentNode == nullptr) { cout << "null1" << endl; }
		if (CurrentNode->ChildNode(IntentionIDs[Team_1P]) == nullptr) { cout << "null2" << endl; }
		//CurrentNode->ChildNode(IntentionIDs[Team_1P])->Search(node::NumCallPlay);
		//CurrentNode->ChildNode(IntentionIDs[Team_1P])->Result(Result);
		/*Max = 0;
			//CurrentNode->ChildNode(IntentionIDs[Team_1P])->PrintChildNodeInfo();
			cout << "2P-1 : x : " << (int)Intention2P_1.DeltaX << " y : " << (int)Intention2P_1.DeltaY << endl;
			cout << "ID: [" << intoCard((int)IntentionIDs[Team_2P][0]) << " ]" << endl;
			cout << "2P-2 : x : " << (int)Intention2P_2.DeltaX << " y : " << (int)Intention2P_2.DeltaY << endl;
			cout << "ID: [" << intoCard((int)IntentionIDs[Team_2P][1]) << " ]" << endl;
		}*/
		
		cout << "=====================Input intentionIDs =====================" << endl;
		cout << "7 8 9" << endl;
		cout << "4 5 6" << endl;
		cout << "1 2 3" << endl;
		bool Results[NumTeams][stage::NumAgents];
		for (int j = 0; j < stage::NumAgents; j++) 
		{
			UserIntentions[Team_1P][j] = IntentionIDs[Team_1P][j];
		}
		for (int j = 0; j < stage::NumAgents; j++)
		{
			cout << "Team2P" << "Agent" << j + 1 << ":" << endl;
			int _IntentionID;
			cin >> _IntentionID;
			UserIntentions[Team_2P][j] = intoID(_IntentionID);
		}
		cout << "=====================End Input intentionIDs =====================" << endl;
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

		cout << "Stage;" << endl;
		Stage.PrintStage();
		cout << "Node" << endl;
		CurrentNode->PrintStage();
		cout << "1PScore : " << Stage.GetScore1P() << endl;
		cout << "2PScore : " << Stage.GetScore2P() << endl;

		t++;		
	} while (t < 41);
	delete CurrentNode;
	CurrentNode = nullptr;
}

int battle_field::intoCard(action_id _IntentinoID)
{
	switch (_IntentinoID)
	{
	case 0: return 5;
			break;
	case 1: return 7;
			break;
	case 2: return 8;
			break;
	case 3: return 9;
			break;
	case 4: return 4;
			break;
	case 5: return 6;
			break;
	case 6: return 1;
			break;
	case 7: return 2;
			break;
	case 8: return 3;
			break;
	}
}
int battle_field::intoID(int _Card)
{
	switch (_Card)
	{
	case 5: return 0;
			break;
	case 7: return 1;
			break;
	case 8: return 2;
			break;
	case 9: return 3;
			break;
	case 4: return 4;
			break;
	case 6: return 5;
			break;
	case 1: return 6;
			break;
	case 2: return 7;
			break;
	case 3: return 8;
			break;
	}
}
