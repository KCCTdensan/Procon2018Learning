#pragma once

#include <array>
#include <random>
#include "General.hpp"


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

enum panel_check
{
	PC_Unchecked,
	PC_Checked,
	PC_Set
};

class panel
{
	int Point;
	team_no State;
	bool Surrounded[NumTeams];

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
	position Position;

public:
	agent();
	~agent();

	void Init(int PositionX, int PositionY, team_no Team);
	void Move(int DeltaX, int DeltaY);
	position GetPosition();
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

	std::random_device RandomDev;

	int PanelPointRandom();
	void InitRandomStage();
	int UpdateRegionScore_Check(int x, int y, team_no Team, panel_check(&CheckedPanel)[NumTeams][MaxY][MaxX]);
	void UpdateRegionScore_Set(int x, int y, team_no Team, bool Surrounded, panel_check(&CheckedPanel)[NumTeams][MaxY][MaxX]);
	void UpdateRegionScore();
	void UpdateTileScore();

public:
	stage();
	~stage();

	void UpdateScore();
	void Action(intention Intentions[]);
	bool CanAction(intention Intentions[]);
};