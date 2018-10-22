#pragma once

#include "Game.hpp"
#include "TreeSearchAI.hpp"
#include <string>


class battle_field
{
	tree_search_ai AI;
	bool Loop;

	std::string MakeFileName(const stage &Stage);

public:
	battle_field(stage &Stage);
	~battle_field();
	void Battle();
	void Play();
	void Stop();
};