#include "Search.hpp"
#include "Random.hpp"
#include <iostream>

int node::Play() //臒l�ȏ�Ȃ�m�[�h��W�J�A臒l�����Ȃ�rollout�AQ�l���X�V
{
	if(N >= Threshold)
	{
		if(IsLeafNode())
		{
			Expansion();
		}
		return Selection();
	}
	return Evaluation();
}

int node::Selection() //�q�m�[�h�̃R�X�g�֐���Q�l�Ɋ�Â��Ďq�m�[�h��I������
{
	float Q_CMax = -1.0;
	int Selected_i = -1, Selected_j = -1;
	for(action_id i = 0; i < Max_ActionID; ++i)
	{
		for(action_id j = 0; j < Max_ActionID; ++j)
		{
			if(Child[i][j] == nullptr)
			{
				continue;
			}
			float Q_C = Child[i][j]->Q + Child[i][j]->Cost(N);
			if(Q_CMax < Q_C)
			{
				Q_CMax = Q_C;
				Selected_i = i;
				Selected_j = j;
			}
		}
	}
	if(Selected_i == -1)
	{
		return 0;
	}
	return Child[Selected_i][Selected_j]->Play();
}

void node::Expansion()
{
	intention Intentions[stage::NumAgents];

	for(action_id i = 0; i < 17; ++i)
	{
		Intentions[0] = i;
		if(!Stage.CanAction(Intentions[0], Team, 0))
		{
			continue;
		}
		for(action_id j = 0; j < 17; ++j)
		{
			Intentions[1] = j;
			if(!Stage.CanAction(Intentions[1], Team, 1))
			{
				continue;
			}

			node *NewNode = new node(this, Stage, (Team == Team_1P) ? Team_2P : Team_1P);
			if(Team == Team_1P)
			{
				NewNode->IntentionID1 = i;
				NewNode->IntentionID2 = j;
				NewNode->Stage.Action(Intentions, Team);
			}
			else
			{
				NewNode->IntentionID1 = None;
				NewNode->IntentionID2 = None;
				NewNode->Stage.Action(Intentions, Team);
			}
			Child[i][j] = NewNode;
			NumChildren++;
		}
	}
}

int node::Evaluation()
{
	int Result = Rollout(Stage, stage::MaxTurn - Stage.GetCntTurn());
	/*switch(Result)
	{
	case -1:
		NumLose++;
		break;

	case 1:
		NumWin++;
		break;
	}*/
	N++;
	Record += Result;
	Q = (float)Record / N;
	return Result;
}

int node::Rollout(stage &Stage, int NumTurn)//�����_���Ɏ���Ō�܂őł��ď��s��Ԃ�
{
	if(Team == Team_1P)
	{
		intention Intentions[NumTeams];
		Stage.Action(Intentions, Team_2P);
	}
	for(int i = 0; i < NumTurn; ++i)
	{
		intention Intentions[NumTeams][stage::NumAgents];
		do
		{
			for(int j = 0; j < 4; ++j)
			{
				Intentions[i / 2][i % 2] = (action_id)rand() % Max_ActionID;
			}
		} while(!Stage.CanAction(Intentions));
		Stage.Action(Intentions);
	}

	if(Stage.GetScore1P() > Stage.GetScore2P())
	{
		return 1;
	}
	if(Stage.GetScore1P() == Stage.GetScore2P())
	{
		return 0;
	}
	return -1;
}

float node::Cost(int Ns) //���̃m�[�h��I�Ԃ̂ɂ�����R�X�g��Ԃ��BAlpha�Q�ƁB
{
	return std::sqrtf(2.0f * std::logf((float)Ns)) / (float)N;
}

bool node::IsLeafNode()
{
	return NumChildren == 0;
}

node::node(node *Parent, stage &Stage, team_no Team)
{
	this->Parent = Parent;
	NumChildren = 0;
	for(action_id i = 0; i < Max_ActionID; ++i)
	{
		for(action_id j = 0; j < Max_ActionID; ++j)
		{
			Child[i][j] = nullptr;
		}
	}
	this->Stage = Stage;
	Q = 0.0;
	N = 0;
	Record = 0;
	this->Team = Team;
}

node::~node()
{
	for(action_id i = 0; i < Max_ActionID; ++i)
	{
		for(action_id j = 0; j < Max_ActionID; ++j)
		{
			if(Child[i][j] != nullptr)
			{
				delete Child[i][j];
				Child[i][j] = nullptr;
			}
		}
	}
}

void node::Search(int NumCallPlay, int(&Result)[Max_ActionID][Max_ActionID])
{
	if(IsLeafNode())
	{
		Expansion();
	}
	for(int i = 0; i < NumCallPlay; ++i)
	{
		Selection();
	}
	for(action_id i = 0; i < Max_ActionID; ++i)
	{
		for(action_id j = 0; j < Max_ActionID; ++j)
		{
			if(Child[i][j] == nullptr)
			{
				Result[i][j] = 0;
				continue;
			}
			Result[i][j] = Child[i][j]->N;
		}
	}
}

node* node::Deepen(action_id Action1, action_id Action2)
{
	for(action_id i = 0; i < Max_ActionID; ++i)
	{
		for(action_id j = 0; j < Max_ActionID; ++j)
		{
			if(i != Action1 || j != Action2)
			{
				delete Child[i][j];
				Child[i][j] = nullptr;
			}
		}
	}
	Child[Action1][Action2]->Parent = nullptr;
	return Child[Action1][Action2];
}
