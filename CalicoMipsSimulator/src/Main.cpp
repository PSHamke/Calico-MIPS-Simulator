#include "Application.h"
#ifdef CL_PLATFORM_WINDOWS
	#include <windows.h>
	bool g_ApplicationRunning = true;
	int APIENTRY WinMain(
		HINSTANCE hInst,
		HINSTANCE hInstPrev,
		PSTR cmdline,
		int cmdshow)
	{
		while (g_ApplicationRunning)
		{
			Application* app = CreateApplication(__argc, __argv);
			app->Run();
			delete app;
		}

		return 0;
	}



#endif