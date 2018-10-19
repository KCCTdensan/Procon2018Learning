#pragma once

#include "Game.hpp"


class image
{
	float Value[MaxY][MaxX];

public:
	float* operator[](int y)
	{
		return Value[y];
	}
	float& operator[](position p)
	{
		return Value[p.y][p.x];
	}
};

struct input
{
	image StageSize;
	image PanelPoints;
	image FriendAgents;
	image FriendPanels;
	image FriendRegion;
	image OpponentAgents;
	image OpponentPanels;
	image OpponentRegion;
};

struct output_policy
{
	float EvaluationValue[ID_MaxID][ID_MaxID];
};

struct output_value
{
	float WinningRate;
};

struct training_data
{
	input Input;
	output_policy OutputPolicy;
	output_value OutputValue;
};

void WriteTrainingData(training_data &Data, const char *FileName);