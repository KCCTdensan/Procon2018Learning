#include <iostream>
#include "Game.hpp"
#include "BattleField.hpp"


int main()
{
	
	//ステージ作成単体テスト
	/*stage s = stage();
	s.PrintStage();*/
	
	//探索テスト
	battle_field BattleField;
	BattleField.Battle(60);

	return 0;
}