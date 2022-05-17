#include "RegisterView.h"
#include "Log.h"
#include "Registers.h"
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
	//ImGui::PushFont(mFont); // Use Regular font maybe ?
	static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Hideable;
	float	TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
	int header_index = 0;
	RegisterInfos *regInfos = Register::GetRegisterInfos();
	//CL_CORE_WARN ("{0}", regInfos->at(0).first);
	bool focus_address = true;
	int address_index = 100;
	std::string HeaderStr[3] = { "[DEC]", "[HEX]","[BIN]" };
	
	ImGuiListClipper clipper;
	// When using ScrollX or ScrollY we need to specify a size for our table container!
	// Otherwise by default the table will fit all available space, like a BeginChild() call.
	//CL_CORE_WARN("Text base = {0}", TEXT_BASE_HEIGHT);
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
		
		//ImGui::TableSetupColumn("EM", ImGuiTableColumnFlags_WidthFixed, 16);

		
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT * 5* (valueState+1));
		static bool column_selected[3] = {};

		//ImGui::TableSetupColumn(buf3, ImGuiTableColumnFlags_None);
		//ImGui::TableHeadersRow();
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
		CL_CORE_INFO("Num = {0}  Value = {1}", numberState, valueState);
		// Demonstrate using clipper for large vertical lists

		clipper.Begin(32);
		while (clipper.Step())
		{
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
			{
				ImGui::TableNextRow();
				if (row <= 0) {
					//CL_CORE_INFO("Start at = {0}", clipper.DisplayStart);
				}
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%s", regInfos->at(row).first.c_str()); // Address section
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
					ImGui::Text("%d", MIPSLayer::MIPS::GetRegisterUMap()[regInfos->at(row).first.c_str()]->getValue());
				}
				else if (valueState == 1) {
					ImGui::Text("0x%.8X", MIPSLayer::MIPS::GetRegisterUMap()[regInfos->at(row).first.c_str()]->getValue());
				}
				else if (valueState == 2) {
					ImGui::Text("0b%s", std::bitset<32> (MIPSLayer::MIPS::GetRegisterUMap()[regInfos->at(row).first.c_str()]->getValue()).to_string().c_str());
				}
				{

					
					
				}/*
				for (int column = 17; column < 18; column++)
				{

					ImGui::TableSetColumnIndex(column);
					ImGui::Text("................", column, row - 1);
				}*/

			}
		}
		/*sprintf("", "%d", clipper.DisplayStart);
		sprintf(buf2, "%d", clipper.DisplayEnd);
		sprintf(buf3, "%f", clipper.StartPosY);*/
		//std::cout << "Start at = " << clipper.DisplayStart << " End at = " << clipper.DisplayEnd << "Start Pos Y " << clipper.StartPosY << std::endl;
		
		//CL_CORE_INFO("End at = {0}", clipper.DisplayEnd);
		if (focus_address)
			//ImGui::SetScrollY(clipper.ItemsHeight * (clipper.DisplayStart));
		focus_address = false;
		header_index = ((clipper.DisplayStart - 1) * 16) % 0x100;
		ImGui::EndTable();
	}

	delete regInfos;
	//ImGui::PopFont();
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
