#include "BattleField.hpp"


battle_field::battle_field()
	:CurrentNode(nullptr, Stage)
{

}

battle_field::~battle_field()
{

}

void battle_field::Battle(int NumTurn)
{
	for (int i = 0; i < NumTurn; ++i)
	{
		float Result[Max_ActionID * Max_ActionID];
		CurrentNode.Search(node::NumCallPlay, Result);
	}
}
