#include<iostream>
#include"Search.hpp"


/*int Search(stage stage){
	for (i = 0;i < 8;i++){
		for (j = 0;j < 8;j++){
			for (int k = 0; k < 4;k++){
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
	node Node();
}*/

int Selection() {
	
}
int Evalution() {

}
void Backup() {

}

node::node(stage &Stage){
	//this->Stage = Stage;
}

void node::Expansion(team_no Team)
{

}
