#pragma once
#include <iostream>
#include <vector>

namespace Settings
{
	static int Tab = 2;
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
	static int PathNum = 0;
	static const char* ViewType[2] = { "INSPECT", "Data&Control Path" };
	static const char* TabNames[4] = { "CALICO SIMULATOR","CALICO|MIPS-32", "CALICO|NOBLE-16","CALICO|NOBLE DATAPATH"};
	static int ViewTypeCounter = 0;
	static bool AssembleCheck = false;
	static bool ExecuteStatus = false;
	static bool StepForwardStatus = false;
	static bool StepBackwardStatus = false;
	static bool ResetStatus = false;
	static char SimulationName[128] = "Test";
	static char InputAsmPath[128]="";
	static char InputInitialMem[128]="";
	static char OutputRegFile[128] = "";
	static char OutputDataMem[128]="";
}