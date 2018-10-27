#include "Panel.hpp"


panel::panel()
{

}

panel::~panel()
{

}

void panel::Init(char Point)
{
	this->Point = Point;
	State = Neutral;
	for (team_no t = 0; t < NumTeams; ++t)
	{
		Surrounded[t] = false;
	}
}

void panel::MakeCard(team_no Team)
{
	State = Team;
}

void panel::RemoveCard()
{
	State = Neutral;
}

char panel::GetScore()const
{
	return Point;
}

void panel::setState(team_no s)
{
	State = s;
}

team_no panel::GetState()const
{
	return State;
}

void panel::SetSurrounded(bool IsSurrounded, team_no Team)
{
	Surrounded[Team] = IsSurrounded;
}

bool panel::GetSurrounded(team_no Team)const
{
	return Surrounded[Team];
}