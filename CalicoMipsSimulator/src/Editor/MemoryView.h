#pragma once
#include <string>
#include "imgui.h"
#include <vector>
class MemoryView {
public:
	enum class RenderType {
		Hex,
		Bin
	};
	enum class MemoryType {
		Text,
		Data
	};
	// Make it singleton
	MemoryView();
	~MemoryView();
	void Render(RenderType rType, MemoryType mType );

	static MemoryView* CreateInstance(const std::string& pInstanceID, unsigned int flag = 0);
	static MemoryView* GetInstance(const std::string& pInstanceID);
	static std::vector<MemoryView*>& GetAllInstances();
	static std::vector<MemoryView*> Instances;
	static bool FreeInstance(const std::string& pInstanceID);
	static bool FreeAllInstances();
	bool SetInstanceId(const std::string& const pInstanceID);
	void SetFont(ImFont* font);
	void SetMemoryType(MemoryType);
	MemoryType GetMemoryType();
	ImFont* GetFont();
	void SetFlag(unsigned int flag);
	std::string GetInstanceId();
private:
	std::string mInstanceId;
	int mFlag;
	ImFont* mFont;
	MemoryType mType;
	void RenderHex();
	void RenderBinary();
};