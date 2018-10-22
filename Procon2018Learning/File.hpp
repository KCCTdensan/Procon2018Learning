#pragma once

#include "Game.hpp"


class image
{
	int Value[MaxY][MaxX];

public:
	int* operator[](int y)
	{
		return Value[y];
	}
	int& operator[](position p)
	{
		return Value[p.y][p.x];
	}
};

struct input
{
	image StageSize;
	image PanelPoints;
	image FriendAgent1;
	image FriendAgent2;
	image FriendPanels;
	image FriendRegion;
	image OpponentAgent1;
	image OpponentAgent2;
	image OpponentPanels;
	image OpponentRegion;
};

struct output_policy
{
	int EvaluationValue[ID_MaxID][ID_MaxID];
};

struct output_value
{
	int WinningRate;
};

struct training_data
{
	input Input;
	output_policy OutputPolicy;
	output_value OutputValue;
};

training_data StageToTrainingData(const stage &Stage, action_id(&BestIntentions)[stage::NumAgents], bool Win);

void WriteTrainingData(const training_data &Data, const char *FileName);