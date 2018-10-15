#pragma once


typedef char team_no;
enum
{
	Neutral = -1,
	Team_1P,
	Team_2P,
	NumTeams
};

class panel
{
	char Point;
	team_no State;
	bool Surrounded[NumTeams];

public:
	panel();
	~panel();

	void Init(char Point);
	void MakeCard(team_no Team);
	void RemoveCard();
	char GetScore();
	team_no GetState();
	void SetSurrounded(bool IsSurrounded, team_no Team);
	bool GetSurrounded(team_no Team);
};