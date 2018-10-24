#pragma once

#include "Game.hpp"
#include "Node.hpp"


extern "C" void __stdcall Init(const char *QRCodeString);
extern "C" void __stdcall Deinit();
extern "C" void __stdcall SetNumTurns(int NumTurns);
extern "C" void __stdcall BestMove(action_id *ID1, action_id *ID2);
extern "C" void __stdcall Move(action_id FriendID1, action_id FriendID2, action_id OpponentID1, action_id OpponentID2);