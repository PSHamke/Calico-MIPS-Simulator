#pragma once
#include <iostream>
#include <vector>

namespace Settings
{
	static int Tab = 0;
	static bool MemoryViewActive = 0;
	static bool RegisterViewActive = 0;
	static int MemoryViewType = 0;
	static int MemoryKind = 0;
	static bool ExecutePressed = 0;
	static bool AssemblePressed = 0;
	static bool StopPressed = 0;
	static int LineCounter = 0;
	static int ExecCounter = 0;
	static bool LabelCounter = 0;
	static const char* ViewType[2] = { "Register & Memory", "Data&Control Path" };
	static int ViewTypeCounter = 0;
}