#include <iostream>
#include "Game.hpp"
#include "BattleField.hpp"


int main()
{
	
	//�X�e�[�W�쐬�P�̃e�X�g
	/*stage s = stage();
	s.PrintStage();*/
	
	//�T���e�X�g
	battle_field BattleField;
	BattleField.Battle(60);

	return 0;
}