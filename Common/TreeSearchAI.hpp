#pragma once

#include "Node.hpp"


class tree_search_ai
{
	friend_node *CurrentNode;

public:
	tree_search_ai(stage &Stage);
	~tree_search_ai();
	void NumTurns(unsigned char NumTurns);
	void BestMove(action_id(&IntentionIDs)[NumTeams][stage::NumAgents]);
	void Move(action_id(&IntentionIDs)[NumTeams][stage::NumAgents]);
	void PrintStage()const;
};