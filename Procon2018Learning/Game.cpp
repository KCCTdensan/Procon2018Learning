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
}

void panel::MakeCard(team_no Team)
{

}

void panel::RemoveCard()
{

}

int panel::GetScore()
{
	return 0;
}

team_no panel::GetState()
{
	return team_no();
}

void panel::SetSurrounded(bool IsSurrounded, team_no Team)
{

}

bool panel::GetSurrounded(team_no Team)
{
	return false;
}


agent::agent()
{

}

agent::~agent()
{

}

void agent::Init(int PositionX, int PositionY, team_no Team)
{
	this->PositionX = PositionX;
	this->PositionY = PositionY;
	this->Team = Team;
}

void agent::Move(int DeltaX, int DeltaY)
{
	PositionX += DeltaX;
	PositionY += DeltaY;
}


int stage::PanelPointRandom()
{
	int abs = rand() % 17;
	int Negative = rand() % 10;
	return (Negative == 0) ? -abs : abs;
}

stage::stage()
{
	NumX = rand() % 10 + 3;
	NumY = rand() % 10 + 3;
	TileScore1P = 0;
	TileScore2P = 0;
	RegionScore1P = 0;
	RegionScore2P = 0;

	for (int y = 0; y < NumY / 2; ++y)
	{
		for (int x = 0; x < NumX / 2; ++x)
		{
			Panels[y][x].Init(PanelPointRandom());
		}
	}

	int CopyType = rand() % 2;
	int AgentX = rand() % (NumX / 2);
	int AgentY = rand() % (NumY / 2);

	if (CopyType == 0)
	{
		for (int y = 0; y < NumY; ++y)
		{
			for (int x = 0; x < NumX / 2; ++x)
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
		for (int y = 0; y < NumY / 2; ++y)
		{
			for (int x = 0; x < NumX; ++x)
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

stage::~stage()
{

}

void stage::Action(intention Intentions[])
{

}

bool stage::CanAction(intention Intentions[])
{
	return false;
}
