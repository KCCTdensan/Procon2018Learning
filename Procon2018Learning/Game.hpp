#pragma once

#include <random>
#include "Intention.hpp"
#include "Panel.hpp"
#include "Agent.hpp"
#include "Position.hpp"


typedef unsigned char can_action_flag;
enum
{
	F_Called = 0x01,
	F_Decided = 0x02,
	F_CanAction = 0x04,
	F_Move = 0x08,
	F_WantToMove = 0x10
};

class stage
{
	typedef char panel_check;
	enum
	{
		PC_Unchecked,
		PC_Checked,
		PC_Set
	};
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
	bool CanAction_Move(team_no Team, char AgentNo, intention(&Intentions)[NumTeams][NumAgents], can_action_flag(&Flags)[NumTeams][NumAgents]);

public:
	stage();
	~stage();

	void UpdateScore();
	void Action(intention(&Intentions)[NumTeams][NumAgents]);
	void CanAction(intention(&Intentions)[NumTeams][NumAgents], bool (&Result)[NumTeams][NumAgents]);
	bool CanAction(intention(&Intentions)[NumAgents]);
	bool CanAction(intention(&Intentions)[NumTeams][NumAgents]);
	char CanActionOne(position Position, intention Intention);
	int GetNumX();
	int GetNumY();
	int GetCntTurn();
	int GetScore1P();
	int GetScore2P();
	agent* GetAgent(team_no Team, int AgentNo);
	void PrintStage();
};