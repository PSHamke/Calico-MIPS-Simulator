#include "RegisterView.h"
#include "Log.h"
#include "AssemblyCore/Registers.h"
#include "bitset"
#include "HelperString.h"
#include "MipsLayer.h"

std::vector<RegisterView*> RegisterView::Instances;
int valueState = 0;
int numberState = 0;
RegisterView::RegisterView() {

}
RegisterView::~RegisterView() {

}

void RegisterView::Render()
{
	static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Hideable;
	float	TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
	RegisterInfos *regInfos = Register::GetRegisterInfos32();
	std::string HeaderStr[3] = { "[DEC]", "[HEX]","[BIN]" };
	int registerValue = 0;
	ImVec4 activeRegister(0.0f, 0.1f, 1.0f, 1.0f);
	ImVec4 inactiveRegister(1.0f, 1.0f, 1.0f, 1.0f);
	
	// When using ScrollX or ScrollY we need to specify a size for our table container!
	// Otherwise by default the table will fit all available space, like a BeginChild() call.
	
	ImVec2 outer_size[3] = { ImVec2(320, TEXT_BASE_HEIGHT * 20),
		ImVec2(340, TEXT_BASE_HEIGHT * 20),
		ImVec2(610, TEXT_BASE_HEIGHT * 20)
	};
	
	ImGui::SetCursorPosX(( ImGui::GetWindowWidth()- outer_size[valueState].x) /2);
	if (ImGui::BeginTable("table_scrolly", 3, flags, outer_size[valueState]))
	{

		ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT * 2);

		
		ImGui::TableSetupColumn("Number", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT*5);
		
		
		
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT * 5* (valueState+1));
		static bool column_selected[3] = {};

		
		ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
		// A bit spagetti // Find Fix 
		ImGui::TableSetColumnIndex(0);
		const char* column_name = ImGui::TableGetColumnName(0); // Retrieve name passed to TableSetupColumn()
		ImGui::PushID(0);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::Text(column_name);
		ImGui::PopStyleVar();
		ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
		ImGui::PopID();
		
		ImGui::TableSetColumnIndex(1);
		column_name = ImGui::TableGetColumnName(1); // Retrieve name passed to TableSetupColumn()
		ImGui::PushID(1);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.3, 0.2, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1, 0.3, 0.2, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1, 0.3, 0.2, 1));
		if (ImGui::Button(string_format("Number %s", HeaderStr[numberState].c_str()).c_str())) {
			numberState = (numberState + 1) % 3;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();
		ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
		ImGui::PopID();
		
		ImGui::TableSetColumnIndex(2);
		column_name = ImGui::TableGetColumnName(2); // Retrieve name passed to TableSetupColumn()
		ImGui::PushID(2);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.3, 0.2, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1, 0.3, 0.2, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1, 0.3, 0.2, 1));
		if (ImGui::Button(string_format("Value %s", HeaderStr[valueState].c_str()).c_str())) {
			valueState = (valueState + 1) % 3;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();
		ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
		ImGui::PopID();
		//CL_CORE_INFO("Num = {0}  Value = {1}", numberState, valueState);
		// Demonstrate using clipper for large vertical lists
		ImGuiListClipper clipper;
		clipper.Begin(32);
		while (clipper.Step())
		{
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
			{
				registerValue = MIPSLayer::MIPS::GetRegisterUMap()[regInfos->at(row).first.c_str()]->getValue();
				ImGui::PushStyleColor(ImGuiCol_Text,registerValue ? activeRegister : inactiveRegister);
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%s", regInfos->at(row).first.c_str()); // Register name
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", regInfos->at(row).second.c_str());
				
				ImGui::TableSetColumnIndex(1);	
				if (numberState == 0) {
					ImGui::Text("%d", row);
				}
				else if (numberState == 1) {
					ImGui::Text("0x%.2X", row);
				}
				else if (numberState == 2) {
					ImGui::Text("0b%s", std::bitset<5> (row).to_string().c_str());
				}
				
				ImGui::TableSetColumnIndex(2);
				if (valueState == 0) {
					ImGui::Text("%d", registerValue);
				}
				else if (valueState == 1) {
					ImGui::Text("0x%.8X", registerValue);
				}
				else if (valueState == 2) {
					ImGui::Text("0b%s", std::bitset<32> (registerValue).to_string().c_str());
				}
				ImGui::PopStyleColor(1);
			}
		}
		ImGui::EndTable();
	}

	delete regInfos;
}

RegisterView* RegisterView::CreateInstance(const std::string& pInstanceID, unsigned int flag)
{
	RegisterView* instance = new RegisterView();
	if (instance->SetInstanceId(pInstanceID)) {
		instance->SetFlag(flag);
		Instances.push_back(instance);
		return instance;
	}
	else {
		delete instance;
		return nullptr;
	}
}

bool RegisterView::FreeInstance(const std::string& pInstanceID)
{
	for (std::vector<RegisterView*>::iterator i = Instances.begin(), endI = Instances.end(); i != endI; ++i)
	{
		if ((*i)->GetInstanceId() == pInstanceID)
		{
			delete* i;
			Instances.erase(i);
			return true;
		}
	}
	return false;
}

bool RegisterView::FreeAllInstances() {

	for (std::vector<RegisterView*>::iterator i = Instances.begin(), endI = Instances.end(); i != endI; ++i)
	{
		delete* i;
	}
	Instances.clear();
	return true;
}



bool RegisterView::SetInstanceId(const std::string& const pInstanceID)
{
	if (!GetInstance(pInstanceID)) {
		mInstanceId = pInstanceID;
		return true;
	}
	return false;
}

void RegisterView::SetFont(ImFont* font)
{
	mFont = font;
}

void RegisterView::SetFlag(unsigned int flag)
{

}

RegisterView* RegisterView::GetInstance(const std::string& pInstanceID)
{
	for (auto& instance : Instances)
	{
		if (instance->GetInstanceId() == pInstanceID)
			return instance;
	}
	return nullptr;
}

std::vector<RegisterView*>& RegisterView::GetAllInstances()
{
	return Instances;
}

std::string RegisterView::GetInstanceId()
{
	return this->mInstanceId;
}
