#include "MemoryView.h"
#include "Log.h"
#include "Memory.h"
std::vector<MemoryView*> MemoryView::Instances;
int header_index = 0;
MemoryView::MemoryView() {
	
}
MemoryView::~MemoryView() {
	
}

MemoryView* MemoryView::CreateInstance(const std::string& pInstanceID, unsigned int flag)
{
	MemoryView* instance = new MemoryView();
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

bool MemoryView::FreeInstance(const std::string& pInstanceID)
{
	for (std::vector<MemoryView*>::iterator i = Instances.begin(), endI = Instances.end(); i != endI; ++i)
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

bool MemoryView::FreeAllInstances() {

	for (std::vector<MemoryView*>::iterator i = Instances.begin(), endI = Instances.end(); i != endI; ++i)
	{
		delete* i;
	}
	Instances.clear();
	return true;
}



bool MemoryView::SetInstanceId(const std::string& const pInstanceID)
{
	if (!GetInstance(pInstanceID)) {
		mInstanceId = pInstanceID;
		return true;
	}
	return false;
}

void MemoryView::SetFont(ImFont* font)
{
	mFont = font;
}

void MemoryView::SetFlag(unsigned int flag)
{

}

MemoryView* MemoryView::GetInstance(const std::string& pInstanceID)
{
	for (auto& instance : Instances)
	{
		if (instance->GetInstanceId() == pInstanceID)
			return instance;
	}
	return nullptr;
}

std::vector<MemoryView*>& MemoryView::GetAllInstances()
{
	return Instances;
}

std::string MemoryView::GetInstanceId()
{
	return this->mInstanceId;
}


void MemoryView::Render()
{
	
	
	int addressBase = 0x400000;
	ImGui::PushFont(mFont);
	static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Hideable;
	float	TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

	
	bool focus_address = true;
	int address_index = 100;
	ImGuiListClipper clipper;
	bool isValidMem = false;
	int result = 0;
	
	// When using ScrollX or ScrollY we need to specify a size for our table container!
	// Otherwise by default the table will fit all available space, like a BeginChild() call.
	//CL_CORE_WARN("Text base = {0}", TEXT_BASE_HEIGHT);
	ImVec2 outer_size = ImVec2(630, TEXT_BASE_HEIGHT * 20);
	ImVec4 activeMemory(0.0f, 0.1f, 1.0f, 1.0f);
	ImVec4 inactiveMemory(1.0f, 1.0f, 1.0f, 1.0f);
	if (ImGui::BeginTable("table_scrolly", 18, flags,outer_size))
	{

		ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
		ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT*4);
		
		for (int i = 0; i < 16; i++) {
			sprintf(buf1, "%.2X", (i + header_index));
			ImGui::TableSetupColumn(buf1, ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT);
		}
	
		//ImGui::TableSetupColumn("EM", ImGuiTableColumnFlags_WidthFixed, 16);
		
			sprintf(buf2, "0123456789ABCDEF");
			ImGui::TableSetupColumn(buf2, ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT*7);
		

		//ImGui::TableSetupColumn(buf3, ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		// Demonstrate using clipper for large vertical lists

		clipper.Begin(1000);
		while (clipper.Step())
		{
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
			{
				std::string ASCIIstr = "";
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("0x%.8X", addressBase + (row)* 0x10); // Address section
				for (int column = 1; column < 17; column++)
				{
					ImGui::TableSetColumnIndex(column);
					result = Memory::HandleTextMemory(row, column, isValidMem);
					
					ImGui::PushStyleColor(ImGuiCol_Text, isValidMem ? activeMemory : inactiveMemory);
					ImGui::Text("%.2X", isValidMem ? result : 0);
					ImGui::PopStyleColor();
					ASCIIstr += isValidMem && (char)result>=32 ? (char)result : '.';
				}
				
					ImGui::TableSetColumnIndex(17);
					ImGui::Text("%s", ASCIIstr.c_str());
				
			}
		}

		//std::cout << "Start at = " << clipper.DisplayStart << " End at = " << clipper.DisplayEnd << "Start Pos Y " << clipper.StartPosY << std::endl;
		//CL_CORE_INFO("Start at = {0}", clipper.DisplayStart);
		//CL_CORE_INFO("End at = {0}", clipper.DisplayEnd);
		/*if (focus_address) // disable for now do more research !
			ImGui::SetScrollY(clipper.ItemsHeight * clipper.DisplayStart);*/
		focus_address = false;
		header_index = ((clipper.DisplayStart - 1) * 16) % 0x100;
		ImGui::EndTable();
	}

	ImGui::PopFont();
}