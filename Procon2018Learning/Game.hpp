#pragma once

#include<array>
#include <random>


enum team_no
{
	None = -1,
	Team_1P,
	Team_2P,
	NumTeams
};

enum intention_action
{
	IA_MoveAgent,
	IA_RemovePanel
};

class panel
{
	int Point;

public:
	panel();
	~panel();

	void Init(int Point);
	void MakeCard(team_no Team);
	void RemoveCard();
	int GetScore();
	team_no GetState();
	void SetSurrounded(bool IsSurrounded, team_no Team);
	bool GetSurrounded(team_no Team);
};

class agent
{
	team_no Team;
	int PositionX;
	int PositionY;

public:
	agent();
	~agent();

	void Init(int PositionX, int PositionY, team_no Team);
	void Move(int DeltaX, int DeltaY);
};

struct intention
{
	int DeltaX;
	int DeltaY;
	intention_action Action;
};

class stage
{
	const static int MaxTurn = 60;
	const static int MaxX = 12;
	const static int MaxY = 12;
	const static int NumAgents = 2;
	int NumX;
	int NumY;
	agent Agents[NumTeams][NumAgents];
	panel Panels[MaxY][MaxX];
	int TileScore1P;
	int TileScore2P;
	int RegionScore1P;
	int RegionScore2P;

	std::random_device rand;

	int PanelPointRandom();

public:
	stage();
	~stage();

	void Action(intention Intentions[]);
	bool CanAction(intention Intentions[]);
};