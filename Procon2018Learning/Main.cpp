#include "BattleField.hpp"
#include <Windows.h>
#include <process.h>
#include <iostream>
#include <fstream>
#include <direct.h>


namespace
{
	bool Loop = true;
	bool IsField = false;
	battle_field *Field = nullptr;
}

using namespace std;

unsigned __stdcall WaitThread(void *arg)
{
	string Input;
	do
	{
		cout << "�v���O�������I������ꍇ�� s ����͂��Ă�������" << endl;
		cout << "���b��Ƀv���O��������~���܂�" << endl;
		cin >> Input;
	} while (Input != "s");
	while (!IsField)
	{
		Sleep(10);
	}
	Loop = false;
	Field->Stop();
	return 0;
}

int main()
{
	DWORD32 ThreadID;
	HANDLE hThread = (HANDLE)_beginthreadex(nullptr, 0, WaitThread, nullptr, 0, &ThreadID);
	if (hThread == NULL)
	{
		cout << "Error : �X���b�h�̍쐬�Ɏ��s���܂���" << endl;
	}
	_mkdir("TrainingData/");
	
	while (Loop)
	{
		stage Stage(STAGE_INIT_RANDOM);
		Field = new battle_field(Stage);
		IsField = true;
		Field->Battle();
		IsField = false;
		delete Field;
		Field = nullptr;
	}
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 0;
}