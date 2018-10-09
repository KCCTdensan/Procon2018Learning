#include <iostream>
#include <random>
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

void node::Selection()
{

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

int node::Rollout(stage Stage, int turn)//ƒ‰ƒ“ƒ_ƒ€‚ÉŽè‚ðÅŒã‚Ü‚Å‘Å‚Á‚ÄŸ”s‚ð•Ô‚·
{
	for(int i = turn; i > 0; i--)
	{
		intention Intentions[4];
		do{
			for (int i = 0; i < 4; ++i)
			{
				Intentions[i].DeltaX = rand() % 3 - 1;
				Intentions[i].DeltaY = rand() % 3 - 1;
				if (rand() % 2)
				{
					Intentions[i].Action = IA_MoveAgent;
				}else
				{
					Intentions[i].Action = IA_RemovePanel;
				}
			}
		} while (!Stage.CanAction(Intentions));
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