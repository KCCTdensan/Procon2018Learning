#include "File.hpp"
#include <iostream>
#include <fstream>


training_data StageToTrainingData(const stage &Stage, action_id(&BestIntentions)[stage::NumAgents], bool Win)
{
	training_data TrainingData = {};
	const panels &Panels = Stage.GetPanels();
	for (unsigned char y = 0; y < Stage.GetNumY(); ++y)
	{
		for (unsigned char x = 0; x < Stage.GetNumX(); ++x)
		{
			TrainingData.Input.StageSize[y][x] = 1;
			TrainingData.Input.PanelPoints[y][x] = Panels[y][x].GetScore();
			TrainingData.Input.FriendPanels[y][x] = Panels[y][x].GetState() == Team_1P ? 1 : 0;
			TrainingData.Input.FriendRegion[y][x] = Panels[y][x].GetSurrounded(Team_1P) ? 1 : 0;
			TrainingData.Input.OpponentPanels[y][x] = Panels[y][x].GetState() == Team_2P ? 1 : 0;
			TrainingData.Input.OpponentRegion[y][x] = Panels[y][x].GetSurrounded(Team_2P) ? 1 : 0;
		}
	}
	TrainingData.Input.FriendAgent1[Stage.GetAgent(Team_1P, 0).GetPosition()] = 1;
	TrainingData.Input.FriendAgent2[Stage.GetAgent(Team_1P, 1).GetPosition()] = 1;
	TrainingData.Input.OpponentAgent1[Stage.GetAgent(Team_2P, 0).GetPosition()] = 1;
	TrainingData.Input.OpponentAgent2[Stage.GetAgent(Team_2P, 1).GetPosition()] = 1;

	for (action_id i = 0; i < ID_MaxID; ++i)
	{
		for (action_id j = 0; j < ID_MaxID; ++j)
		{
			TrainingData.OutputPolicy.EvaluationValue[i][j] = 0;
		}
	}
	TrainingData.OutputPolicy.EvaluationValue[BestIntentions[0]][BestIntentions[1]] = 1;

	TrainingData.OutputValue.WinningRate = Win ? 1 : 0;

	return TrainingData;
}

void WriteTrainingData(const training_data &Data, const char *FileName)
{
	using namespace std;
	ofstream File;
	File.open(FileName, ios::out | ios::binary | ios::trunc);
	if(!File)
	{
		cout << "File Open Error" << endl;
	}
	File.write((char*)&Data, sizeof(training_data));
	File.close();
}
