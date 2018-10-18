#pragma once

#include "Intention.hpp"
#include "Panel.hpp"
#include "Agent.hpp"
#include "Position.hpp"


typedef unsigned char can_action_flag;
enum
{
	F_Decided = 0x01,
	F_CanAction = 0x02,
	F_Move = 0x04
};

typedef char color_id;
enum
{
	COL_BLACK,
	COL_DARKBLUE,
	COL_DARKGREEN,
	COL_DARKCYAN,
	COL_DARKRED,
	COL_DARKVIOLET,
	COL_DARKYELLOW,
	COL_GRAY,
	COL_LIGHTGRAY,
	COL_BLUE,
	COL_GREEN,
	COL_CYAN,
	COL_RED,
	COL_VIOLET,
	COL_YELLOW,
	COL_WHITE,
	MASK_INTENSITY = 0x08,
	MASK_RED = 0x04,
	MASK_GREEN = 0x02,
	MASK_BLUE = 0x01
};

const static unsigned char MaxX = 12;
const static unsigned char MaxY = 12;

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
	struct intention_info
	{
		intention Delta;
		position ExpectedPosition;
		position NextPosition;
		char CanAct;
	};
	panels Panels;
	agent Agents[NumTeams][NumAgents];
	unsigned char NumX;
	unsigned char NumY;
	unsigned char CntTurn;
	unsigned short TileScore1P;
	unsigned short TileScore2P;
	unsigned short RegionScore1P;
	unsigned short RegionScore2P;

	int PanelPointRandom();
	
	int UpdateRegionScore_Check(int x, int y, team_no Team, panel_check(&CheckedPanel)[NumTeams][MaxY][MaxX]);
	void UpdateRegionScore_Set(int x, int y, team_no Team, bool Surrounded, panel_check(&CheckedPanel)[NumTeams][MaxY][MaxX]);
	void UpdateRegionScore();
	void UpdateTileScore();

	bool Move(intention_info(&Infos)[NumTeams][NumAgents], team_no t, char a);

public:
	stage();
	~stage();

	void InitRandomStage();
	//両チーム用
	void Action(intention(&Intentions)[NumTeams][NumAgents]);
	void Action(action_id(&IntentionIDs)[NumTeams][NumAgents]);
	void CanAction(intention(&Intentions)[NumTeams][NumAgents], bool(&Result)[NumTeams][NumAgents]);
	void CanAction(action_id(&IntentionIDs)[NumTeams][NumAgents], bool(&Result)[NumTeams][NumAgents]);
	bool CanAction(intention(&Intentions)[NumTeams][NumAgents]);
	bool CanAction(action_id(&IntentionIDs)[NumTeams][NumAgents]);

	//1チーム用
	void Action(intention(&Intentions)[NumAgents], team_no Team);
	void Action(action_id(&IntentionIDs)[NumAgents], team_no Team);
	bool CanAction(intention(&Intentions)[NumAgents], team_no Team);
	bool CanAction(action_id(&IntentionIDs)[NumAgents], team_no Team);

	//1プレイヤー用
	bool CanAction(intention Intention, team_no Team, char AgentNo);
	bool CanAction(action_id IntentionID, team_no Team, char AgentNo);
	char CanActionOne(position Position, intention Intention);

	void UpdateScore();
	int GetNumX();
	int GetNumY();
	int GetCntTurn();
	int GetScore1P();
	int GetScore2P();
	agent* GetAgent(team_no Team, int AgentNo);
	void ChangeColor(color_id CharColor, color_id BackColor);
	void PrintStage();
};