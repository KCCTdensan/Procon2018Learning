#pragma once

#include "Game.hpp"
#include "Node.hpp"


__declspec(dllexport) void __stdcall Init(const char *QRCodeString);
__declspec(dllexport) void __stdcall Deinit();
__declspec(dllexport) void __stdcall SetNumTurns(int NumTurns);
__declspec(dllexport) void __stdcall BestMove(action_id *ID1, action_id *ID2);
__declspec(dllexport) void __stdcall Move(action_id FriendID1, action_id FriendID2, action_id OpponentID1, action_id OpponentID2);