#include <iostream>
#include "Game.hpp"
#include "BattleField.hpp"
#include "Random.hpp"
#include <time.h>


using namespace std;

int main(int argc,char *argv[])
{
	//’TõƒeƒXƒg
	stage Stage(STAGE_INIT_RANDOM);
	battle_field BattleField(Stage);
	BattleField.Battle(60);

	return 0;
}