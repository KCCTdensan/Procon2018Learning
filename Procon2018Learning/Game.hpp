#pragma once

#include "Intention.hpp"
#include "Panel.hpp"
#include "Agent.hpp"
#include "Position.hpp"


typedef unsigned char can_action_flag;
enum
{
	F_Decided = 0x01,
	F_CanAction = 0x02
};

const static int MaxX = 12;
const static int MaxY = 12;

class panels
{
	panel Panels[MaxY][MaxX];

public:
	panel* operator[](char y)
	{
		return Panels[y];
	}
	panel& operator[](position p)
	{
		return Panels[p.y][p.x];
	}
};

class stage
{
public:
	const static int MaxTurn = 60;
	const static int NumAgents = 2;

private:
	typedef char panel_check;
	enum
	{
		PC_Unchecked,
		PC_Checked,
		PC_Set
	};
	panels Panels;
	agent Agents[NumTeams][NumAgents];
	int NumX;
	int NumY;
	int CntTurn;
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

	//両チーム用
	void Action(intention(&Intentions)[NumTeams][NumAgents]);
	void Action(action_id(&IntentionIDs)[NumTeams][NumAgents]);
	void CanAction(intention(&Intentions)[NumTeams][NumAgents], bool (&Result)[NumTeams][NumAgents]);
	bool CanAction(intention(&Intentions)[NumTeams][NumAgents]);

	//1チーム用
	void Action(intention(&Intentions)[NumAgents], team_no Team);
	bool CanAction(intention(&Intentions)[NumAgents]);

	//1プレイヤー用
	bool CanAction(intention &Intention, team_no Team, char AgentNo);
	char CanActionOne(position Position, intention Intention);

	void UpdateScore();
	int GetNumX();
	int GetNumY();
	int GetCntTurn();
	int GetScore1P();
	int GetScore2P();
	agent* GetAgent(team_no Team, int AgentNo);
	void color(int, int, int, int);
	void PrintStage();
};