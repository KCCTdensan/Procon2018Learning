#include <iostream>
#include <random>
#include<cmath>
#include "Search.hpp"


node::node(node *Parent, stage &Stage)
{
	this->Parent = Parent;
	this->Stage = Stage;
}

node::~node()
{
	int NumChild = (int)Child.size();
	for(int i = 0; i < NumChild; ++i)
	{
		delete Child[i];
		Child[i] = nullptr;
	}
	Child.clear();
}

void node::play() //臒l�ȏ�Ȃ�m�[�h��W�J�A臒l�ȉ��Ȃ�rollout�AQ�l���X�V
{
	if ()
	{
		
	}
}

void node::Selection() //�q�m�[�h�̃R�X�g�֐���Q�l�Ɋ�Â��Ďq�m�[�h��I������
{
	int Q_Cmax = -114514,selected = 0;
	for (int i = 0; i < Child.size(); i++)
	{
		if (Q_Cmax < *Child[i]->Q + *Child[i]->cost(N))
		{
			Q_Cmax = *Child[i]->Q + *Child[i]->cost(N);
			selected = i;
		}
	}
	*Child[selected]->play();
}

int node::cost(int Ns) //���̃m�[�h��I�Ԃ̂ɂ�����R�X�g��Ԃ��BAlpha�Q�ƁB
{
	return std::sqrt(2 * std::log(Ns)) / N;
}

void node::Expansion(team_no Team)
{
	position Agent1Pos = Stage.GetAgent(Team, 0)->GetPosition();
	position Agent2Pos = Stage.GetAgent(Team, 0)->GetPosition();

	for (int i = 0; i < 17; ++i)
	{
		if (!CanMove((action_id)i, Agent1Pos))
		{
			continue;
		}
		for (int j = 0; j < 17; ++j)
		{
			if (!CanMove((action_id)j, Agent2Pos))
			{
				continue;
			}

			node *NewNode = new node(this, Stage);
			if (Team == Team_1P)
			{
				NewNode->Intention1 = (action_id)i;
				NewNode->Intention2 = (action_id)j;
			}
			else
			{
				NewNode->Intention1 = None;
				NewNode->Intention2 = None;
			}
			NewNode->Team = (Team == Team_1P) ? Team_2P : Team_1P;
			Child.push_back(NewNode);
		}
	}
}

void node::Evaluation()
{

}

bool node::IsLeafNode()
{
	return Child.size() == 0;
}

int node::Rollout(stage Stage, int turn)//�����_���Ɏ���Ō�܂őł��ď��s��Ԃ�
{
	for(int i = turn; i > 0; i--)
	{
		intention Intentions[2][2];
		bool results[2][2];
		do{
			for (int i = 0; i < 4; ++i)
			{
				Intentions[i / 2][i % 2].DeltaX = rand() % 3 - 1;
				Intentions[i / 2][i % 2].DeltaY = rand() % 3 - 1;
				if (rand() % 2)
				{
					Intentions[i / 2][i % 2].Action = IA_MoveAgent;
				}else
				{
					Intentions[i / 2][i % 2].Action = IA_RemovePanel;
				}
			}
			Stage.CanAction(Intentions, results);
		} while (!(results[0][0]&&results[0][1]&&results[1][0]&&results[1][1]));
		Stage.Action(Intentions);
	}

	if (Stage.GetScore1P() > Stage.GetScore2P())
	{
		return 1;
	}
	else if (Stage.GetScore1P() == Stage.GetScore2P())
	{
		return 0;
	}
	else
	{
		return - 1;
	}
	
}