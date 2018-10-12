#include "BattleField.hpp"


battle_field::battle_field()
	:CurrentNode(nullptr, Stage, Neutral)
{

}

battle_field::~battle_field()
{

}

void battle_field::Battle(int NumTurn)
{
	for (int i = 0; i < NumTurn; ++i)
	{
		int Result[Max_ActionID][Max_ActionID];
		CurrentNode.Search(node::NumCallPlay, Result);
	}
}
