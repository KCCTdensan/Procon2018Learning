#include<iostream>
#include<random>
#include"Search.hpp"



void Node::Selection(){
	
}
void Node::Evalution(){

}

Node::Node(Stage s){
	this->Stage = s;
}
void Node::Search(Stage stage) {
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (int k = 0; k < 4; k++) {
				Intention intention;
				Intention intention2;
				switch (k) {
				case 0:
					intention.Action = IA_MoveAgent;
					intention2.Action = IA_MoveAgent;
				case 1:
					intention.Action = IA_MoveAgent;
					intention2.Action = IA_RemovePanel;
				case 2:
					intention.Action = IA_RemovePanel;
					intention2.Action = IA_MoveAgent;
				case 3:
					intention.Action = IA_RemovePanel;
					intention2.Action = IA_RemovePanel;
				}
				switch (i) {
				case 0:
					intention.DeltaX = 0;
					intention.DeltaY = 1;
				case 1:
					intention.DeltaX = 1;
					intention.DeltaY = 1;
				case 2:
					intention.DeltaX = 1;
					intention.DeltaY = 0;
				case 3:
					intention.DeltaX = 1;
					intention.DeltaY = -1;
				case 4:
					intention.DeltaX = 0;
					intention.DeltaY = -1;
				case 5:
					intention.DeltaX = -1;
					intention.DeltaY = -1;
				case 6:
					intention.DeltaX = -1;
					intention.DeltaY = 0;
				case 7:
					intention.DeltaX = -1;
					intention.DeltaY = 1;
				}
				switch (j) {
				case 0:
					intention2.DeltaX = 0;
					intention2.DeltaY = 1;
				case 1:
					intention2.DeltaX = 1;
					intention2.DeltaY = 1;
				case 2:
					intention2.DeltaX = 1;
					intention2.DeltaY = 0;
				case 3:
					intention2.DeltaX = 1;
					intention2.DeltaY = -1;
				case 4:
					intention2.DeltaX = 0;
					intention2.DeltaY = -1;
				case 5:
					intention2.DeltaX = -1;
					intention2.DeltaY = -1;
				case 6:
					intention2.DeltaX = -1;
					intention2.DeltaY = 0;
				case 7:
					intention2.DeltaX = -1;
					intention2.DeltaY = 1;
				}

			}
		}
	}
}
bool Node::rollout(Stage s,int turn) {
	for (int i = turn; i > 0;i--) {
		std::random_device rand;
		Intention intentionA;
		Intention intentionB;
		Intention intentinoC;
		Intention intentionD;
		intentionA.DeltaX = rand() % 3 - 1;
		intentionA.DeltaY = rand() % 3 - 1;
		intentionB.DeltaX = rand() % 3 - 1;
		intentionB.DeltaY = rand() % 3 - 1;
		intentionC.DeltaX = rand() % 3 - 1;
		intentionC.DeltaY = rand() % 3 - 1;
		intentionD.DeltaX = rand() % 3 - 1;
		intentionD.DeltaY = rand() % 3 - 1;
		s.Action({ intentionA,intentionB,intentinoC,intentionD });
	}
}