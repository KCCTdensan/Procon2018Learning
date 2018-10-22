#include "BattleField.hpp"
#include "File.hpp"
#include <string>


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
		Play(i);
	}
}

void battle_field::Play(int CntTurns)
{
	std::string FileName = "TrainingData/ProconAITrainingData-";
	const stage &Stage = AI.GetStage();
	FileName += Stage.GetNumX() + "x" + Stage.GetNumY();
	FileName += Stage.GetPanels()[0][0].GetScore();
	FileName += Stage.GetPanels()[0][Stage.GetNumX() - 1].GetScore();
	FileName += Stage.GetPanels()[Stage.GetNumY() - 1][0].GetScore();
	FileName += Stage.GetPanels()[Stage.GetNumY() - 1][Stage.GetNumX() - 1].GetScore();
	FileName += "-";
	FileName += CntTurns;
	FileName += ".pat";
	action_id IntentionIDs[NumTeams][stage::NumAgents] = {{-1, -1}, {-1, -1}};
	int Record = AI.BestMove(IntentionIDs);
	WriteTrainingData(StageToTrainingData(Stage, IntentionIDs[Team_1P], Record > 0), FileName.c_str());
	AI.Move(IntentionIDs);
}

void battle_field::Stop()
{
	Loop = false;
}
