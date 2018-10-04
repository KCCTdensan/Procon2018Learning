#pragma once
#include"Game.hpp"

class Node{
Stage Stage;
	int Q;
	int N;
	Node* childnode;
	Node(Stage s);
};