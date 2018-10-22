#include "BattleField.hpp"
#include <iostream>


battle_field::battle_field(stage &Stage)
	:AI(Stage)
{

}

battle_field::~battle_field()
{

}

void battle_field::Battle(int NumTurns)
{
	AI.NumTurns(NumTurns);
	AI.PrintStage();
	for (int i = 0; i < NumTurns; ++i)
	{
		Play();
	}
}

void battle_field::Play()
{
	using namespace std;
	int Max = 0;
	action_id IntentionIDs[NumTeams][stage::NumAgents] = { {-1,-1},{-1,-1} };

	cout << "================================================================" << endl;

	AI.BestMove(IntentionIDs);

	{
		intention Intention1P_1 = IntentionIDs[Team_1P][0];
		intention Intention1P_2 = IntentionIDs[Team_1P][1];
		intention Intention2P_1 = IntentionIDs[Team_2P][0];
		intention Intention2P_2 = IntentionIDs[Team_2P][1];
		cout << "1P-1  x : " << (int)Intention1P_1.DeltaX << " y : " << (int)Intention1P_1.DeltaY << endl;
		cout << "1P-2  x : " << (int)Intention1P_2.DeltaX << " y : " << (int)Intention1P_2.DeltaY << endl;
		cout << "2P-1 : x : " << (int)Intention2P_1.DeltaX << " y : " << (int)Intention2P_1.DeltaY << endl;
		cout << "2P-2 : x : " << (int)Intention2P_2.DeltaX << " y : " << (int)Intention2P_2.DeltaY << endl;
	}

	AI.Move(IntentionIDs);
	AI.PrintStage();
}
