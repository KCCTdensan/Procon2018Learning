#include "BattleField.hpp"
#include "Node.hpp"
#include "Console.hpp"
#include <Windows.h>
#include <process.h>
#include <iostream>


using namespace std;

namespace battle_field
{
	static friend_node *CurrentNode;
	static bool LoopSearch = true;

	namespace
	{
		const int NumThreads = 12;
		HANDLE hThreads[NumThreads];
		DWORD32 ThreadIDs[NumThreads];

		unsigned __stdcall SearchThread(void *arg)
		{
			friend_node *CurrentNode = (friend_node*)arg;
			if (CurrentNode == nullptr)
			{
				ChangeColor(COL_RED, COL_BLACK);
				cout << "Error(In SearchThread) : CurrentNode is nullptr" << endl;
				ChangeColor(COL_WHITE, COL_BLACK);
				return -1;
			}
			while (LoopSearch)
			{
				CurrentNode->Search(100);
			}
			return 0;
		}

		void StartSearch()
		{
			LoopSearch = true;
			if (CurrentNode == nullptr)
			{
				ChangeColor(COL_RED, COL_BLACK);
				cout << "Error(In StartSearch) : CurrentNode is nullptr" << endl;
				ChangeColor(COL_WHITE, COL_BLACK);
				return;
			}
			for (int i = 0; i < NumThreads; ++i)
			{
				hThreads[i] = (HANDLE)_beginthreadex(NULL, 0, SearchThread, CurrentNode, 0, &ThreadIDs[i]);
			}
			if (hThreads == NULL)
			{
				ChangeColor(COL_RED, COL_BLACK);
				cout << "Error(In StartSearch) : Failed to create SearchThread" << endl;
				ChangeColor(COL_WHITE, COL_BLACK);
			}
		}

		void StopSearch(action_id(&Result)[stage::NumAgents])
		{
			int ResultN[ID_MaxID][ID_MaxID];
			int Max = 0;
			LoopSearch = false;
			WaitForMultipleObjects(NumThreads, hThreads, true, INFINITE);
			for (int i = 0; i < NumThreads; ++i)
			{
				CloseHandle(hThreads[i]);
			}
			CurrentNode->Result(ResultN);
			for (char i = 0; i < stage::NumAgents; ++i)
			{
				Result[i] = ID_Stay;
			}
			for (action_id i = 0; i < ID_MaxID; ++i)
			{
				for (action_id j = 0; j < ID_MaxID; ++j)
				{
					if (Max < ResultN[i][j])
					{
						Max = ResultN[i][j];
						Result[0] = i;
						Result[1] = j;
					}
				}
			}
		}
	}

	int intoCard(action_id _IntentinoID)
	{
		switch (_IntentinoID)
		{
		case 1:
			return 7;

		case 2:
			return 8;

		case 3:
			return 9;

		case 4:
			return 4;

		case 5:
			return 6;

		case 6:
			return 1;

		case 7:
			return 2;

		case 8:
			return 3;

		default:
			return 5;
		}
	}

	int intoID(int _Card)
	{
		switch (_Card)
		{
		case 7:
			return 1;

		case 8:
			return 2;

		case 9:
			return 3;

		case 4:
			return 4;

		case 6:
			return 5;

		case 1:
			return 6;

		case 2:
			return 7;

		case 3:
			return 8;

		default:
			return 0;
		}
	}

	action_id InputOpponentIntention(string PlayerName)
	{
		int CardNo;
		cout << PlayerName << endl;
		cin >> CardNo;
		return intoID(CardNo);
	}

	void InputOpponentIntentions(action_id(&Result)[stage::NumAgents])
	{
		cout << "====================== Input Opponent Intentions ======================" << endl;
		cout << "7 8 9" << endl;
		cout << "4 5 6" << endl;
		cout << "1 2 3" << endl;
		for (char i = 0; i < stage::NumAgents; ++i)
		{
			string PlayerName = "2P-";
			PlayerName += to_string(i + 1);
			ChangeColor(BackColors[Team_2P][i], COL_BLACK);
			Result[i] = InputOpponentIntention(PlayerName);
			ChangeColor(COL_WHITE, COL_BLACK);
		}
	}

	void OutputFriendIntention(string PlayerName, const action_id Intention)
	{
		cout << PlayerName << " : " << intoCard(Intention) << endl;
	}

	void OutputFriendIntentions(const action_id(&Intentions)[stage::NumAgents])
	{
		cout << "====================== Output Friend Intentions ======================" << endl;
		cout << "7 8 9" << endl;
		cout << "4 5 6" << endl;
		cout << "1 2 3" << endl;
		for (char i = 0; i < stage::NumAgents; ++i)
		{
			string PlayerName = "1P-";
			PlayerName += to_string(i + 1);
			ChangeColor(BackColors[Team_1P][i], COL_BLACK);
			OutputFriendIntention(PlayerName, Intentions[i]);
			ChangeColor(COL_WHITE, COL_BLACK);
		}
	}

	void Play(int CntTurns)
	{
		cout << "================================================================" << endl;
		cout << CntTurns + 1 << "ƒ^[ƒ“–Ú" << endl;
		cout << "================================================================" << endl;
		if (CurrentNode == nullptr)
		{
			cout << "Error(In Play) : CurrentNode is nullptr" << endl;
			return;
		}
		StartSearch();
		cout << "”CˆÓ‚ÌƒL[‚Å’Tõ‚ð’†’f‚µ‚Ä‚­‚¾‚³‚¢" << endl;
		{
			char Dummy;
			cin >> Dummy;
		}
		action_id Intentions[NumTeams][stage::NumAgents];
		StopSearch(Intentions[Team_1P]);
		OutputFriendIntentions(Intentions[Team_1P]);
		InputOpponentIntentions(Intentions[Team_2P]);
		if (CurrentNode == nullptr)
		{
			ChangeColor(COL_RED, COL_BLACK);
			cout << "Error(In Play) : CurrentNode is nullptr" << endl;
			ChangeColor(COL_WHITE, COL_BLACK);
			return;
		}
		opponent_node *OpponentNode = CurrentNode->ChildNode(Intentions[Team_1P]);
		if (OpponentNode == nullptr)
		{
			ChangeColor(COL_RED, COL_BLACK);
			cout << "Error(In Play) : OpponentNode is nullptr" << endl;
			ChangeColor(COL_WHITE, COL_BLACK);
			return;
		}
		OpponentNode->ChildNode(Intentions[Team_2P]);
		bool CanAct[NumTeams][stage::NumAgents];
		CurrentNode->GetStage().CanAction(Intentions, CanAct);
		for (int i = 0; i < NumTeams; ++i)
		{
			for (int j = 0; j < stage::NumAgents; ++j)
			{
				if (!CanAct[i][j])
				{
					Intentions[i][j] = ID_Stay;
				}
			}
		}
		CurrentNode = CurrentNode->UpdateCurrentNode(Intentions);
		OpponentNode = nullptr;
		CurrentNode->GetStage().PrintStage();
		cout << "1PScore : " << CurrentNode->GetStage().GetScore1P() << endl;
		cout << "2PScore : " << CurrentNode->GetStage().GetScore2P() << endl;
	}

	void Battle(int NumTurns)
	{
		node::ChangeNumTurns(NumTurns);
		{
			stage Stage;
			//cout << "ƒ`[ƒ€Ø‚è‘Ö‚¦ YES 1  NO 0" << endl;
			//int isSwitch;
			//cin >> isSwitch;
			//if (isSwitch == 1) { Stage.isSwitch = true; }
			CurrentNode = new friend_node(Stage, 0);
		}
		if (CurrentNode == nullptr)
		{
			return;
		}
		node::Init();
		CurrentNode->PrintStage();

		for (int i = 0; i < NumTurns; ++i)
		{
			Play(i);
		}
		node::Deinit();
		delete CurrentNode;
	}
}