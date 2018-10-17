#include "Game.hpp"
#include "Random.hpp"
#include <iostream>
#include <windows.h>


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

	if(CopyType == 0)
	{
		for(int y = 0; y < NumY; ++y)
		{
			for(int x = 0; x < NumX / 2; ++x)
			{
				Panels[y][x].Init(PanelPointRandom());
				int xInv = NumX - x - 1;
				int Score = Panels[y][x].GetScore();
				Panels[y][xInv].Init(Score);
			}
			if(NumX % 2 != 0)
			{
				Panels[y][NumX / 2].Init(PanelPointRandom());
			}
		}
		Agents[0][0].Init(AgentX, AgentY, Team_1P);
		Agents[0][1].Init(NumX - AgentX - 1, AgentY, Team_2P);
		Agents[1][0].Init(AgentX, NumY - AgentY - 1, Team_1P);
		Agents[1][1].Init(NumX - AgentX - 1, NumY - AgentY - 1, Team_2P);
	}
	else
	{
		for(int y = 0; y < NumY / 2; ++y)
		{
			for(int x = 0; x < NumX; ++x)
			{
				Panels[y][x].Init(PanelPointRandom());
				int yInv = NumY - y - 1;
				int Score = Panels[y][x].GetScore();
				Panels[yInv][x].Init(Score);
			}
		}
		if(NumY % 2 != 0)
		{
			for(int x = 0; x < NumX; ++x)
			{
				Panels[NumY / 2][x].Init(PanelPointRandom());
			}
		}
		Agents[0][0].Init(AgentX, AgentY, Team_1P);
		Agents[0][1].Init(AgentX, NumY - AgentY - 1, Team_1P);
		Agents[1][0].Init(NumX - AgentX - 1, AgentY, Team_2P);
		Agents[1][1].Init(NumX - AgentX - 1, NumY - AgentY - 1, Team_2P);
	}
#ifdef _DEBUG
	std::cout << "<===========================================>" << std::endl;
	PrintStage();
	std::cout << "<===========================================>" << std::endl;
#endif
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

bool stage::Move(intention_info(&Infos)[NumTeams][NumAgents], team_no Team, char AgentNo)
{
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
	InitRandomStage();
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
			position AgentPosition = Agents[t][a].GetPosition();
			position NextPosition = AgentPosition + Intention;
			if (Panels[NextPosition].GetState() == Neutral || Panels[NextPosition].GetState() == t)
			{
				Agents[t][a].Move(Intention);
				Panels[NextPosition].MakeCard(t);
			}
			else
			{
				Panels[NextPosition].RemoveCard();
			}
		}
	}
	UpdateScore();
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

void stage::CanAction(intention(&Intentions)[NumTeams][NumAgents], bool(&Result)[NumTeams][NumAgents])
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

void stage::CanAction(action_id(&IntentionIDs)[NumTeams][NumAgents], bool(&Result)[NumTeams][NumAgents])
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

bool stage::CanAction(intention(&Intentions)[NumTeams][NumAgents])
{
	bool Result[NumTeams][NumAgents];
	CanAction(Intentions, Result);
	return (Result[0][0] && Result[0][1]) && (Result[1][0] && Result[1][1]);
}

void stage::CanAction(action_id(&IntentionIDs)[NumTeams][NumAgents])
{
	intention Intentions[NumTeams][NumAgents];
	for (team_no t = 0; t < NumTeams; ++t)
	{
		for (char a = 0; a < NumAgents; ++a)
		{
			Intentions[t][a] = IntentionIDs[t][a];
		}
	}
	CanAction(Intentions);
}

void stage::Action(intention(&Intentions)[NumAgents], team_no Team)
{
	if(!CanAction(Intentions))
	{
		return;
	}
	for(char a = 0; a < NumAgents; ++a)
	{
		intention &Intention = Intentions[a];
		position AgentPosition = Agents[Team][a].GetPosition();
		position NextPosition = AgentPosition + Intention;
		if (Panels[NextPosition].GetState() == Neutral || Panels[NextPosition].GetState() == Team)
		{
			Agents[Team][a].Move(Intention);
			Panels[NextPosition].MakeCard(Team);
		}
		else
		{
			Panels[Agents[Team][a].GetPosition()].RemoveCard();
		}
	}
	UpdateScore();
}

void stage::Action(action_id(&IntentionIDs)[NumAgents], team_no Team)
{
	intention Intentions[NumTeams];
	for(char a = 0; a < NumAgents; ++a)
	{
		Intentions[a] = IntentionIDs[a];
	}
	Action(Intentions, Team);
}

bool stage::CanAction(intention(&Intentions)[NumAgents], team_no Team)
{
	if(!(CanActionOne(Agents[Team][0].GetPosition(), Intentions[0]) != -1 && CanActionOne(Agents[Team][1].GetPosition(), Intentions[1]) != -1))
	{
		return false;
	}
	return Agents[Team][0].GetPosition() + Intentions[0] != Agents[Team][1].GetPosition() + Intentions[1];
}

bool stage::CanAction(intention &Intention, team_no Team, char AgentNo)
{
	return CanActionOne(Agents[Team][AgentNo].GetPosition(), Intention) != -1;
}

char stage::CanActionOne(position Position, intention Intention)
{
	if(Intention.DeltaX == 0 && Intention.DeltaY == 0)
	{
		return 1;
	}
	Position += Intention;
	return (0 <= Position.x && Position.x < NumX) && (0 <= Position.y && Position.y < NumY) ? 0 : -1;
}

void stage::UpdateScore()
{
	UpdateRegionScore();
	UpdateTileScore();
}

int stage::GetNumX()
{
	return NumX;
}

int stage::GetNumY()
{
	return NumY;
}

int stage::GetCntTurn()
{
	return CntTurn;
}

int stage::GetScore1P()
{
	return TileScore1P + RegionScore1P;
}

int stage::GetScore2P()
{
	return TileScore2P + RegionScore2P;
}

agent* stage::GetAgent(team_no Team, int AgentNo)
{
	return &Agents[Team][AgentNo];
}

void stage::ChangeColor(color_id CharColor, color_id BackColor)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD attr = 0;
	if (CharColor & MASK_INTENSITY)
	{
		attr |= FOREGROUND_INTENSITY;
	}
	if (CharColor & MASK_RED)
	{
		attr |= FOREGROUND_RED;
	}
	if (CharColor & MASK_GREEN)
	{
		attr |= FOREGROUND_GREEN;
	}
	if (CharColor & MASK_BLUE)
	{
		attr |= FOREGROUND_BLUE;
	}
	if (BackColor & MASK_INTENSITY)
	{
		attr |= BACKGROUND_INTENSITY;
	}
	if (BackColor & MASK_RED)
	{
		attr |= BACKGROUND_RED;
	}
	if (BackColor & MASK_GREEN)
	{
		attr |= BACKGROUND_GREEN;
	}
	if (BackColor & MASK_BLUE)
	{
		attr |= BACKGROUND_BLUE;
	}
	SetConsoleTextAttribute(hConsole, attr);
}

void stage::PrintStage()
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
				break;

			case Team_1P:
				CharColor = COL_CYAN;
				break;

			case Team_2P:
				CharColor = COL_YELLOW;
				break;
			}
			if(AgentPositions[0][0] == n || AgentPositions[0][1] == n)
			{
				BackColor = COL_BLUE;
			}
			if(AgentPositions[1][0] == n || AgentPositions[1][1] == n)
			{
				BackColor = COL_RED;
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