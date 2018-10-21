#pragma once

#include "Game.hpp"

#define AI_API	__stdcall __declspec(dllexport)


void AI_API Init(const char *QRCodeString);
void AI_API Deinit();