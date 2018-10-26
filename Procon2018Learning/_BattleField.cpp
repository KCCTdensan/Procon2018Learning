#include "BattleField.hpp"
#include "File.hpp"
#include <iostream>


std::string battle_field::MakeFileName(const stage &Stage)
{
	using namespace std;
	string String = "TrainingData/ProconAITrainingData-";
	String += to_string(Stage.GetNumX());
	String += 'x';
	String += to_string(Stage.GetNumY());
	String += '-';
	String += to_string(abs(Stage.GetPanels()[0][0].GetScore()));
	String += to_string(abs(Stage.GetPanels()[0][1].GetScore()));
	String += to_string(abs(Stage.GetPanels()[0][2].GetScore()));
	String += to_string(abs(Stage.GetPanels()[1][0].GetScore()));
	String += to_string(abs(Stage.GetPanels()[1][1].GetScore()));
	String += to_string(abs(Stage.GetPanels()[1][2].GetScore()));
	String += to_string(abs(Stage.GetPanels()[2][0].GetScore()));
	String += to_string(abs(Stage.GetPanels()[2][1].GetScore()));
	String += to_string(abs(Stage.GetPanels()[2][2].GetScore()));
	String += '-';
	String += to_string(Stage.GetCntTurn());
	String += ".pat";
	return String;
}

battle_field::battle_field(stage &Stage)
	:AI(Stage)
{
	Loop = true;
}

battle_field::~battle_field()
{

}

void battle_field::Battle()
{
	AI.NumTurns(stage::MaxTurns);
	for (int i = 0; i < stage::MaxTurns && Loop; ++i)
	{
		Play();
	}
}

void battle_field::Play()
{
	stage Stage = AI.GetStage();
	action_id IntentionIDs[NumTeams][stage::NumAgents];
	int Record = AI.BestMove(IntentionIDs);
	AI.Move(IntentionIDs);

	std::string FileName = MakeFileName(Stage);
	WriteTrainingData(StageToTrainingData(Stage, IntentionIDs[Team_1P], Record > 0), FileName.c_str());
	std::cout << "File Write : " << FileName << std::endl;
}

<<<<<<< HEAD
void battle_field::Stop()
{
	Loop = false;
=======
		cout << "================Redo or Continue or End===================" << endl;
		do
		{
			cout << "0‚ÅI—¹,‚»‚Ì‘¼‚Å‘±s,r‚ÅŒ³‚É–ß‚·,v‚Å— •Ô‚·" << endl;
			cin >> isContinue;
			if (isContinue == 'v')
			{
				int x, y,state;
				cout << "x,y,state(neutral:-1 friendly:0 enemy:1):" << endl;
				cin >> x >> y >> state;
				Stage.SetState(x,y,state);
				friend_node *NewCurrentNode = new friend_node(nullptr, Stage, NumTurn);
				CurrentNode = NewCurrentNode;
			}
			if (isContinue == 'r' && t>0)
			{
				t--;
				Stage = Stages[t];
				CurrentNode = CurrentNodes[t];
				cout << "Stage:" << endl;
				Stage.PrintStage();
				cout << "Node:" << endl;
				CurrentNode->PrintStage();
			}
		} while (isContinue == 'r' && t>0);
		
	} while (isContinue != '0');
	delete CurrentNode;
	CurrentNode = nullptr;
>>>>>>> Prodution
}
