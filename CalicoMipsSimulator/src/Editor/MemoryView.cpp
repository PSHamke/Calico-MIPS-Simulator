#include "MemoryView.h"
#include "Log.h"
#include "Memory.h"
#include "settings.h"
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

void MemoryView::SetMemoryType(MemoryType type)
{
	mType = type;
}


MemoryView::MemoryType MemoryView::GetMemoryType()
{
	return mType;
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

ImFont* MemoryView::GetFont() {
	return mFont;
}
void MemoryView::RenderHex()
{
	char buf1[17];
	char buf2[17];
	char buf3[20];

	int addressBase = GetMemoryType() == MemoryType::Text ?  0x400000 : 0x1001000;
	
	ImGui::PushFont(mFont);
	static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Hideable;
	float	TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

	bool focus_address = true;
	int address_index = 100;

	bool isValidMem = false;
	int result = 0;

	// When using ScrollX or ScrollY we need to specify a size for our table container!
	// Otherwise by default the table will fit all available space, like a BeginChild() call.
	//CL_CORE_WARN("Text base = {0}", TEXT_BASE_HEIGHT);
	ImVec2 outer_size = ImVec2(630, TEXT_BASE_HEIGHT * 20);
	ImVec4 activeMemory(0.0f, 0.1f, 1.0f, 1.0f);
	ImVec4 inactiveMemory(1.0f, 1.0f, 1.0f, 1.0f);
	//ImGui::SetCursorPos(ImVec2((ImGui::GetWindowWidth() - outer_size.x)/2, 70));
	if (ImGui::BeginTable("table_scrolly", 18, flags, outer_size))
	{

		ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
		ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT * 4);

		for (int i = 0; i < 16; i++) {
			sprintf(buf1, "%.2X", (i + header_index));
			ImGui::TableSetupColumn(buf1, ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT);
		}

		//ImGui::TableSetupColumn("EM", ImGuiTableColumnFlags_WidthFixed, 16);

		sprintf(buf2, "0123456789ABCDEF");
		ImGui::TableSetupColumn(buf2, ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT * 7);


		//ImGui::TableSetupColumn(buf3, ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		// Demonstrate using clipper for large vertical lists
		ImGuiListClipper clipper;
		clipper.Begin(1000);
		while (clipper.Step())
		{
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
			{
				std::string ASCIIstr = "";
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("0x%.8X", addressBase + (row) * 0x10); // Address section
				for (int column = 1; column < 17; column++)
				{
					ImGui::TableSetColumnIndex(column);
					
					result = Memory::HandleMemoryByte(row, column, mType == MemoryType::Text ? 0 : 1, isValidMem);
					ImGui::PushStyleColor(ImGuiCol_Text, isValidMem ? activeMemory : inactiveMemory);
					ImGui::Text("%.2X", isValidMem ? result : 0);
					ImGui::PopStyleColor();
					ASCIIstr += isValidMem && (char)result >= 32 ? (char)result : '.';
					
					
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
		header_index = ((clipper.DisplayStart - 1) * 16) % 0x100;
		ImGui::EndTable();
	}

	ImGui::PopFont();
}

void MemoryView::RenderBinary()
{
	
		char buf1[17];
		char buf2[17];
		char buf3[20];


		int addressBase = GetMemoryType() == MemoryType::Text ? 0x400000 : 0x0000;

		//CL_CORE_INFO ("Memsize {0}",Memory::GetTextMemory().size());
		ImGui::PushFont(mFont);
		static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Hideable;
		float	TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();


		bool focus_address = true;
		int address_index = 100;

		bool isValidMem = false;
		int result = 0;

		// When using ScrollX or ScrollY we need to specify a size for our table container!
		// Otherwise by default the table will fit all available space, like a BeginChild() call.
		//CL_CORE_WARN("Text base = {0}", TEXT_BASE_HEIGHT);
		ImVec2 outer_size = ImVec2(985, TEXT_BASE_HEIGHT * 20);
		ImVec4 activeMemory(0.0f, 0.1f, 1.0f, 1.0f);
		ImVec4 inactiveMemory(1.0f, 1.0f, 1.0f, 1.0f);
		if (ImGui::BeginTable("table_scrolly", 34, flags, outer_size))
		{

			ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
			ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT * 4);

			for (int i = 0; i < 32; i++) {
				sprintf(buf1, "%.2X", (31-i));
				ImGui::TableSetupColumn(buf1, ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT);
			}

			//ImGui::TableSetupColumn("EM", ImGuiTableColumnFlags_WidthFixed, 16);

			sprintf(buf2, "ASCII");
			ImGui::TableSetupColumn(buf2, ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT * 7);


			//ImGui::TableSetupColumn(buf3, ImGuiTableColumnFlags_None);
			ImGui::TableHeadersRow();

			// Demonstrate using clipper for large vertical lists
			ImGuiListClipper clipper;
			clipper.Begin(1000);
			while (clipper.Step())
			{
				for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
				{
					std::string ASCIIstr = "";
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("0x%.8X", addressBase + (row) * 0x4); // Address section
					for (int column = 1; column < 33; column++)
					{
						ImGui::TableSetColumnIndex(column);
						result = Memory::HandleMemoryBit(row, 32-column, mType == MemoryType::Text ? 0 : 1, isValidMem);

						ImGui::PushStyleColor(ImGuiCol_Text, isValidMem ? activeMemory : inactiveMemory);
						ImGui::Text(" %X", isValidMem ? result : 0);
						ImGui::PopStyleColor();
						ASCIIstr += isValidMem && (char)result >= 32 ? (char)result : '.';
					}

					ImGui::TableSetColumnIndex(33);
					ImGui::Text("%s", Memory::GetAscii(row));

				}
			}

			
			ImGui::EndTable();
		}

		ImGui::PopFont();
	
}

void MemoryView::Render16BitHex()
{
	char buf1[17];
	char buf2[17];
	char buf3[20];

	int addressBase = GetMemoryType() == MemoryType::Text ? 0x000000 : 0x0000;

	ImGui::PushFont(mFont);
	static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Hideable;
	float	TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

	bool focus_address = true;
	int address_index = 100;

	bool isValidMem = false;
	int result = 0;

	// When using ScrollX or ScrollY we need to specify a size for our table container!
	// Otherwise by default the table will fit all available space, like a BeginChild() call.
	//CL_CORE_WARN("Text base = {0}", TEXT_BASE_HEIGHT);
	ImVec2 outer_size = ImVec2(370, TEXT_BASE_HEIGHT * 25);
	ImVec4 activeMemory(0.0f, 0.1f, 1.0f, 1.0f);
	ImVec4 inactiveMemory(1.0f, 1.0f, 1.0f, 1.0f);
	//ImGui::SetCursorPos(ImVec2((ImGui::GetWindowWidth() - outer_size.x)/2, 70));
	if (ImGui::BeginTable("table_scrolly", 10, flags, outer_size))
	{

		ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
		ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT * 4);

		for (int i = 0; i < 8; i++) {
			sprintf(buf1, "%.2X", (i + header_index));
			ImGui::TableSetupColumn(buf1, ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT);
		}

		//ImGui::TableSetupColumn("EM", ImGuiTableColumnFlags_WidthFixed, 16);

		sprintf(buf2, "01234567");
		ImGui::TableSetupColumn(buf2, ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT * 7);


		//ImGui::TableSetupColumn(buf3, ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		// Demonstrate using clipper for large vertical lists
		ImGuiListClipper clipper;
		clipper.Begin(1000);
		while (clipper.Step())
		{
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
			{
				std::string ASCIIstr = "";
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("0x%.3X", addressBase + (row) * 0x8); // Address section
				for (int column = 1; column < 9; column++)
				{
					ImGui::TableSetColumnIndex(column);

					result = Memory::HandleMemoryByte(row, column, mType == MemoryType::Text ? 0 : 1, isValidMem);
					ImGui::PushStyleColor(ImGuiCol_Text, isValidMem ? activeMemory : inactiveMemory);
					ImGui::Text("%.2X", isValidMem ? result : 0);
					ImGui::PopStyleColor();
					ASCIIstr += isValidMem && (char)result >= 32 ? (char)result : '.';


				}

				ImGui::TableSetColumnIndex(9);
				ImGui::Text("%s", ASCIIstr.c_str());

			}
		}

		//std::cout << "Start at = " << clipper.DisplayStart << " End at = " << clipper.DisplayEnd << "Start Pos Y " << clipper.StartPosY << std::endl;
		//CL_CORE_INFO("Start at = {0}", clipper.DisplayStart);
		//CL_CORE_INFO("End at = {0}", clipper.DisplayEnd);
		/*if (focus_address) // disable for now do more research !
			ImGui::SetScrollY(clipper.ItemsHeight * clipper.DisplayStart);*/
		header_index = ((clipper.DisplayStart - 1) * 16) % 0x100;
		ImGui::EndTable();
	}

	ImGui::PopFont();
}
void MemoryView::Render16BitBinary() 
{
	char buf1[17];
	char buf2[17];
	char buf3[20];


	int addressBase = GetMemoryType() == MemoryType::Text ? 0x000000 : 0x0000;

	//CL_CORE_INFO ("Memsize {0}",Memory::GetTextMemory().size());
	//mFont->FontSize = 5;
	ImGui::PushFont(mFont);
	static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Hideable;
	float	TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();


	bool focus_address = true;
	int address_index = 100;

	bool isValidMem = false;
	int result = 0;

	// When using ScrollX or ScrollY we need to specify a size for our table container!
	// Otherwise by default the table will fit all available space, like a BeginChild() call.
	//CL_CORE_WARN("Text base = {0}", TEXT_BASE_HEIGHT);
	ImVec2 outer_size = ImVec2(370, TEXT_BASE_HEIGHT * 25);
	ImVec4 activeMemory(0.0f, 0.1f, 1.0f, 1.0f);
	ImVec4 inactiveMemory(1.0f, 1.0f, 1.0f, 1.0f);
	if (ImGui::BeginTable("table_scrolly", 18, flags, outer_size))
	{

		ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
		ImGui::TableSetupColumn("Addr", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT * 2);

		for (int i = 0; i < 16; i++) {
			sprintf(buf1, "%X", (15 - i));
			ImGui::TableSetupColumn(buf1, ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT/2);
		}

		//ImGui::TableSetupColumn("EM", ImGuiTableColumnFlags_WidthFixed, 16);

		sprintf(buf2, "ASCII");
		ImGui::TableSetupColumn(buf2, ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT * 7);


		//ImGui::TableSetupColumn(buf3, ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		// Demonstrate using clipper for large vertical lists
		ImGuiListClipper clipper;
		clipper.Begin(1000);
		while (clipper.Step())
		{
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
			{
				std::string ASCIIstr = "";
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("0x%.3X", addressBase + (row) * 0x2); // Address section
				for (int column = 1; column < 17; column++)
				{
					ImGui::TableSetColumnIndex(column);
					result = Memory::HandleMemoryBit(row, 16 - column, mType == MemoryType::Text ? 0 : 1, isValidMem);

					ImGui::PushStyleColor(ImGuiCol_Text, isValidMem ? activeMemory : inactiveMemory);
					ImGui::Text("%X", isValidMem ? result : 0);
					ImGui::PopStyleColor();
					ASCIIstr += isValidMem && (char)result >= 32 ? (char)result : '.';
				}

				ImGui::TableSetColumnIndex(17);
				ImGui::Text("%s", Memory::GetAscii(row));

			}
		}


		ImGui::EndTable();
	}

	ImGui::PopFont();
}
void MemoryView::Render(RenderType rType, MemoryType mType)
{
	SetMemoryType(mType);
	rType == RenderType::Hex ? RenderHex() : RenderBinary();
	return;
}

void MemoryView::Render16Bit(RenderType rType, MemoryType mType)
{
	SetMemoryType(mType);
	rType == RenderType::Hex ? Render16BitHex() : Render16BitBinary();
	
	return;
}