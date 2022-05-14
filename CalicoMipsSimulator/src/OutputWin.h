#pragma once
#include "imgui.h"
#include <string>

class OutputWindow {
public :
	OutputWindow() {}
	~OutputWindow() {}
	void Render(const char* aTitle, const ImVec2& aSize, bool aBorder = false);
	std::string output = "";
};