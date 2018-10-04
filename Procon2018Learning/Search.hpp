#pragma once

#include <vector>
#include "Game.hpp"

class Node{
Stage Stage;
	int Q;
	int N;
	Node *Parent;
	std::vector<Node> *child;

public:
	Node(Stage s);
	void Search();
	bool rollout();
};