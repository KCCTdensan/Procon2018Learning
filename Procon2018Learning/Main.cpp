#include <iostream>
#include "Game.hpp"
#include "BattleField.hpp"


int main()
{
	
	//�X�e�[�W�쐬�P�̃e�X�g
	stage s;
	s.InitRandomStage();
	s.PrintStage();
	
	//�T���e�X�g
	battle_field BattleField(s);
	BattleField.Battle(60);

	return 0;
}