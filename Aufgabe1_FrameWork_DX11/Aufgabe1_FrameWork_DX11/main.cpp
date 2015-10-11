#include "enginecore.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	EngineCore* Core;
	bool result;


	// Create the system object.
	Core = new EngineCore;
	if (!Core)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = Core->Init();
	if (result)
	{
		Core->Run();
	}

	// Shutdown and release the system object.
	delete Core;
	Core = 0;

	return 0;
}