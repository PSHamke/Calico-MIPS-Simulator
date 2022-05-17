#pragma once
#include  <vector>
#include <string>
#include "imgui.h"
class RegisterView {
	
public:

	// Make it singleton
	RegisterView();
	~RegisterView();
	void Render();

	static RegisterView* CreateInstance(const std::string& pInstanceID, unsigned int flag = 0);
	static RegisterView* GetInstance(const std::string& pInstanceID);
	static std::vector<RegisterView*>& GetAllInstances();
	static std::vector<RegisterView*> Instances;
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