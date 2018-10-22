#include "Export.h"


namespace
{
	stage *Stage = nullptr;
}

void __stdcall Init(const char *QRCodeString)
{
	Stage = new stage(QRCodeString);
}

void AI_API Deinit()
{
	delete Stage;
	Stage = nullptr;
}

