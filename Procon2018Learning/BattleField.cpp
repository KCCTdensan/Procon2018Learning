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

void battle_field::Stop()
{
	Loop = false;
}