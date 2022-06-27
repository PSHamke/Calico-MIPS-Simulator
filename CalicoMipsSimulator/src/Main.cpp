#include "Application.h"
#ifdef CL_PLATFORM_WINDOWS

	#include <windows.h>
	
	
	int APIENTRY WinMain(
		HINSTANCE hInst,
		HINSTANCE hInstPrev,
		PSTR cmdline,
		int cmdshow)
	{
		Application* app = CreateApplication(__argc, __argv);
		app->Run();
		delete app;
		
		return 0;
	}


#endif