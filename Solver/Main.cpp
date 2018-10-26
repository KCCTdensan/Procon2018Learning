#include <iostream>
#include "Game.hpp"
#include "BattleField.hpp"
#include "Random.hpp"
#include <time.h>

using namespace std;

int main(int argc, char *argv[])
{
	battle_field BattleField;
	BattleField.Battle(40);

	return 0;
}