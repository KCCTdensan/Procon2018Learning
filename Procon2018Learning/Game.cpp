#include "Game.hpp"


Panel::Panel()
{

}

Panel::~Panel()
{

}

void Panel::Init(int Point)
{
	this->Point = Point;
}

void Panel::MakeCard(TeamNo Team)
{

}

void Panel::RemoveCard()
{

}

int Panel::GetScore()
{
	return 0;
}

TeamNo Panel::GetState()
{
	return TeamNo();
}

void Panel::SetSurrounded(bool IsSurrounded, TeamNo Team)
{

}

bool Panel::GetSurrounded(TeamNo Team)
{
	return false;
}


Agent::Agent()
{

}

Agent::~Agent()
{

}

void Agent::Init(int PositionX, int PositionY, TeamNo Team)
{
	this->PositionX = PositionX;
	this->PositionY = PositionY;
	this->Team = Team;
}

void Agent::Move(int DeltaX, int DeltaY)
{
	PositionX += DeltaX;
	PositionY += DeltaY;
}


int Stage::PanelPointRandom()
{
	int abs = rand() % 17;
	int Negative = rand() % 10;
	return (Negative == 0) ? -abs : abs;
}

Stage::Stage()
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

Stage::~Stage()
{

}

void Stage::Action(Intention Intentions[])
{

}

bool Stage::CanAction(Intention Intentions[])
{
	return false;
}
