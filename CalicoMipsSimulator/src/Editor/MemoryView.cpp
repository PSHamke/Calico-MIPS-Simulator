#include "MemoryView.h"
#include "Log.h"
#include "Memory.h"
#include "settings.h"
#include "NobleLayer.h"
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

void MemoryView::Lock()
{
	mLocked = true;
}
void MemoryView::Unlock()
{
	mLocked = false;
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
					ImGui::Text("%s", Memory::GetAscii(row, mType == MemoryType::Text ? 0 : 1));

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
		clipper.Begin(128);
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
					if (mType==MemoryType::Data)
						isValidMem = !mLocked && isValidMem;
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

void MemoryView::SetTextMemoryToolTip(int row) {
	ImVec2 m = ImGui::GetIO().MousePos;
	ImGui::SetNextWindowPos(ImVec2(m.x + 10, m.y));
	ImGui::Begin("1", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::Text("%s", NobleLayer::Noble::GetInstructionTable()[row].m_Instruction.c_str());
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(m.x + 50, m.y));
	ImGui::Begin("2", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
	//ImGui::Text("SECOND TOOLTIP");
	std::string registerNames[8] = {
		"$zero", " $v0 "," $t0 "," $t1 "," $t2 "," $s0 "," $s1 "," $ra "
	};
	Instruction* tempInst = NobleLayer::Noble::GetInstructionUMap()[NobleLayer::Noble::GetInstructionTable()[row].m_Instruction];
	std::vector<std::pair<const char*, ImU32>> header;
	std::vector<std::pair<std::string, ImU32>> data;
	
	static ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInner;
	static int row_bg_type = 1;
	static int row_bg_target = 1;
	static int cell_bg_type = 1;
	InstructionType	insType = NobleLayer::Noble::GetInstructionUMap()[NobleLayer::Noble::GetInstructionTable()[row].m_Instruction]->getType();
	int columnCount = 0;
	switch (insType)
	{
	case InstructionType::R:
		
		
		header.push_back(std::make_pair(" opcode ", 0x800020ff));
		header.push_back(std::make_pair("  rs  ", 0xffc040a0));
		header.push_back(std::make_pair("  rt  ", 0xffc040a0));
		header.push_back(std::make_pair("  rd  ", 0xffc040a0));
		header.push_back(std::make_pair("funct ", 0x60caf400));
		if (tempInst->getFunct() == 5) {
			data.push_back(std::make_pair(string_format("  0x%.2X ", tempInst->getOpcode()), 0x800020ff));
			data.push_back(std::make_pair(registerNames[NobleLayer::Noble::GetInstructionTable()[row].m_RegisterNames[0]], 0xffc040a0));
			data.push_back(std::make_pair(" N/A", 0xffc040a0));
			data.push_back(std::make_pair(" N/A", 0xffc040a0));
			data.push_back(std::make_pair(string_format(" 0x%.2X ", tempInst->getFunct()), 0x60caf400));
		}
		else {
			data.push_back(std::make_pair(string_format("  0x%.2X ", tempInst->getOpcode()), 0x800020ff));
			data.push_back(std::make_pair(registerNames[NobleLayer::Noble::GetInstructionTable()[row].m_RegisterNames[1]], 0xffc040a0));
			data.push_back(std::make_pair(registerNames[NobleLayer::Noble::GetInstructionTable()[row].m_RegisterNames[2]], 0xffc040a0));
			data.push_back(std::make_pair(registerNames[NobleLayer::Noble::GetInstructionTable()[row].m_RegisterNames[0]], 0xffc040a0));
			data.push_back(std::make_pair(string_format(" 0x%.2X ", tempInst->getFunct()), 0x60caf400));
		}
		
		columnCount = 5;
		
		break;
	case InstructionType::I:
		header.push_back(std::make_pair(" opcode ", 0x800020ff));
		header.push_back(std::make_pair("  rs  ", 0xffc040a0));
		header.push_back(std::make_pair("  rt  ", 0xffc040a0));
		header.push_back(std::make_pair("  immediate ", 0xe1dd3e00));

		if (tempInst->getOpcode() == 1) {
			data.push_back(std::make_pair(string_format("  0x%.2X ", tempInst->getOpcode()), 0x800020ff));
			data.push_back(std::make_pair(" N/A", 0xffc040a0));
			data.push_back(std::make_pair(registerNames[NobleLayer::Noble::GetInstructionTable()[row].m_RegisterNames[0]], 0xffc040a0));
			data.push_back(std::make_pair(string_format("    0x%.2X ", NobleLayer::Noble::GetInstructionTable()[row].m_Immediate), 0xe1dd3e00));
		}
		else {
			data.push_back(std::make_pair(string_format("  0x%.2X ", tempInst->getOpcode()), 0x800020ff));
			data.push_back(std::make_pair(registerNames[NobleLayer::Noble::GetInstructionTable()[row].m_RegisterNames[1]], 0xffc040a0));
			data.push_back(std::make_pair(registerNames[NobleLayer::Noble::GetInstructionTable()[row].m_RegisterNames[0]], 0xffc040a0));
			data.push_back(std::make_pair(string_format("    0x%.2X ", NobleLayer::Noble::GetInstructionTable()[row].m_Immediate), 0xe1dd3e00));
		}
		
		columnCount = 4;
		break;
	case InstructionType::J:
		header.push_back(std::make_pair(" opcode ", 0x800020ff));
		header.push_back(std::make_pair("            address           ", 0x8c0864f0));


		data.push_back(std::make_pair(string_format("  0x%.2X ", tempInst->getOpcode()), 0x800020ff));
		data.push_back(std::make_pair(string_format("            0x%.4X ", NobleLayer::Noble::GetInstructionTable()[row].m_Immediate), 0x8c0864f0));
		columnCount = 2;
	default:
		break;
	}
	
	if (ImGui::BeginTable("table1", columnCount, flags))
	{
		ImGui::TableNextRow();


		ImGui::TableNextRow();
		for (int column = 0; column < columnCount; column++) {
			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", header[column].first);
			ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, header[column].second);
		}
		ImGui::TableNextRow();
		for (int column = 0; column < columnCount; column++)
		{
			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", data[column].first.c_str());
			ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, data[column].second);


		}

		ImGui::EndTable();
	}
	ImGui::End();
}

void MemoryView::SetDataMemoryToolTip(int row)
{
	ImVec2 m = ImGui::GetIO().MousePos;
	ImGui::SetNextWindowPos(ImVec2(m.x + 10, m.y));
	ImGui::Begin("1", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::Text("%d", Memory::GetDataMemory()[row].second);
	ImGui::End();
}

void MemoryView::Render16BitBinary() 
{
	char buf1[17];
	char buf2[17];
	char buf3[20];


	int addressBase = GetMemoryType() == MemoryType::Text ? 0x00000 : 0x0000;

	//CL_CORE_INFO ("Memsize {0}",Memory::GetTextMemory().size());
	//mFont->FontSize = 5;
	ImGui::PushFont(mFont);
	static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Hideable;
	float	TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

	bool focus_address = true;
	int address_index = 100;

	bool isValidMem = false;
	int result = 0;
	static int timeCount = 0;
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
		if (mType == MemoryType::Data) {
			sprintf(buf2, "VALUE");
		}
		else {
			sprintf(buf2, "ASCII");
		}
		ImGui::TableSetupColumn(buf2, ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_HEIGHT * 7);


		//ImGui::TableSetupColumn(buf3, ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		// Demonstrate using clipper for large vertical lists
		ImGuiListClipper clipper;
		clipper.Begin(512);
		while (clipper.Step())
		{
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
			{
				std::string ASCIIstr = "";
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("0x%.3X", addressBase + (row) * 0x2); // Address section
				Memory::HandleMemoryBit(row, 0, mType == MemoryType::Text ? 0 : 1, isValidMem);
				
				if (mType== MemoryType::Text && isValidMem) {
					if (ImGui::IsItemHovered()) {
						SetTextMemoryToolTip(row);
					}
						//ImGui::SetTooltip("%s", GetToolTip(row).c_str());
				}
				else if (mType == MemoryType::Data && isValidMem) {
					if (ImGui::IsItemHovered()) {
						SetDataMemoryToolTip(row);
					}
				}
				if (mType == MemoryType::Text) {
					if (row == NobleLayer::Noble::GetActiveInsMem()) {
						activeMemory = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
					}
					else {
						activeMemory = ImVec4(0.0f, 0.1f, 1.0f, 1.0f);
					}
				}
				
				for (int column = 1; column < 17; column++)
				{
					ImGui::TableSetColumnIndex(column);
					result = Memory::HandleMemoryBit(row, 16 - column, mType == MemoryType::Text ? 0 : 1, isValidMem);
					if (mType == MemoryType::Data)
						isValidMem = !mLocked && isValidMem;
						
					ImGui::PushStyleColor(ImGuiCol_Text, isValidMem ? activeMemory : inactiveMemory);
					
					ImGui::Text("%X", isValidMem ? result : 0);
					ImGui::PopStyleColor();
					ASCIIstr += isValidMem && (char)result >= 32 ? (char)result : '.';
				}

				ImGui::TableSetColumnIndex(17);
				if (mType == MemoryType::Data) {
					if (isValidMem) {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
						ImGui::Text("%d", Memory::GetDataMemory()[row].second);
						ImGui::PopStyleColor();
					}
					else {
						ImGui::Text("N/A");
					}
					
				}
				else {
					ImGui::Text("%s", Memory::GetAscii(row, mType == MemoryType::Text ? 0 : 1));
				}
				
				
				

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