#include "Search.hpp"


int Selection()
{
	
}
int Evalution()
{

}
void Backup()
{

}

bool node::CanMove(action_id No, position Position)
{
	switch (No)
	{
	case Stay:
		return true;

	case Move_TopLeft:
	case Remove_TopLeft:
		return Position.x > 0 && Position.y > 0;

	case Move_Top:
	case Remove_Top:
		return Position.y > 0;

	case Move_TopRight:
	case Remove_TopRight:
		return Position.x < Stage.GetNumX() - 1 && Position.y > 0;

	case Move_Left:
	case Remove_Left:
		return Position.x > 0;

	case Move_Right:
	case Remove_Right:
		return Position.x < Stage.GetNumX() - 1;

	case Move_BottomLeft:
	case Remove_BottomLeft:
		return Position.x > 0 && Position.y < Stage.GetNumY() - 1;

	case Move_Bottom:
	case Remove_Bottom:
		return Position.y < Stage.GetNumY() - 1;

	case Move_BottomRight:
	case Remove_BottomRight:
		return Position.x < Stage.GetNumX() - 1 && Position.y < Stage.GetNumY() - 1;

	default:
		return false;
	}
}

node::node(node *Parent, stage &Stage)
{
	this->Parent = Parent;
	this->Stage = Stage;
}

node::~node()
{
	int NumChild = (int)Child.size();
	for (int i = 0; i < NumChild; ++i)
	{
		delete Child[i];
		Child[i] = nullptr;
	}
	Child.clear();
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