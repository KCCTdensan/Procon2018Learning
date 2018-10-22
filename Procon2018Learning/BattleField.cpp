#include "BattleField.hpp"
#include <iostream>


battle_field::battle_field(stage &Stage)
{
	Stages[0] = new stage(Stage);
	for (int i = 1; i < 80; ++i)
	{
		Stages[i] = nullptr;
		CurrentNodes[i] = nullptr;
	}
}

battle_field::~battle_field()
{
	for (int i = 0; i < 80; ++i)
	{
		if (Stages[i] == nullptr)
		{
			break;
		}
		delete Stages[i];
		Stages[i] = nullptr;
	}
}

void battle_field::Battle(int NumTurn)
{
	int CntTurn = 0;
	char isContinue;
	CurrentNodes[0] = new friend_node(nullptr, *Stages[0], NumTurn);
	friend_node *CurrentNode = CurrentNodes[0];
	Stages[0]->PrintStage();
	do
	{
		using namespace std;
		int Max = 0;
		action_id UserIntentions[NumTeams][stage::NumAgents] = {{-1, -1}, {-1, -1}};
		action_id IntentionIDs[NumTeams][stage::NumAgents] = {{-1, -1}, {-1, -1}};
		int Result[ID_MaxID][ID_MaxID];

		cout << "================================================================" << endl;
		cout << "Action" << CntTurn << endl;

		CurrentNode->Search(node::NumCallPlay);
		CurrentNode->Result(Result);

		//Stages[CntTurn] = new stage(Stages);
		CurrentNodes[CntTurn] = CurrentNode;

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
			cout << "1P-1  x : " << (int)Intention1P_1.DeltaX << " y : " << (int)Intention1P_1.DeltaY << endl;
			cout << "ID:[ " << (int)IntentionIDs[Team_1P][0] << " ]" << endl;
			cout << "1P-2  x : " << (int)Intention1P_2.DeltaX << " y : " << (int)Intention1P_2.DeltaY << endl;
			cout << "ID:[ " << (int)IntentionIDs[Team_1P][1] << " ]" << endl;
		}
		if (CurrentNode == nullptr) { cout << "null1" << endl; }
		if (CurrentNode->ChildNode(IntentionIDs[Team_1P]) == nullptr) { cout << "null2" << endl; }
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
			cout << "2P-1 : x : " << (int)Intention2P_1.DeltaX << " y : " << (int)Intention2P_1.DeltaY << endl;
			cout << "ID: [" << (int)IntentionIDs[Team_2P][0] << " ]" << endl;
			cout << "2P-2 : x : " << (int)Intention2P_2.DeltaX << " y : " << (int)Intention2P_2.DeltaY << endl;
			cout << "ID: [" << (int)IntentionIDs[Team_2P][1] << " ]" << endl;
		}
		
		cout << "=====================Input intentionIDs =====================" << endl;
		cout << "1 2 3" << endl;
		cout << "4 0 5" << endl;
		cout << "6 7 8" << endl;
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

		CntTurn++;

		cout << "================Redo or Continue or End===================" << endl;
		do
		{
			cout << "0‚ÅI—¹,‚»‚Ì‘¼‚Å‘±s,r‚ÅŒ³‚É–ß‚·" << endl;
			cin >> isContinue;
			if (isContinue == 'r' && CntTurn>0)
			{
				CntTurn--;
				Stage = Stages[CntTurn];
				CurrentNode = CurrentNodes[CntTurn];
				cout << "Stage:" << endl;
				Stage.PrintStage();
				cout << "Node:" << endl;
				CurrentNode->PrintStage();
			}
		} while (isContinue == 'r' && CntTurn>0);
		
	} while (isContinue != '0');
	delete CurrentNode;
	CurrentNode = nullptr;
}

void battle_field::Play()
{
}
