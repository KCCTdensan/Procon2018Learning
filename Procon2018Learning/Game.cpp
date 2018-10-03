#include "Game.hpp"


panel::panel()
{

}

panel::~panel()
{

}

void panel::Init(int Point)
{
	this->Point = Point;
	State = None;
}

void panel::MakeCard(team_no Team)
{
	State = Team;
}

void panel::RemoveCard()
{
	State = None;
}

int panel::GetScore()
{
	return Point;
}

team_no panel::GetState()
{
	return State;
}

void panel::SetSurrounded(bool IsSurrounded, team_no Team)
{
	Surrounded[Team] = IsSurrounded;
}

bool panel::GetSurrounded(team_no Team)
{
	return Surrounded[Team];
}


agent::agent()
{

}

agent::~agent()
{

}

void agent::Init(int PositionX, int PositionY, team_no Team)
{
	Position.x = PositionX;
	Position.y = PositionY;
	this->Team = Team;
}

void agent::Move(int DeltaX, int DeltaY)
{
	Position.x += DeltaX;
	Position.y += DeltaY;
}

position agent::GetPosition()
{
	return Position;
}


int stage::PanelPointRandom()
{
	int abs = rand() % 17;
	int Negative = rand() % 10;
	return (Negative == 0) ? -abs : abs;
}

void stage::InitRandomStage()
{
	NumX = rand() % 10 + 3;
	NumY = rand() % 10 + 3;
	TileScore1P = 0;
	TileScore2P = 0;
	RegionScore1P = 0;
	RegionScore2P = 0;

	for(int y = 0; y < NumY / 2; ++y)
	{
		for(int x = 0; x < NumX / 2; ++x)
		{
			Panels[y][x].Init(PanelPointRandom());
		}
	}

	int CopyType = rand() % 2;
	int AgentX = rand() % (NumX / 2);
	int AgentY = rand() % (NumY / 2);

	if(CopyType == 0)
	{
		for(int y = 0; y < NumY; ++y)
		{
			for(int x = 0; x < NumX / 2; ++x)
			{
				int xInv = NumX - x - 1;
				int Score = Panels[y][x].GetScore();
				Panels[y][xInv].Init(Score);
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
				int yInv = NumY - y - 1;
				int Score = Panels[y][x].GetScore();
				Panels[yInv][x].Init(Score);
			}
		}
		Agents[0][0].Init(AgentX, AgentY, Team_1P);
		Agents[0][1].Init(AgentX, NumY - AgentY - 1, Team_1P);
		Agents[1][0].Init(NumX - AgentX - 1, AgentY, Team_2P);
		Agents[1][1].Init(NumX - AgentX - 1, NumY - AgentY - 1, Team_2P);
	}
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
	for(int t = 0; t < NumTeams; ++t)
	{
		for(int y = 0; y < NumY; ++y)
		{
			for(int x = 0; x < NumX; ++x)
			{
				if(CheckedPanel[t][y][x] == 0)
				{
					int Ret = UpdateRegionScore_Check(x, y, (team_no)t, CheckedPanel);
					if(Ret == 1)
					{
						UpdateRegionScore_Set(x, y, (team_no)t, true, CheckedPanel);
					}
				}
			}
		}
	}
}

void stage::UpdateTileScore()
{
	RegionScore1P = 0;
	RegionScore2P = 0;
	for(int y = 0; y < NumY; ++y)
	{
		for(int x = 0; x < NumX; ++x)
		{
			int Score = Panels[y][x].GetScore();
			switch(Panels[y][x].GetState())
			{
			case Team_1P:
				RegionScore1P += Score;
				break;

			case Team_2P:
				RegionScore2P += Score;
				break;
			}
		}
	}
}

stage::stage()
{
	InitRandomStage();
}

stage::~stage()
{

}

void stage::UpdateScore()
{
	UpdateRegionScore();
	UpdateTileScore();
}

void stage::Action(intention Intentions[])
{

}

bool stage::CanAction(intention Intentions[])
{
	return false;
}
