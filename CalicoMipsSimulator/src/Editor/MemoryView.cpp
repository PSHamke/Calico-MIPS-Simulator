#include "MemoryView.h"


std::vector<MemoryView*> MemoryView::Instances;

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
	char buf1[17];
	char buf2[17];
	char buf3[20];
	int header_index = 0;
	int addressBase = 0x400000;
	int virtualMemory[1000][16];
	//mFont->Scale = 0.75;
	//ImGui::PushFont(mFont);
	static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Hideable;
	float	TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
	
	bool focus_address = true;
	int address_index = 100;
	ImGuiListClipper clipper;
	// When using ScrollX or ScrollY we need to specify a size for our table container!
	// Otherwise by default the table will fit all available space, like a BeginChild() call.
	ImVec2 outer_size = ImVec2(700.0f, TEXT_BASE_HEIGHT * 20);
	if (ImGui::BeginTable("table_scrolly", 33, flags, outer_size))
	{

		ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
		ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed, 70);
		for (int i = 0; i < 16; i++) {
			sprintf(buf1, "%.2X", (i + header_index));
			ImGui::TableSetupColumn(buf1, ImGuiTableColumnFlags_WidthFixed, 16);
		}
		//ImGui::TableSetupColumn("EM", ImGuiTableColumnFlags_WidthFixed, 16);
		for (int i = 0; i < 16; i++) {
			sprintf(buf2, "%X", (i));
			ImGui::TableSetupColumn(buf2, ImGuiTableColumnFlags_WidthFixed, 5);
		}

		//ImGui::TableSetupColumn(buf3, ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		// Demonstrate using clipper for large vertical lists

		clipper.Begin(1000);
		while (clipper.Step())
		{
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
			{
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("0x%.8X", addressBase + row * 0x10); // Address section
				for (int column = 1; column < 17; column++)
				{

					ImGui::TableSetColumnIndex(column);
					ImGui::Text("%.2X", virtualMemory[row][column - 1]);
				}
				for (int column = 17; column < 33; column++)
				{

					ImGui::TableSetColumnIndex(column);
					ImGui::Text("A", column, row - 1);
				}

			}
		}
		sprintf(buf1, "%d", clipper.DisplayStart);
		sprintf(buf2, "%d", clipper.DisplayEnd);
		sprintf(buf3, "%f", clipper.StartPosY);
		//std::cout << "Start at = " << clipper.DisplayStart << " End at = " << clipper.DisplayEnd << "Start Pos Y " << clipper.StartPosY << std::endl;
		if (focus_address)
			ImGui::SetScrollY(clipper.ItemsHeight * clipper.DisplayStart);
		focus_address = false;
		header_index = ((clipper.DisplayStart - 1) * 16) % 0x100;
		ImGui::EndTable();
	}
	//mFont->Scale = 1.0;
	//ImGui::PopFont();
}