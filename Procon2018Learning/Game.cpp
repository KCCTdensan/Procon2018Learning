#include "Game.hpp"
#include <iostream>


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
	CntTurn = 0;
	TileScore1P = 0;
	TileScore2P = 0;
	RegionScore1P = 0;
	RegionScore2P = 0;

	int CopyType = rand() % 2;
	int AgentX = rand() % (NumX / 2);
	int AgentY = rand() % (NumY / 2);

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

bool stage::CanAction_Move(team_no Team, char AgentNo, intention(&Intentions)[NumTeams][NumAgents], can_action_flag(&Flags)[NumTeams][NumAgents])
{
	can_action_flag &Flag = Flags[Team][AgentNo];
	intention &Intention = Intentions[Team][AgentNo];
	position Position = Agents[Team][AgentNo].GetPosition();
	//すでに移動可能か決まっていれば終了
	if(Flag & F_Decided)
	{
		return (Flag & F_Move) != 0;
	}
	//すでに呼び出し済みなら終了
	if(Flag & F_Called)
	{
		return false;
	}
	switch(CanActionOne(Position, Intention))
	{
	case 0://座標外へのアクションの場合
		Flag |= F_Decided;
	case 1://留まる場合
		Flag |= F_CanAction;
		return false;
	}
	//移動の場合
	if(Intention.Action == IA_MoveAgent)
	{
		position NextPosition = Position + Intention;
		char x = NextPosition.x;
		char y = NextPosition.y;
		team_no State = Panels[y][x].GetState();
		team_no EnemyTeam = Team == Team_1P ? Team_2P : Team_1P;
		//移動先に敵タイルがあった場合
		if(State == EnemyTeam)
		{
			Flag |= F_Decided;
			return false;
		}
		char SelectAgentNo = -AgentNo + 1;
		for(char a = 0; a < NumAgents; ++a)
		{
			position NextSelectAgentPosition = Agents[EnemyTeam][a].GetPosition() + Intentions[EnemyTeam][a];
			//敵エージェントの移動先が被った場合
			/*if(NextSelectAgentPosition == NextPosition)
			{
				bool Ret = CanAction_Move(EnemyTeam, a, Intentions, Flags);
				//敵が移動できる場合
				if(Ret)
				{
					Flag |= F_CanAction | F_Move;
				}
			}*/
		}
		/*if(Agents[Team][SelectAgentNo].GetPosition() + Intentions[Team][SelectAgentNo] == NextPosition)
		{
			bool Ret = CanAction_Move(Team, SelectAgentNo, Intentions, Flags);
			Flag |= F_Decided;
			if(Ret)
			{
				Flag |= F_CanAction | F_Move;
				return true;
			}
			return false;
		}*/
	}
	//タイル除去の場合
	Intention.Action = IA_MoveAgent;
	position NextPosition = Position + Intention;
	char x = NextPosition.x;
	char y = NextPosition.y;

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

void stage::Action(intention(&Intentions)[NumTeams][NumAgents])
{
	for(team_no t = 0; t < NumTeams; ++t)
	{
		for(char a = 0; a < NumAgents; ++a)
		{

		}
	}
}

void stage::CanAction(intention(&Intentions)[NumTeams][NumAgents], bool(&Result)[NumTeams][NumAgents])
{
	can_action_flag Flags[NumTeams][NumAgents] = {};
	position NextPositions[NumTeams][NumAgents];
	for(team_no t = 0; t < NumTeams; ++t)
	{
		for(char a = 0; a < NumAgents; ++a)
		{
			NextPositions[t][a] = Agents[t][a].GetPosition() + Intentions[t][a];
		}
	}

	auto CanMove = [&, this](team_no AgentTeam, char AgentNo)
	{
		if(Intention.Action == IA_MoveAgent)
		{

			for(team_no t = 0; t < NumTeams; ++t)
			{
				for(char a = 0; a < NumAgents; ++a)
				{
					if(t == AgentTeam && a == AgentNo)
					{
						continue;
					}
					if(NextPositions[t][a] == NextPositions[AgentTeam][AgentNo])
					{
						CanMove(t, a);
					}
				}
			}
		}
		else
		{
			Intention.Action = IA_MoveAgent;
			position NextPosition = Position + Intention;

		}
	};
	for(team_no t = 0; t < NumTeams; ++t)
	{
		for(char a = 0; a < NumAgents; ++a)
		{
			CanAction_Move(t, a, Intentions, Flags);
			Result[t][a] = (Flags[t][a] & F_CanAction) != 0;
		}
	}
}

bool stage::CanAction(intention(&Intentions)[NumAgents])
{
	if(!(CanActionOne(Agents[Team_1P][0].GetPosition(), Intentions[0]) && CanActionOne(Agents[Team_1P][1].GetPosition(), Intentions[1])))
	{
		return false;
	}
	return Agents[Team_1P][0].GetPosition() + Intentions[0] != Agents[Team_1P][1].GetPosition() + Intentions[1];
}

bool stage::CanAction(intention(&Intentions)[NumTeams][NumAgents])
{
	bool Result[NumTeams][NumAgents];
	CanAction(Intentions, Result);
	return (Result[0][0] && Result[0][1]) && (Result[1][0] && Result[1][1]);
}

char stage::CanActionOne(position Position, intention Intention)
{
	if(Intention.DeltaX == 0 && Intention.DeltaY == 0)
	{
		return 1;
	}
	Intention.Action = IA_MoveAgent;
	Position += Intention;
	return (0 <= Position.x && Position.x < NumX) && (0 <= Position.y && Position.y < NumY) ? -1 : 0;
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

void stage::PrintStage()
{
	using namespace std;
	for(int y = 0; y < NumY; y++)
	{
		for(int x = 0; x < NumX; x++)cout << Panels[y][x].GetScore() << " ";
		cout << endl;
	}
}