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

void node::play() //閾値以上ならノードを展開、閾値以下ならrollout、Q値を更新
{
	if (1)
	{
		
	}
}

void node::Selection() //子ノードのコスト関数とQ値に基づいて子ノードを選択する
{
	int Q_Cmax = -114514, selected = 0;
	for(int i = 0; i < Child.size(); i++)
	{
		if(Q_Cmax < Child[i]->Q + Child[i]->cost(N))
		{
			Q_Cmax = Child[i]->Q + Child[i]->cost(N);
			selected = i;
		}
	}
	Child[selected]->play();
}

int node::cost(int Ns) //このノードを選ぶのにかかるコストを返す。Alpha参照。
{
	return std::sqrt(2 * std::log(Ns)) / N;
}

void node::Expansion(team_no Team)
{
	for (action_id i = 0; i < 17; ++i)
	{
		intention Intention1 = i;
		if(!Stage.CanAction(Intention1, Team, 0))
		{
			continue;
		}
		for (action_id j = 0; j < 17; ++j)
		{
			intention Intention2 = j;
			if(!Stage.CanAction(Intention2, Team, 1))
			{
				continue;
			}

			node *NewNode = new node(this, Stage);
			if (Team == Team_1P)
			{
				NewNode->IntentionID1 = i;
				NewNode->IntentionID2 = j;
			}
			else
			{
				NewNode->IntentionID1 = None;
				NewNode->IntentionID2 = None;
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

int node::Rollout(stage Stage, int turn)//ランダムに手を最後まで打って勝敗を返す
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