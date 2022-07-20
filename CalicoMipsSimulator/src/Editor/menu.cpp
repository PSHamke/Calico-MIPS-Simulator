#include "menu.h"
#include "imguipp.h"
#include "icons.h"
#include "OutputWin.h"
#include "Log.h"
#include "MemoryView.h"
#include "RegisterView.h"
#include "HelperString.h"
#include "settings.h"
#include "MipsLayer.h"
#include "NobleLayer.h"
#include "DataControlPath/ImDrawer.h"
#include "bitset"
#include "iostream"
#include "fstream"
extern ImFont* Consolas;


void Menu::Render()
{
	ImGui::Columns(4);
	ImGui::SetColumnOffset(1, 230);
	//First Column
	{
		//Left side
		static ImVec4 active = imguipp::to_vec4(41, 40, 41, 255);
		static ImVec4 inactive = imguipp::to_vec4(31, 30, 31, 255);

		ImGui::PushStyleColor(ImGuiCol_Button, Settings::Tab == 1 ? active : inactive);
		if (ImGui::Button(ICON_FA_CODE " MIPS-32", ImVec2(230 - 15, 41)))
			Settings::Tab = 1;

		ImGui::Spacing();
		ImGui::PushStyleColor(ImGuiCol_Button, Settings::Tab == 2 ? active : inactive);
		if (ImGui::Button(ICON_FA_CODE " NOBLE-16", ImVec2(230 - 15, 41)))
			Settings::Tab = 2;
		

		ImGui::Spacing();
		ImGui::PushStyleColor(ImGuiCol_Button, Settings::Tab == 3 ? active : inactive);
		if (ImGui::Button(ICON_FA_CODE " NOBLE PATH", ImVec2(230 - 15, 41)))
			Settings::Tab = 3;
		ImGui::PopStyleColor(3);

		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 40);
		imguipp::center_text_ex("Calico|MIPS Simulator", 230, 1, false);
	}
	ImGui::NextColumn();
	//Second Column
	{
		
		// Executor tab
		if (Settings::Tab == 1)
		{
			ImGui::SetColumnOffset(2, 810);
			if (ImGui::Button(ICON_FA_CHECK" Execute", ImVec2(135, 34))) {
				Settings::ExecutePressed = !Settings::ExecutePressed;
				
				
			}
			
			if (Settings::ExecutePressed) {
				TextEditor::GetInstance("##MainEditor")->SetReadOnly(true);
				Memory::SetCallingReason(1);
				TextEditor::GetInstance("##COutputEditor")->SetText(
					MIPSLayer::MIPS::ValidateInput(TextEditor::GetInstance("##MainEditor")->GetText(), TextEditor::GetInstance("##DataEditor")->GetText())
					);
				MIPSLayer::MIPS::Execute(-1);
				TextEditor::GetInstance("##OutputEditor")->SetText(string_format("Executed\nPC at 0X%.2X", 0x400000+Memory::GetPC()*4).c_str());
				Settings::ExecutePressed = false;
				
				
			}
			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_BACKWARD" Step", ImVec2(135, 34))) {
				Settings::LineCounter--;
				Memory::SetVirtualPC(0);
				Memory::FreeTextMemory();
				MIPSLayer::MIPS::ResetRegisterUMap();
				Memory::SetPC(0);
				if (Settings::LineCounter == 1) {
					Memory::FreeTextMemory();
					Memory::SetVirtualPC(0);
					TextEditor::GetInstance("##COutputEditor")->SetText(
						MIPSLayer::MIPS::ValidateInput(TextEditor::GetInstance("##MainEditor")->GetText(), TextEditor::GetInstance("##DataEditor")->GetText())
					);			
					Memory::SetPC(0);
					MIPSLayer::MIPS::ResetRegisterUMap();
					Settings::ExecCounter = 0;
				}
				
				MIPSLayer::MIPS::Execute(Settings::LineCounter);
				TextEditor::CurrentRunLine runLine;
				runLine[Memory::GetPC()+1] = "";
				CL_CORE_ERROR("For Marking PC {0} ", Memory::GetPC());
				TextEditor::GetInstance("##MainEditor")->SetCurrentMarkers(runLine);
			}
			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_FORWARD" Step", ImVec2(135, 34))){
				Settings::LineCounter++;
				if (Settings::LineCounter == 1) {
					Memory::FreeTextMemory();
					MIPSLayer::MIPS::ResetRegisterUMap();

					Memory::SetVirtualPC(0);
					TextEditor::GetInstance("##COutputEditor")->SetText(
						MIPSLayer::MIPS::ValidateInput(TextEditor::GetInstance("##MainEditor")->GetText(), TextEditor::GetInstance("##DataEditor")->GetText())
					);
					Memory::SetPC(0);
					MIPSLayer::MIPS::ResetRegisterUMap();
					Settings::ExecCounter = 0;
				}

				TextEditor::CurrentRunLine runLine;
				runLine[Memory::GetPC()+1] = "";
				CL_CORE_ERROR("For Marking PC {0} ", Memory::GetPC());
				TextEditor::GetInstance("##MainEditor")->SetCurrentMarkers(runLine);
				MIPSLayer::MIPS::Execute(Settings::LineCounter);
			}
			ImGui::SameLine();
			
			//ImGui::SetCursorPosX(ImGui::GetCursorPosX() +imguipp::getx()-145); // Set Stop button place
			if (ImGui::Button(ICON_FA_SQUARE" Stop", ImVec2(135, 34))) {
				Settings::ExecutePressed = 0;
				Settings::ExecCounter = 0;
				Settings::LineCounter = 0;
				Memory::SetPC(0);
				TextEditor::GetInstance("##MainEditor")->SetReadOnly(false);
				TextEditor::GetInstance("##OutputEditor")->SetText("");
			}
			//if (ImGui::Button(ICON_FA_FILE" Execute from file", ImVec2(200, 35))) {}

			ImGui::Spacing();
			ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0);
			ImGui::PushFont(Consolas);
			TextEditor::GetInstance("##MainEditor")->Render("##EditorWindow", ImVec2(imguipp::getx(), 700*(0.64f)));
			ImGui::PopFont();
			ImGui::PopStyleVar();
			//ImGui::Spacing();
			
			
			//if (ImGui::Button(ICON_FA_FILE" Execute from file", ImVec2(200, 35))) {}

			//ImGui::Spacing();
			ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0);
			ImGui::PushFont(Consolas);
			TextEditor::GetInstance("##DataEditor")->Render("##EditorWindow2", ImVec2(imguipp::getx(), 700*(0.35f)));
			ImGui::PopFont();
			ImGui::PopStyleVar();
			ImGui::Spacing();
		}

		// Noble Tab
		else if (Settings::Tab == 2)
		{
			if (TextEditor::GetInstance("##NobleMainEditor")->GetText().length() < 8) {
				Settings::AssembleCheck = false;
			}
			else {
				Settings::AssembleCheck = true;
			}
			Settings::ExecuteStatus = NobleLayer::Noble::GetStatus();
			ImGui::SetColumnOffset(2, 630);
			if (!Settings::AssembleCheck)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}
			
			
			if (ImGui::Button(ICON_FA_COGS" Assemble", ImVec2(188, 34))) {
				Settings::AssemblePressed = !Settings::AssemblePressed;
				TextEditor::GetInstance("##NobleOutputEditor")->SetText("");
				Memory::FreeDataMemory();
				TextEditor::GetInstance("##NobleCOutputEditor")->SetText(
					NobleLayer::Noble::ValidateInput(TextEditor::GetInstance("##NobleMainEditor")->GetText(),
						TextEditor::GetInstance("##NobleDataEditor")->GetText(), 3));
			
				NobleLayer::DataMemoryHandler(TextEditor::GetInstance("##NobleDataEditor")->GetText());
				//CL_CORE_TRACE("Result map outside = {}", error1[1]);
				//std::cout << error1[1] << "\n";
				NobleLayer::Noble::ResetOutputBuffer();
				NobleLayer::Noble::CreateInstructionTable();
				
				Settings::LineCounter = 0;
				
			}
			if (!Settings::AssembleCheck)
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}
			ImGui::SameLine();
			if (!Settings::ExecuteStatus)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}
			if (ImGui::Button(ICON_FA_CHECK" Execute", ImVec2(188, 34))) {
				Settings::ExecutePressed = !Settings::ExecutePressed;
			}
			if (!Settings::ExecuteStatus)
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}
			

			if (Settings::ExecutePressed) {
				NobleLayer::Noble::ResetOutputBuffer();
				//TextEditor::GetInstance("##NobleMainEditor")->SetReadOnly(true);
				TextEditor::GetInstance("##NobleCOutputEditor")->SetText(
					NobleLayer::Noble::ValidateInput(TextEditor::GetInstance("##NobleMainEditor")->GetText(),
						TextEditor::GetInstance("##NobleDataEditor")->GetText(),4));
				NobleLayer::Noble::Execute(-1);
				TextEditor::GetInstance("##NobleOutputEditor")->SetText(string_format("%sExecution done.\n",NobleLayer::Noble::GetOutputString().c_str()));
				Settings::ExecutePressed = false;
				Settings::LineCounter = Memory::GetVirtualPC()+1;
				MemoryView::GetInstance("##NobleMainMemoryView")->Unlock();
				RegisterView::GetInstance("##NobleMainRegisterView")->Unlock();

			}
			
			ImGui::Spacing();
			ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0);
			ImGui::PushFont(Consolas);
			TextEditor::GetInstance("##NobleMainEditor")->Render("##EditorWindow", ImVec2(imguipp::getx(), 700 * (0.64f)));
			ImGui::PopFont();
			ImGui::PopStyleVar();
			//ImGui::Spacing();


			//if (ImGui::Button(ICON_FA_FILE" Execute from file", ImVec2(200, 35))) {}

			//ImGui::Spacing();
			ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0);
			ImGui::PushFont(Consolas);
			TextEditor::GetInstance("##NobleDataEditor")->Render("##EditorWindow2", ImVec2(imguipp::getx(), 700 * (0.35f)));
			ImGui::PopFont();
			ImGui::PopStyleVar();
			ImGui::Spacing();
		}
		else if (Settings::Tab == 3) {
		ImGui::SetColumnOffset(2, 1200);
			ImDrawer::SetDrawList(ImGui::GetWindowDrawList());
			ImDrawer::Draw();
			
		}
	}

	ImGui::NextColumn();
	//Third Column
	{
		if (Settings::Tab == 1)
		{	
			Settings::MemoryViewActive = false;
			ImGui::SetColumnOffset(3, 1450);
			if(ImGui::Button("Memory View", ImVec2(155, 34))) {
				Settings::MemoryViewActive = !Settings::MemoryViewActive;
			}
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + imguipp::getx() - 145);
			if (ImGui::Button("Register View", ImVec2(155, 34))) {
				Settings::RegisterViewActive = !Settings::RegisterViewActive;
			}
			//outputWnd.output=editor.GetCurrentLineText();
			//outputWindow.Render("##OutputWindow", ImVec2(imguipp::getx(), 580));
			//std::cout << outputWindow.output << "\n";
			//ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 42);
			ImGui::Spacing();	
			ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0);
			ImGui::PushFont(Consolas);
			TextEditor::GetInstance("##NobleCOutputEditor")->Render("##NobleCOutputWindow", ImVec2(imguipp::getx(),700 * (0.64f)));
			ImGui::PopFont();
			ImGui::PopStyleVar();
			ImGui::Spacing();
			ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0);
			ImGui::PushFont(Consolas);
			TextEditor::GetInstance("##NobleOutputEditor")->Render("##NobleOutputWindow", ImVec2(imguipp::getx(), 700 * (0.345)));
			ImGui::PopFont();
			ImGui::PopStyleVar();
			ImGui::Spacing();
			if (Settings::MemoryViewActive) {
			
				Menu::MemoryViewTheme();
				ImGui::Begin("Calico Memory View", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
				if (Settings::MemoryViewType == 1) {

					ImGui::SetWindowSize(ImVec2(1000, 390)); // Bin
				}
				else {
					ImGui::SetWindowSize(ImVec2(645, 390)); // Arrange Window Size
				}
				Menu::TitleBarMemoryView();
				MemoryView::GetInstance("##MainMemoryView")->Render(
					Settings::MemoryViewType ?  MemoryView::RenderType::Bin : MemoryView::RenderType::Hex,
					Settings::MemoryKind ? MemoryView::MemoryType::Data : MemoryView::MemoryType::Text
				);
				//ImGui::Text("works!");
				
				ImGui::End();
			}
			
			if (Settings::RegisterViewActive) {
				Menu::RegisterViewTheme();
				ImGui::Begin("Calico Register View", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
				Menu::TitleBarRegisterView();
				RegisterView::GetInstance("##MainRegisterView")->Render();
				ImGui::End();
			}
		}
		if (Settings::Tab == 2)
		{
			ImGui::SetColumnOffset(3, 1030);
			if (!Settings::ExecuteStatus || !Settings::StepBackwardStatus)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

			}
			if (ImGui::Button(ICON_FA_BACKWARD" Step", ImVec2(122.5, 34))) {
				Settings::LineCounter--;
				Memory::SetVirtualPC(0);
				NobleLayer::Noble::ResetRegisterUMap();
				Memory::FreeDataMemory();
				Memory::SetPC(0);
				if (Settings::LineCounter == 1) {
					Memory::FreeTextMemory();
					Memory::SetVirtualPC(0);
					TextEditor::GetInstance("##NobleCOutputEditor")->SetText(
						NobleLayer::Noble::ValidateInput(TextEditor::GetInstance("##NobleMainEditor")->GetText(),
							TextEditor::GetInstance("##NobleDataEditor")->GetText(),4)
					);
					Memory::SetPC(0);
					NobleLayer::Noble::ResetRegisterUMap();
					Settings::ExecCounter = 0;
					
				}

				NobleLayer::Noble::Execute(Settings::LineCounter);
				TextEditor::CurrentRunLine runLine;
				runLine[Memory::GetPC()] = "";
				CL_CORE_ERROR("For Marking PC {0} ", Memory::GetPC());
				TextEditor::GetInstance("##NobleMainEditor")->SetCurrentMarkers(runLine);
				TextEditor::GetInstance("##NobleOutputEditor")->SetText(NobleLayer::Noble::GetOutputString().c_str());
				CL_CORE_INFO("Noble Output str {0}", NobleLayer::Noble::GetOutputString().c_str());
			}
			if (!Settings::ExecuteStatus || !Settings::StepBackwardStatus)
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}
			ImGui::SameLine();
			//CL_CORE_INFO ("Exec Table Size : {0} ",NobleLayer::Noble::GetExecutionTable().size());
			if (!Settings::ExecuteStatus || !Settings::StepForwardStatus )
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				
			}
			if (ImGui::Button(ICON_FA_FORWARD" Step", ImVec2(122.5, 34))) {
				Settings::LineCounter++;
				if (Settings::LineCounter == 1) {
					NobleLayer::Noble::ResetRegisterUMap();
					Memory::SetVirtualPC(0);
					TextEditor::GetInstance("##NobleCOutputEditor")->SetText(
						NobleLayer::Noble::ValidateInput(
							TextEditor::GetInstance("##NobleMainEditor")->GetText(),
							TextEditor::GetInstance("##NobleDataEditor")->GetText(),4)); // Read only ins mem
					Memory::SetPC(0);
					NobleLayer::Noble::ResetRegisterUMap();
					Settings::ExecCounter = 0;
				}

				TextEditor::CurrentRunLine runLine;
				runLine[Memory::GetPC() + 1] = "";
				CL_CORE_ERROR("For LineCounter {0} ", Settings::LineCounter);
				TextEditor::GetInstance("##NobleMainEditor")->SetCurrentMarkers(runLine);
				NobleLayer::Noble::Execute(Settings::LineCounter);
				TextEditor::GetInstance("##NobleOutputEditor")->SetText(NobleLayer::Noble::GetOutputString().c_str());
				CL_CORE_INFO("Noble Output str {0}", NobleLayer::Noble::GetOutputString().c_str());
			}
			if (!Settings::ExecuteStatus || !Settings::StepForwardStatus)
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}

			
			if (Settings::LineCounter <= Memory::GetVirtualPC()) {
				Settings::StepForwardStatus = true;
				
			}
			else {
				Settings::StepForwardStatus = false;
				NobleLayer::Noble::SetActiveInsMem(-1);
			}

			if (Settings::LineCounter != 0) {
				Settings::StepBackwardStatus = true;
			}
			else {
				Settings::StepBackwardStatus = false;
			
			}
			ImGui::SameLine();
			if (!Settings::ExecuteStatus || !Settings::ResetStatus)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

			}
			if (ImGui::Button(ICON_FA_HISTORY" Rewind", ImVec2(122.5, 34))) {
				Settings::ExecutePressed = 0;
				Settings::ExecCounter = 0;
				Settings::LineCounter = 0;
				NobleLayer::Noble::ResetRegisterUMap();
				Memory::FreeDataMemory();
				NobleLayer::DataMemoryHandler(TextEditor::GetInstance("##NobleDataEditor")->GetText());
				Memory::SetPC(0);
				TextEditor::GetInstance("##NobleMainEditor")->SetReadOnly(false);
				TextEditor::GetInstance("##NobleOutputEditor")->SetText("");
				TextEditor::CurrentRunLine runLine;
				//runLine[Memory::GetPC() + 1] = "";
				CL_CORE_ERROR("For LineCounter {0} ", Settings::LineCounter);
				TextEditor::GetInstance("##NobleMainEditor")->SetCurrentMarkers(runLine);
				NobleLayer::Noble::ResetOutputBuffer();
				NobleLayer::Noble::SetEditorLine(0);
			}
			if (!Settings::ExecuteStatus || !Settings::ResetStatus)
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}

			if (Settings::LineCounter > 0) {
				Settings::ResetStatus = true;
			}
			else {
				Settings::ResetStatus = false;
			}
			//ImGui::SetCursorPosX(ImGui::GetCursorPosX() +imguipp::getx()-145); // Set Stop button place
			/*if (ImGui::Button(ICON_FA_SQUARE" Stop", ImVec2(188, 34))) {
				Settings::ExecutePressed = 0;
				Settings::ExecCounter = 0;
				Settings::LineCounter = 0;
				Memory::SetPC(0);
				TextEditor::GetInstance("##NobleMainEditor")->SetReadOnly(false);
				TextEditor::GetInstance("##NobleOutputEditor")->SetText("");
			}*/
			/*if (ImGui::Button("Memory View", ImVec2(145, 34))) {
				Settings::MemoryViewActive = !Settings::MemoryViewActive;
			}
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + imguipp::getx() - 145);
			if (ImGui::Button("Register View", ImVec2(145, 34))) {
				Settings::RegisterViewActive = !Settings::RegisterViewActive;
			}*/

			ImGui::Spacing();
			ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0);
			ImGui::PushFont(Consolas);
			TextEditor::GetInstance("##NobleCOutputEditor")->Render("##NobleCOutputWindow", ImVec2(imguipp::getx(), 700 * (0.64f)));
			ImGui::PopFont();
			ImGui::PopStyleVar();
			ImGui::Spacing();
			ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0);
			ImGui::PushFont(Consolas);
			TextEditor::GetInstance("##NobleOutputEditor")->Render("##NobleOutputWindow", ImVec2(imguipp::getx(), 700 * (0.345)));
			ImGui::PopFont();
			ImGui::PopStyleVar();
			ImGui::Spacing();


		}
		if (Settings::Tab == 3) {
			
			float temp = Consolas->Scale;
			Consolas->Scale = 0.7;
			ImGui::PushFont(Consolas);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0, 0)));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(41, 40, 41, 255)));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(41, 40, 41, 255)));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0, 0.0, 0.0, 0));
			
				if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 46 * ImGui::GetTextLineHeightWithSpacing())))
				{

					if (ImGui::Button("Noble Path", ImVec2(180, 20))) {
						ImDrawer::SetCurrentScene(ImDrawer::Scene::ImDrawer_All);
						ImDrawer::SetCurrentSceneHeader(std::string("General Path"));
					}
					if (!NobleLayer::Noble::GetInstructionTable().empty()) {

						for (const auto& it : NobleLayer::Noble::GetInstructionTable()) {
							if (ImGui::Button(it.m_InputLine.c_str(), ImVec2(180, 20))) {
								CL_CORE_ERROR("HERE!");
								ImDrawer::ActivatePath(std::string(it.m_Instruction));
								ImDrawer::SetCurrentScene(ImDrawer::Scene::ImDrawer_Path);
								ImDrawer::SetCurrentSceneHeader(std::string (it.m_Instruction));
							}
						}
					}
					ImGui::EndListBox();
				}
			
			Consolas->Scale = temp;
			ImGui::PopStyleColor(4);
			ImGui::PopFont();
			
		}
	}
	ImGui::NextColumn();
	//Forth Column
	{
		if (Settings::Tab == 2) {

			const char* viewType[2] = { "HEX", "BIN" };
			const char* memoryTypes[2] = { "INS MEMORY","DATA MEMORY" };
			
			if (!Settings::ExecuteStatus)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}
			if (ImGui::Button(ICON_FA_MICROCHIP" Xilinx Vivado Simulate", ImVec2(370, 34))) {
				std::string iDataMemPathStr = "C:\\Users\\hayre\\OneDrive\\Masaüstü\\Noble-16\\Noble-16.sim\\sim_1\\behav\\xsim\\test\\input\\" + std::string(Settings::SimulationName) + "_idmem.inoble";
				std::ofstream MyFile2(iDataMemPathStr);
				for (auto& it : Memory::GetDataMemory()) {

					MyFile2 << std::bitset<16>(it.second.second) << "\n";
				}

				MyFile2.close();
				int tCounter = 0;
				{
					MemoryView::GetInstance("##NobleMainMemoryView")->Lock();
					RegisterView::GetInstance("##NobleMainRegisterView")->Lock();
					TextEditor::GetInstance("##NobleCOutputEditor")->SetText(
							NobleLayer::Noble::ValidateInput(TextEditor::GetInstance("##NobleMainEditor")->GetText(),
							TextEditor::GetInstance("##NobleDataEditor")->GetText(), 4));
					NobleLayer::Noble::Execute(-1);
				}
				CL_CORE_ERROR("Total Cycle! {0}", ((NobleLayer::Noble::GetTotalCycle()+1)*10)+5);
				std::ofstream SpecsFile("C:\\Users\\hayre\\OneDrive\\Masaüstü\\Noble-16\\Noble-16.srcs\\sim_1\\new\\testspecs.v");
				SpecsFile << "`ifndef TESTSPECS_H_" << "\n" << "`define TESTSPECS_H_" << "\n";
				SpecsFile << "`define simulation_time #" << ((NobleLayer::Noble::GetTotalCycle() + 1) * 10) + 5 << "\n";
				SpecsFile << "`endif";
				SpecsFile.close();
				
				std::ofstream SpecsFile2("C:\\Users\\hayre\\OneDrive\\Masaüstü\\Noble-16\\Noble-16.srcs\\sources_1\\new\\testspecs_1.v");
				SpecsFile2 << "`ifndef TESTSPECS_1_H_" << "\n" << "`define TESTSPECS_1_H_" << "\n";
				SpecsFile2 << "`define inputFile \"" << Settings::InputAsmPath<< "\"\n";
				SpecsFile2 << "`define inputFileDataMem \"" <<Settings::InputInitialMem << "\"\n";
				SpecsFile2 << "`define outputFileMem \"" << Settings::OutputDataMem<< "\"\n";
				SpecsFile2 << "`define outputFileReg \"" << Settings::OutputRegFile<< "\"\n";
				SpecsFile2 << "`endif";
				SpecsFile2.close();
				std::string inputAsmPathStr = "C:\\Users\\hayre\\OneDrive\\Masaüstü\\Noble-16\\Noble-16.sim\\sim_1\\behav\\xsim\\test\\input\\" + std::string (Settings::SimulationName) + ".noble";
				std::ofstream MyFile(inputAsmPathStr);
				for (auto& it : Memory::GetTextMemory()) {
					CL_CORE_INFO("{0}", std::bitset<16>(it));
					MyFile << std::bitset<16>(it) << "\n";
				}
				MyFile << std::bitset<16>(-11967);
				MyFile.close();
				SimulationAction();
			}
			if (!Settings::ExecuteStatus)
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}
			if (Settings::ViewTypeCounter == 1) {
				//ImDrawer::Draw();
			}
			if (Settings::ViewTypeCounter == 0) {
				Settings::MemoryViewActive = true;
			}
			
			ImGui::Spacing();
			if (ImGui::Button(memoryTypes[Settings::MemoryKind], ImVec2(181, 34))) {
				Settings::MemoryKind = (Settings::MemoryKind + 1) % 2;
				CL_CORE_INFO("Button press {0}", Settings::MemoryKind);
			}


			
			ImGui::SameLine();
			if (ImGui::Button(string_format("MemoryView [%s]", viewType[Settings::MemoryViewType]).c_str(), ImVec2(181, 34))) {
				Settings::MemoryViewType = (Settings::MemoryViewType + 1) % 2;
			}


			
			if (Settings::MemoryViewActive) {

				MemoryView::GetInstance("##NobleMainMemoryView")->Render16Bit(
					Settings::MemoryViewType ? MemoryView::RenderType::Bin : MemoryView::RenderType::Hex,
					Settings::MemoryKind ? MemoryView::MemoryType::Data : MemoryView::MemoryType::Text
				);
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0, 0)));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1, 0.3, 0.2, 1));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1, 0.3, 0.2, 1));
				float temp = Consolas->Scale;
				Consolas->Scale = 0.80;
				ImGui::PushFont(Consolas);
				if (ImGui::Button("REGISTERS", ImVec2(370, 22))) {
					NobleLayer::Noble::ResetRegisterUMap();
					//Settings::MemoryViewType = (Settings::MemoryViewType + 1) % 2;
				}
				Consolas->Scale = temp;
				ImGui::PopFont();
				ImGui::PopStyleColor(3);
				RegisterView::GetInstance("##NobleMainRegisterView")->Render16Bit();
			}

		}
		
	}
}

void Menu::Theme()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	// to do create spec class 
	float aspectRatio = 16 /9.f;
	float width = 1400.f;
	float height = width / aspectRatio;
	//std::cout << height << "\n";
	style->WindowBorderSize = 0;
	style->WindowTitleAlign = ImVec2(0.5, 0.5);
	style->WindowMinSize = ImVec2(width,height);
	style->FramePadding = ImVec2(8, 6);

	style->Colors[ImGuiCol_TitleBg] = ImColor(107, 3, 252, 255);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(54, 194, 164, 125);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(34, 16, 54, 130);

	style->Colors[ImGuiCol_Button] = ImColor(31, 30, 31, 255);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(41, 40, 41, 255);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(41, 40, 41, 255);

	style->Colors[ImGuiCol_Separator] = ImColor(70, 70, 70, 255);
	style->Colors[ImGuiCol_SeparatorActive] = ImColor(76, 76, 76, 255);
	style->Colors[ImGuiCol_SeparatorHovered] = ImColor(76, 76, 76, 255);

	style->Colors[ImGuiCol_FrameBg] = ImColor(25, 94, 56, 255);
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(25, 94, 56, 255);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(25, 94, 56, 255);

	style->Colors[ImGuiCol_Header] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(25, 94, 56, 255);

	style->Colors[ImGuiCol_MenuBarBg] = ImColor(34, 16, 54, 130);
}

void Menu::MemoryViewTheme()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	// to do create spec class 
	float aspectRatio = 16 / 9.f;
	float width =  645.f;
	float height = 390;
	//CL_CORE_INFO("W = {0} H = {1}", width, height);
	
	style->WindowBorderSize = 0;
	style->WindowTitleAlign = ImVec2(0.5, 0.5);
	style->WindowMinSize = ImVec2(width, height);
	style->FramePadding = ImVec2(8, 6);
	
	style->Colors[ImGuiCol_TitleBg] = ImColor(107, 3, 252, 255);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(54, 194, 164, 125);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(34, 16, 54, 130);

	style->Colors[ImGuiCol_Button] = ImColor(31, 30, 31, 255);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(41, 40, 41, 255);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(41, 40, 41, 255);

	style->Colors[ImGuiCol_Separator] = ImColor(70, 70, 70, 255);
	style->Colors[ImGuiCol_SeparatorActive] = ImColor(76, 76, 76, 255);
	style->Colors[ImGuiCol_SeparatorHovered] = ImColor(76, 76, 76, 255);

	style->Colors[ImGuiCol_FrameBg] = ImColor(25, 94, 56, 255);
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(25, 94, 56, 255);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(25, 94, 56, 255);

	style->Colors[ImGuiCol_Header] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(25, 94, 56, 255);

	style->Colors[ImGuiCol_MenuBarBg] = ImColor(34, 16, 54, 130);
}

void Menu::RegisterViewTheme()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	// to do create spec class 
	float aspectRatio = 16 / 12.f;
	float width = 630.f;
	float height = width / aspectRatio;
	//std::cout << height << "\n";
	style->WindowBorderSize = 0;
	style->WindowTitleAlign = ImVec2(0.5, 0.5);
	style->WindowMinSize = ImVec2(width, height);
	style->FramePadding = ImVec2(8, 6);

	style->Colors[ImGuiCol_TitleBg] = ImColor(107, 3, 252, 255);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(54, 194, 164, 125);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(34, 16, 54, 130);

	style->Colors[ImGuiCol_Button] = ImColor(31, 30, 31, 255);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(41, 40, 41, 255);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(41, 40, 41, 255);

	style->Colors[ImGuiCol_Separator] = ImColor(70, 70, 70, 255);
	style->Colors[ImGuiCol_SeparatorActive] = ImColor(76, 76, 76, 255);
	style->Colors[ImGuiCol_SeparatorHovered] = ImColor(76, 76, 76, 255);

	style->Colors[ImGuiCol_FrameBg] = ImColor(25, 94, 56, 255);
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(25, 94, 56, 255);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(25, 94, 56, 255);

	style->Colors[ImGuiCol_Header] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(25, 94, 56, 255);

	style->Colors[ImGuiCol_MenuBarBg] = ImColor(34, 16, 54, 130);
}


void Menu::TitleBar(MSG& msg) {
	if (ImGui::BeginMenuBar())
	{
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(ImColor(34, 16, 54, 130)));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(ImColor(34, 16, 54, 130)));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(ImColor(34, 16, 54, 130)));
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::BeginMenu("Simulation Options"))
			{
				static int n = 0;
				ImGui::Combo("Platform", &n, "Xilinx Vivado\0None\0");
				
				ImGui::InputText("Simulation Name", Settings::SimulationName, IM_ARRAYSIZE(Settings::SimulationName));
				if (ImGui::BeginMenu("Input Paths")) {
					sprintf(Settings::InputAsmPath,"./test/input/%s.noble", Settings::SimulationName);
					ImGui::InputText("ASM Binary", Settings::InputAsmPath, IM_ARRAYSIZE(Settings::InputAsmPath));
					sprintf(Settings::InputInitialMem, "./test/input/%s_idmem.inoble", Settings::SimulationName);
					ImGui::InputText("Initial Data Mem", Settings::InputInitialMem, IM_ARRAYSIZE(Settings::InputInitialMem));
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Output Paths")) {
					sprintf(Settings::OutputRegFile, "./test/output/%s_regfile.nro", Settings::SimulationName);
					ImGui::InputText("Register Output", Settings::OutputRegFile, IM_ARRAYSIZE(Settings::OutputRegFile));
					sprintf(Settings::OutputDataMem, "./test/output/%s_datamem.nmo", Settings::SimulationName);
					ImGui::InputText("Data Mem Output", Settings::OutputDataMem, IM_ARRAYSIZE(Settings::OutputDataMem));
					ImGui::EndMenu();
				}
				
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::PopStyleColor(3);
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 30);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(34, 16, 54, 130)));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(107, 3, 252, 255))); 
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(107, 3, 252, 255)));

		if (ImGui::Button(ICON_FA_SIGN_OUT" ", ImVec2(0, 0))) {
			msg.message = WM_QUIT;
		}
		
		ImGui::PopStyleColor(3);
		ImGui::SetCursorPosX(ImGui::GetWindowWidth()/2);
		ImGui::Text(Settings::TabNames[Settings::Tab], ImVec2(0, 0));
		
		ImGui::EndMenuBar();
	}
}
void Menu::TitleBarMemoryView() {
	const char* viewType[2] = { "HEX", "BIN" };
	const char* memoryTypes[2] = { "TEXT MEMORY","DATA MEMORY" };
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::Button(memoryTypes[Settings::MemoryKind], ImVec2(0, 0))) {
			Settings::MemoryKind = (Settings::MemoryKind + 1) % 2;
			CL_CORE_INFO("Button press {0}", Settings::MemoryKind);
		}
		
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 30);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.3, 0.2, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1, 0.3, 0.2, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1, 0.3, 0.2, 1));

		if (ImGui::Button(ICON_FA_SIGN_OUT" ", ImVec2(0, 0))) {
			Settings::MemoryViewActive = false;
		}

		ImGui::PopStyleColor(3);
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() / 2)- 80);
		if (ImGui::Button(string_format("MemoryView [%s]",viewType[Settings::MemoryViewType]).c_str(), ImVec2(0, 0))) {
			Settings::MemoryViewType = (Settings::MemoryViewType+1) %2;
		}
			
		
		ImGui::EndMenuBar();
	}
}

void Menu::TitleBarRegisterView() {
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			ImGui::EndMenu();
		}
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 30);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.3, 0.2, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1, 0.3, 0.2, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1, 0.3, 0.2, 1));

		if (ImGui::Button(ICON_FA_SIGN_OUT" ", ImVec2(0, 0))) {
			Settings::RegisterViewActive = false;
		}

		ImGui::PopStyleColor(3);
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() / 2)-50);
		ImGui::Text(" Register View", ImVec2(0, 0));

		
		ImGui::EndMenuBar();
	}
}

void Menu::SimulationAction()
{
	if (Settings::InputAsmPath[0] != '\0' && Settings::InputInitialMem[0] != '\0' && Settings::OutputDataMem[0] != '\0' && Settings::OutputRegFile[0] != '\0') {
		std::string SimulationStr = string_format("Simulation configurations\n \"%s\" created!", Settings::SimulationName);
		TextEditor::GetInstance("##NobleOutputEditor")->SetText(SimulationStr);
		CL_CORE_INFO("{0}", Settings::InputAsmPath);
	}
	else {
		TextEditor::GetInstance("##NobleOutputEditor")->SetText("Configuration Error!");
	}
	

}