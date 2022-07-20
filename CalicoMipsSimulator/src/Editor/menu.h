#pragma once
#include <Windows.h>
#include "TextEditor.h"
#include "OutputWin.h"
namespace Menu
{

	void Theme();
	void MemoryViewTheme();
	void RegisterViewTheme();
	void TitleBar(MSG& msg);
	void TitleBarMemoryView();
	void TitleBarRegisterView();
	void Render();
	void SimulationAction();
}
