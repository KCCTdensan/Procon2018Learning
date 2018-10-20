#include <iostream>
#include "Game.hpp"
#include "BattleField.hpp"
#include "Random.hpp"
#include <time.h>

using namespace std;

int main()
{
	//’TõƒeƒXƒg
	int Time_s = 0;
	double integralTime = 0;
	std::cin >> Time_s;
	while (integralTime < Time_s)
	{
		clock_t start = clock();
		battle_field BattleField;
		BattleField.Battle(60);
		clock_t end = clock();
		integralTime += (double)(end - start)/CLOCKS_PER_SEC;
		std::cout << "TotalTime:" << integralTime <<  "s" << endl;
		std::cout << "LastProcessTime" << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
	}

	return 0;
}