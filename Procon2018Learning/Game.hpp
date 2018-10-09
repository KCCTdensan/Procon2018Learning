#pragma once

#include <random>
#include "General.hpp"


enum team_no
{
	Neutral = -1,
	Team_1P,
	Team_2P,
	NumTeams
};

enum intention_action
{
	IA_MoveAgent,
	IA_RemovePanel
};

enum action_id
{
	None = -1,
	Stay,
	Move_TopLeft,
	Move_Top,
	Move_TopRight,
	Move_Left,
	Move_Right,
	Move_BottomLeft,
	Move_Bottom,
	Move_BottomRight,
	Remove_TopLeft,
	Remove_Top,
	Remove_TopRight,
	Remove_Left,
	Remove_Right,
	Remove_BottomLeft,
	Remove_Bottom,
	Remove_BottomRight
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
	static std::random_device RandomDev;
	int NumX;
	int NumY;
	int CntTurn;
	agent Agents[NumTeams][NumAgents];
	panel Panels[MaxY][MaxX];
	int TileScore1P;
	int TileScore2P;
	int RegionScore1P;
	int RegionScore2P;

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
	int GetNumX();
	int GetNumY();
	int GetCntTurn();
	int GetScore1P();
	int GetScore2P();
	agent* GetAgent(team_no Team, int AgentNo);
	void PrintStage();
};