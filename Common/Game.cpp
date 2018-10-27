#include "Game.hpp"
#include "Random.hpp"
#include "Console.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


int stage::PanelPointRandom()
{
	int abs = random::Mod(17);
	int Negative = random::Mod(10);
	return (Negative == 0) ? -abs : abs;
}

void stage::InitRandomStage()
{
	NumX = random::Mod(10) + 3;
	NumY = random::Mod(10) + 3;
	CntTurn = 0;
	TileScore1P = 0;
	TileScore2P = 0;
	RegionScore1P = 0;
	RegionScore2P = 0;

	int CopyType = random::Mod(2);
	int AgentX = random::Mod(NumX / 2);
	int AgentY = random::Mod(NumY / 2);

	if (CopyType == 0)
	{
		for (int y = 0; y < NumY; ++y)
		{
			for (int x = 0; x < NumX / 2; ++x)
			{
				Panels[y][x].Init(PanelPointRandom());
				int xInv = NumX - x - 1;
				int Score = Panels[y][x].GetScore();
				Panels[y][xInv].Init(Score);
			}
			if (NumX % 2 != 0)
			{
				Panels[y][NumX / 2].Init(PanelPointRandom());
			}
		}
		Agents[Team_1P][0].Init(AgentX, AgentY);
		Agents[Team_1P][1].Init(AgentX, NumY - AgentY - 1);
		Agents[Team_2P][0].Init(NumX - AgentX - 1, AgentY);
		Agents[Team_2P][1].Init(NumX - AgentX - 1, NumY - AgentY - 1);
	}
	else
	{
		for (int y = 0; y < NumY / 2; ++y)
		{
			for (int x = 0; x < NumX; ++x)
			{
				Panels[y][x].Init(PanelPointRandom());
				int yInv = NumY - y - 1;
				int Score = Panels[y][x].GetScore();
				Panels[yInv][x].Init(Score);
			}
		}
		if (NumY % 2 != 0)
		{
			for (int x = 0; x < NumX; ++x)
			{
				Panels[NumY / 2][x].Init(PanelPointRandom());
			}
		}
		Agents[Team_1P][0].Init(AgentX, AgentY);
		Agents[Team_1P][1].Init(NumX - AgentX - 1, AgentY);
		Agents[Team_2P][0].Init(AgentX, NumY - AgentY - 1);
		Agents[Team_2P][1].Init(NumX - AgentX - 1, NumY - AgentY - 1);
	}
}

void stage::BinaryStage()
{
	std::ifstream file("./StageInfo.bin", std::ios_base::in | std::ios_base::binary);
	std::string QRText(300,'\0');
	file.read(&QRText[0],300);
	std::cout << QRText << std::endl;
	TextStage(QRText);
	file.close();
}

void stage::TextStage(std::string text)
{
	CntTurn = 0;
	TileScore1P = 0;
	TileScore2P = 0;
	RegionScore1P = 0;
	RegionScore2P = 0;
	NumX = stoi(split(split(text, ':')[0], ' ')[1]);
	NumY = stoi(split(split(text, ':')[0], ' ')[0]);
	int Agentx1 = stoi(split(split(text, ':')[NumY + 1], ' ')[1]) - 1;
	int Agenty1 = stoi(split(split(text, ':')[NumY + 1], ' ')[0]) - 1;
	int Agentx2 = stoi(split(split(text, ':')[NumY + 2], ' ')[1]) - 1;
	int Agenty2 = stoi(split(split(text, ':')[NumY + 2], ' ')[0]) - 1;
	//std::cout << Agentx1 << "," << Agenty1 << std::endl;
	//std::cout << Agentx2 << "," << Agenty2 << std::endl;
	for (int y = 0; y < NumY; y++)
	{
		for (int x = 0; x < NumX; x++)
		{
			char PanelsScore = (char)stoi(split((split(text, ':')[y + 1]), ' ')[x]);
			Panels[y][x].Init(PanelsScore);
		}
	}
	Agents[0][0].Init(Agentx2,Agenty2);
	Agents[0][1].Init(Agenty1,Agentx1);
	if(Agentx1 == Agentx2)
	{
		Agents[1][0].Init(NumX - Agentx1,Agenty1);
		Agents[1][1].Init(NumX - Agentx2,Agenty2);
		Panels[Agenty1][NumX - Agentx1].setState(1);
		Panels[Agenty2][NumX - Agentx2].setState(1);

	}
	else if (Agenty1 == Agenty2)
	{
		Agents[1][0].Init(Agentx1, NumY - 1 - Agenty1);
		Agents[1][1].Init(Agentx2, NumY - 1 - Agenty2);
		Panels[NumY - 1 - Agenty1][Agentx1].setState(1);
		Panels[NumY - 1 - Agenty1][Agentx2].setState(1);

	}
	else 
	{
		Agents[1][0].Init(Agentx1,Agenty2);
		Agents[1][1].Init(Agentx2,Agenty1);
		Panels[Agenty2][Agentx1].setState(1);
		Panels[Agenty1][Agentx2].setState(1);

	}
	

	Panels[Agenty1][Agentx1].setState(0);
	Panels[Agenty2][Agentx2].setState(0);

}

std::vector<std::string> stage::split(std::string str,char sp)
{
	std::vector<std::string> T;
	std::stringstream ss(str);
	std::string buffer;
	while (std::getline(ss, buffer, sp))
	{
		T.push_back(buffer);
	}
	return T;
}

int stage::UpdateRegionScore_Check(int x, int y, team_no Team, panel_check(&CheckedPanel)[NumTeams][MaxY][MaxX])
{
	if(Panels[y][x].GetState() == Team)
	{
		return 1;
	}
	if(CheckedPanel[Team][y][x] != PC_Unchecked)
	{
		return -1;
	}
	CheckedPanel[Team][y][x] = PC_Checked;
	if((x == 0) || (x == NumX - 1) || (y == 0) || (y == NumY - 1))
	{
		UpdateRegionScore_Set(x, y, Team, false, CheckedPanel);
		return 0;
	}

	int l = UpdateRegionScore_Check(x - 1, y, Team, CheckedPanel);
	if(l == 0)
	{
		return 0;
	}
	int t = UpdateRegionScore_Check(x, y + 1, Team, CheckedPanel);
	if(t == 0)
	{
		return 0;
	}
	int r = UpdateRegionScore_Check(x + 1, y, Team, CheckedPanel);
	if(r == 0)
	{
		return 0;
	}
	int b = UpdateRegionScore_Check(x, y - 1, Team, CheckedPanel);
	if(b == 0)
	{
		return 0;
	}
	if((l == -1 && t == -1) && (r == -1 && b == -1))
	{
		return -1;
	}
	return 1;
}

void stage::UpdateRegionScore_Set(int x, int y, team_no Team, bool Surrounded, panel_check(&CheckedPanel)[NumTeams][MaxY][MaxX])
{
	if(Panels[y][x].GetState() == Team)
	{
		Panels[y][x].SetSurrounded(false, Team);
		return;
	}
	if(CheckedPanel[Team][y][x] == PC_Set)
	{
		return;
	}
	CheckedPanel[Team][y][x] = PC_Set;
	Panels[y][x].SetSurrounded(Surrounded, Team);
	if(x > 0)
	{
		UpdateRegionScore_Set(x - 1, y, Team, Surrounded, CheckedPanel);
	}
	if(y > 0)
	{
		UpdateRegionScore_Set(x, y - 1, Team, Surrounded, CheckedPanel);
	}
	if(x < NumX - 1)
	{
		UpdateRegionScore_Set(x + 1, y, Team, Surrounded, CheckedPanel);
	}
	if(y < NumY - 1)
	{
		UpdateRegionScore_Set(x, y + 1, Team, Surrounded, CheckedPanel);
	}
}

void stage::UpdateRegionScore()
{
	panel_check CheckedPanel[NumTeams][MaxY][MaxX] = {};
	RegionScore1P = 0;
	RegionScore2P = 0;
	for(team_no t = 0; t < NumTeams; ++t)
	{
		for(char y = 0; y < NumY; ++y)
		{
			for(char x = 0; x < NumX; ++x)
			{
				if(CheckedPanel[t][y][x] == 0)
				{
					int Ret = UpdateRegionScore_Check(x, y, t, CheckedPanel);
					if(Ret == 1)
					{
						UpdateRegionScore_Set(x, y, t, true, CheckedPanel);
					}
				}
			}
		}
	}
	for (char y = 0; y < NumY; ++y)
	{
		for (char x = 0; x < NumX; ++x)
		{
			if (Panels[y][x].GetSurrounded(Team_1P))
			{
				RegionScore1P += abs(Panels[y][x].GetScore());
			}
			if (Panels[y][x].GetSurrounded(Team_2P))
			{
				RegionScore2P += abs(Panels[y][x].GetScore());
			}
		}
	}
}

void stage::UpdateTileScore()
{
	TileScore1P = 0;
	TileScore2P = 0;
	for(int y = 0; y < NumY; ++y)
	{
		for(int x = 0; x < NumX; ++x)
		{
			int Score = Panels[y][x].GetScore();
			switch(Panels[y][x].GetState())
			{
			case Team_1P:
				TileScore1P += Score;
				break;

			case Team_2P:
				TileScore2P += Score;
				break;
			}
		}
	}
}

bool stage::Move(intention_info(&Infos)[NumTeams][NumAgents], team_no Team, char AgentNo)const
{
	static int NumCall = 0;
	//すでに移動不可とわかっている場合
	if (Infos[Team][AgentNo].CanAct == -1)
	{
		return false;
	}

	//座標外への移動の試行やとどまる手など、移動可能かどうかがすぐに確定する場合
	switch (CanActionOne(Agents[Team][AgentNo].GetPosition(), Infos[Team][AgentNo].Delta))
	{
	case -1:
		return false;

	case 1:
		return true;
	}

	//目標座標の重複や他エージェントの位置に移動しようとした場合などを判定
	for (team_no t = 0; t < NumTeams; ++t)
	{
		for (char a = 0; a < NumAgents; ++a)
		{
			//自分のエージェントとは比較しない
			if (t == Team && a == AgentNo)
			{
				continue;
			}

			//他エージェントと目標座標が重複していた場合
			if (Infos[Team][AgentNo].ExpectedPosition == Infos[t][a].ExpectedPosition)
			{
				Infos[Team][AgentNo].CanAct = -1;
				Infos[t][a].CanAct = -1;
				return false;
			}

			//他エージェントの位置と目標座標が重複していた場合
			if (Infos[Team][AgentNo].ExpectedPosition == Infos[t][a].NextPosition)
			{
				Infos[Team][AgentNo].CanAct = -1;
				return false;
			}

			//他エージェントの現在の位置が目標座標と重複していた場合
			if (Infos[Team][AgentNo].ExpectedPosition == Agents[t][a].GetPosition())
			{
				//自エージェントの現在の位置が目標座標と重複していた場合
				if (Infos[t][a].ExpectedPosition == Agents[Team][AgentNo].GetPosition())
				{
					Infos[Team][AgentNo].CanAct = -1;
					Infos[t][a].CanAct = -1;
					return false;
				}

				//他エージェントが移動できる場合
				if (Move(Infos, t, a))
				{
					NumCall--;
					Infos[Team][AgentNo].CanAct = 1;
					return true;
				}

				//他エージェントが移動できない場合
				Infos[Team][AgentNo].CanAct = -1;
				return false;
			}
		}
	}
	return true;
}

stage::stage()
{
	//InitRandomStage();
	BinaryStage();
}

stage::stage(stage_initialize_flag Flag)
{
	switch (Flag)
	{
	case STAGE_INIT_RANDOM:
		InitRandomStage();
		return;

	case STAGE_INIT_BINARYFILE:
		BinaryStage();
		return;
	}
}

stage::stage(const char *QRCodeString)
{
	TextStage(QRCodeString);
}

stage::stage(std::string QRText)
{
	TextStage(QRText);
}

stage::~stage()
{

}

void stage::Action(intention(&Intentions)[NumTeams][NumAgents])
{
	bool CanActionFlags[NumTeams][NumAgents];
	CanAction(Intentions, CanActionFlags);
	for(team_no t = 0; t < NumTeams; ++t)
	{
		for (char a = 0; a < NumAgents; ++a)
		{
			if (!CanActionFlags[t][a])
			{
				continue;
			}
			intention &Intention = Intentions[t][a];
			position NextPosition = Agents[t][a].GetPosition() + Intention;
			//移動する手
			if (Panels[NextPosition].GetState() == Neutral || Panels[NextPosition].GetState() == t)
			{
				Agents[t][a].Move(Intention);
				Panels[NextPosition].MakeCard(t);
			}
			//パネルを返す手
			else
			{
				Panels[NextPosition].RemoveCard();
				Panels[Agents[t][a].GetPosition()].MakeCard(t);
			}
		}
	}
	UpdateScore();
	CntTurn++;
}

void stage::Action(action_id(&IntentionIDs)[NumTeams][NumAgents])
{
	intention Intentions[NumTeams][NumAgents];
	for(team_no t = 0; t < NumTeams; ++t)
	{
		for(char a = 0; a < NumAgents; ++a)
		{
			Intentions[t][a] = IntentionIDs[t][a];
		}
	}
	Action(Intentions);
}

void stage::CanAction(intention(&Intentions)[NumTeams][NumAgents], bool(&Result)[NumTeams][NumAgents])const
{
	intention_info Infos[NumTeams][NumAgents];
	for (team_no t = 0; t < NumTeams; ++t)
	{
		for (char a = 0; a < NumAgents; ++a)
		{
			Infos[t][a].Delta = Intentions[t][a];
			Infos[t][a].ExpectedPosition = Agents[t][a].GetPosition() + Infos[t][a].Delta;
			team_no ExpectedPosState = Panels[Infos[t][a].ExpectedPosition].GetState();
			if (ExpectedPosState != t && ExpectedPosState != Neutral)
			{
				Infos[t][a].NextPosition = Agents[t][a].GetPosition();
			}
			else
			{
				Infos[t][a].NextPosition = Infos[t][a].ExpectedPosition;
			}
			Infos[t][a].CanAct = 0;
		}
	}
	for (team_no t = 0; t < NumTeams; ++t)
	{
		for (char a = 0; a < NumAgents; ++a)
		{
			Result[t][a] = Move(Infos, t, a);
		}
	}
}

void stage::CanAction(action_id(&IntentionIDs)[NumTeams][NumAgents], bool(&Result)[NumTeams][NumAgents])const
{
	intention Intentions[NumTeams][NumAgents];
	for (team_no t = 0; t < NumTeams; ++t)
	{
		for (char a = 0; a < NumAgents; ++a)
		{
			Intentions[t][a] = IntentionIDs[t][a];
		}
	}
	CanAction(Intentions, Result);
}

bool stage::CanAction(intention(&Intentions)[NumTeams][NumAgents])const
{
	bool Result[NumTeams][NumAgents];
	CanAction(Intentions, Result);
	return (Result[0][0] && Result[0][1]) && (Result[1][0] && Result[1][1]);
}

bool stage::CanAction(action_id(&IntentionIDs)[NumTeams][NumAgents])const
{
	intention Intentions[NumTeams][NumAgents];
	for (team_no t = 0; t < NumTeams; ++t)
	{
		for (char a = 0; a < NumAgents; ++a)
		{
			Intentions[t][a] = IntentionIDs[t][a];
		}
	}
	return CanAction(Intentions);
}

bool stage::CanAction(intention(&Intentions)[NumAgents], team_no Team)const
{
	if(CanActionOne(Agents[Team][0].GetPosition(), Intentions[0]) == -1 || CanActionOne(Agents[Team][1].GetPosition(), Intentions[1]) == -1)
	{
		return false;
	}
	if (Agents[Team][0].GetPosition() + Intentions[0] == Agents[Team][1].GetPosition() + Intentions[1])
	{
		return false;
	}
	if ((Agents[Team][0].GetPosition() + Intentions[0] == Agents[Team][1].GetPosition()) && (Agents[Team][1].GetPosition() + Intentions[1] == Agents[Team][0].GetPosition()))
	{
		return false;
	}
	return true;
}

bool stage::CanAction(action_id(&IntentionIDs)[NumAgents], team_no Team)const
{
	intention Intentions[NumAgents];
	for (char a = 0; a < NumAgents; ++a)
	{
		Intentions[a] = IntentionIDs[a];
	}
	return CanAction(Intentions, Team);
}

bool stage::CanAction(intention Intention, team_no Team, char AgentNo)const
{
	return CanActionOne(Agents[Team][AgentNo].GetPosition(), Intention) != -1;
}

bool stage::CanAction(action_id IntentionID, team_no Team, char AgentNo)const
{
	return CanAction((intention)IntentionID, Team, AgentNo);
}

char stage::CanActionOne(position Position, intention Intention)const
{
	if(Intention.DeltaX == 0 && Intention.DeltaY == 0)
	{
		return 1;
	}
	Position += Intention;
	return ((0 <= Position.x && Position.x < NumX) && (0 <= Position.y && Position.y < NumY)) ? 0 : -1;
}

void stage::UpdateScore()
{
	UpdateRegionScore();
	UpdateTileScore();
}

unsigned char stage::GetNumX()const
{
	return NumX;
}

unsigned char stage::GetNumY()const
{
	return NumY;
}

unsigned char stage::GetCntTurn()const
{
	return CntTurn;
}

short stage::GetScore1P()const
{
	return TileScore1P + RegionScore1P;
}

short stage::GetScore2P()const
{
	return TileScore2P + RegionScore2P;
}
/*
agent& stage::GetAgent(team_no Team, char AgentNo)
{
	return Agents[Team][AgentNo];
}
*/
const agent& stage::GetAgent(team_no Team, char AgentNo)const
{
	return Agents[Team][AgentNo];
}

const panels& stage::GetPanels()const
{
	return Panels;
}

void stage::PrintStage()const
{
	using namespace std;

	position AgentPositions[NumTeams][NumAgents];
	for (team_no t = 0; t < NumTeams; ++t)
	{
		for (char a = 0; a < NumAgents; ++a)
		{
			AgentPositions[t][a] = Agents[t][a].GetPosition();
		}
	}
	for(char y = 0; y < NumY; y++)
	{
		for(char x = 0; x < NumX; x++)
		{
			position n = {x,y};
			color_id CharColor = 0;
			color_id BackColor = 0;
			switch(Panels[y][x].GetState())
			{
			case Neutral:
				if(Panels[y][x].GetSurrounded(Team_1P))
				{
					CharColor |= MASK_BLUE;
				}
				if(Panels[y][x].GetSurrounded(Team_2P))
				{
					CharColor |= MASK_RED;
				}
				if(CharColor == 0)
				{
					CharColor = COL_WHITE;
				}
				CharColor |= MASK_INTENSITY;
				break;

			case Team_1P:
				CharColor = COL_CYAN;
				break;

			case Team_2P:
				CharColor = COL_YELLOW;
				break;
			}
			if(AgentPositions[0][0] == n)
			{
				BackColor = COL_BLUE;
			}
			if(AgentPositions[0][1] == n)
			{
				BackColor = COL_DARKBLUE;
			}
			if(AgentPositions[1][0] == n)
			{
				BackColor = COL_RED;
			}
			if (AgentPositions[1][1] == n)
			{
				BackColor = COL_DARKRED;
			}
			ChangeColor(CharColor, BackColor);
			std::printf("%3d ", Panels[y][x].GetScore());
		}
		ChangeColor(COL_WHITE, COL_BLACK);
		cout << endl;
	}
	cout << endl;
	ChangeColor(COL_WHITE, COL_BLACK);
}

void stage::SetState(int x, int y, int state)
{
	Panels[x][y].setState((team_no)state);
}