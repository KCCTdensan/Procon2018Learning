#include "Random.hpp"


namespace random
{
	std::random_device RandomDev;
	std::mt19937 RandomMT;

	int ZeroToMax(int MaxValue)
	{
		return RandomMT() % ++MaxValue;
	}

	int Mod(int Value)
	{
		return RandomMT() % Value;
	}
}