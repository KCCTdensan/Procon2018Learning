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
	int Point;
	team_no State;
	bool Surrounded[NumTeams];

public:
	panel();
	~panel();

	void Init(int Point);
	void MakeCard(team_no Team);
	void RemoveCard();
	int GetScore();
	team_no GetState();
	void SetSurrounded(bool IsSurrounded, team_no Team);
	bool GetSurrounded(team_no Team);
};