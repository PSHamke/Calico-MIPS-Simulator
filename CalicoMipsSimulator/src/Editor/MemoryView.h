#pragma once
#include <string>
#include "imgui.h"
#include <vector>
class MemoryView {
public:

	// Make it singleton
	MemoryView();
	~MemoryView();
	void Render();

	static MemoryView* CreateInstance(const std::string& pInstanceID, unsigned int flag = 0);
	static MemoryView* GetInstance(const std::string& pInstanceID);
	static std::vector<MemoryView*>& GetAllInstances();
	static std::vector<MemoryView*> Instances;
	static bool FreeInstance(const std::string& pInstanceID);
	static bool FreeAllInstances();
	bool SetInstanceId(const std::string& const pInstanceID);
	void SetFont(ImFont* font);
	void SetFlag(unsigned int flag);
	std::string GetInstanceId();
private:
	std::string mInstanceId;
	int mFlag;
	ImFont* mFont;
};