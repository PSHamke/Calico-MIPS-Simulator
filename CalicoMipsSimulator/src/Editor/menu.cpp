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
extern ImFont* Consolas;

void mWorker() {
	
}
void Menu::Render()
{
	ImGui::Columns(3);
	ImGui::SetColumnOffset(1, 230);
	
	{
		//Left side
		static ImVec4 active = imguipp::to_vec4(41, 40, 41, 255);
		static ImVec4 inactive = imguipp::to_vec4(31, 30, 31, 255);

		ImGui::PushStyleColor(ImGuiCol_Button, Settings::Tab == 1 ? active : inactive);
		if (ImGui::Button(ICON_FA_CODE " Executor", ImVec2(230 - 15, 41)))
			Settings::Tab = 1;

		ImGui::Spacing();
		ImGui::PushStyleColor(ImGuiCol_Button, Settings::Tab == 2 ? active : inactive);
		if (ImGui::Button(ICON_FA_DOWNLOAD " Settings", ImVec2(230 - 15, 41)))
			Settings::Tab = 2;
		ImGui::PopStyleColor(2);

		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 40);
		imguipp::center_text_ex("Calico|MIPS Simulator", 230, 1, false);
	}
	ImGui::NextColumn();
	ImGui::SetColumnOffset(2, 765);
	//Right side
	{
		
		// Executor tab
		if (Settings::Tab == 1)
		{
			
			if (ImGui::Button(ICON_FA_CHECK" Execute", ImVec2(115, 34))) {
				Settings::ExecutePressed = !Settings::ExecutePressed;
				//Memory::FreeTextMemory();
				MIPSLayer::MIPS::ResetRegisterUMap();
				Memory::SetPC(0);
				TextEditor::GetInstance("##MainEditor")->SetReadOnly(true);
				Memory::SetVirtualPC(0);
				Memory::SetCallingReason(1);
				//MIPSLayer::MIPS::Execute(TextEditor::GetInstance("##MainEditor")->GetText(), TextEditor::GetInstance("##DataEditor")->GetText());
				CL_CORE_INFO("Called!");
			}
			
			if (Settings::ExecutePressed) {
				unsigned int PC = 0;

				
				std::vector<std::pair<std::string, int>> Lines;
				std::string delimiter = "\n";
				std::string aData = TextEditor::GetInstance("##MainEditor")->GetText();
				TextEditor::CurrentRunLine mCur;
				int lineCounter = 0;
				size_t pos = 0;
				std::string temp;
				std::string PCstring = "";
				while ((pos = aData.find(delimiter)) != std::string::npos) {
					temp = trim(aData.substr(0, pos));
					if (temp != "")
						Lines.push_back(std::make_pair(temp, lineCounter++));
					aData.erase(0, pos + delimiter.length());
				}
				if (trim(aData) != "") {
					Lines.push_back(std::make_pair(trim(aData.substr(0, pos)), lineCounter++));
				}

				
				Lines.erase(Lines.begin()); // sure that its a segment identifier
				//TextEditor::GetInstance("##MainEdiyor");

				int labelCounter = 0;
				while (Memory::GetPC() < Lines.size()) {
					bool labelCheck = false;
					Settings::ExecCounter++;
					CL_CORE_INFO("Counter = {0}, Linecounter = {1}, Mem = {2}", Settings::ExecCounter, Settings::LineCounter, Memory::GetPC());
					mCur.clear();
					MIPSLayer::MIPS::TranslateToC(Lines[Memory::GetPC()].first, TextEditor::GetInstance("##DataEditor")->GetText(), 0,labelCheck);
					if (labelCheck) {
						labelCounter++;
						Lines.erase(Lines.begin() + Memory::GetPC());
					}
					mCur[Memory::GetPC() + 1 + labelCounter] = "sdfsd";
					TextEditor::GetInstance("##MainEditor")->SetCurrentMarkers(mCur);
					PCstring= string_format("Executed Successfully!\nPC at 0X%X", 0x400000+(Memory::GetPC())*4);
					TextEditor::GetInstance("##OutputEditor")->SetText(PCstring.c_str());
					//Memory::SetPC(Memory::GetPC() - 1);
				}		
				Settings::ExecutePressed = false;
			}
			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_SQUARE" Step", ImVec2(115, 34))) {
				Settings::LineCounter++;
				if (Settings::LineCounter == 1) {
					Memory::FreeTextMemory();
					MIPSLayer::MIPS::ResetRegisterUMap();
					Memory::SetPC(0);
					Settings::ExecCounter = 0;
				}
			}
			if (Settings::LineCounter > 0 && Settings::LineCounter >= Settings::ExecCounter) {
				CL_CORE_INFO("Counter = {0}, Linecounter = {1}, Mem = {2}", Settings::ExecCounter, Settings::LineCounter, Memory::GetPC());
				std::vector<std::pair<std::string, int>> Lines;
				std::string delimiter = "\n";
				std::string aData = TextEditor::GetInstance("##MainEditor")->GetText();
				TextEditor::CurrentRunLine mCur;
				int lineCounter = 0;
				size_t pos = 0;
				std::string temp;
				while ((pos = aData.find(delimiter)) != std::string::npos) {
					temp = trim(aData.substr(0, pos));
					if (temp != "")
						Lines.push_back(std::make_pair(temp, lineCounter++));
					aData.erase(0, pos + delimiter.length());
				}
				if (trim(aData) != "") {
					Lines.push_back(std::make_pair(trim(aData.substr(0, pos)), lineCounter++));
				}


				Lines.erase(Lines.begin()); // sure that its a segment identifier
				
				while (Memory::GetPC() < Lines.size()) {
					Settings::ExecCounter++;
					bool check = false;
					mCur.clear();
					MIPSLayer::MIPS::TranslateToC(Lines[Memory::GetPC()].first, TextEditor::GetInstance("##DataEditor")->GetText(), 0, check);
					if(check)
						Lines.erase(Lines.begin()+ Memory::GetPC());
					mCur[Memory::GetPC() + 1] = "sdfsd";
					TextEditor::GetInstance("##MainEditor")->SetCurrentMarkers(mCur);
					TextEditor::GetInstance("##OutputEditor")->SetText("Step Successfully!");
					//Memory::SetPC(Memory::GetPC() - 1);
				}
			}



			
			ImGui::SameLine();

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() +imguipp::getx()-145); // Set Stop button place
			if (ImGui::Button(ICON_FA_SQUARE" Stop", ImVec2(115, 34))) {
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
			TextEditor::GetInstance("##MainEditor")->Render("##EditorWindow", ImVec2(imguipp::getx(), 640*(0.64f)));
			ImGui::PopFont();
			ImGui::PopStyleVar();
			//ImGui::Spacing();
			
			
			//if (ImGui::Button(ICON_FA_FILE" Execute from file", ImVec2(200, 35))) {}

			//ImGui::Spacing();
			ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0);
			ImGui::PushFont(Consolas);
			TextEditor::GetInstance("##DataEditor")->Render("##EditorWindow2", ImVec2(imguipp::getx(), 640*(0.35f)));
			ImGui::PopFont();
			ImGui::PopStyleVar();
			ImGui::Spacing();
		}

		// Dumper Tab
		else if (Settings::Tab == 2)
		{
			
		}		
	}

	ImGui::NextColumn();
	//ImGui::SetColumnOffset(3, 1300);
	{
		if (Settings::Tab == 1)
		{
			if(ImGui::Button("Memory View", ImVec2(145, 34))) {
				Settings::MemoryViewActive = !Settings::MemoryViewActive;
			}
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + imguipp::getx() - 145);
			if (ImGui::Button("Register View", ImVec2(145, 34))) {
				Settings::RegisterViewActive = !Settings::RegisterViewActive;
			}
			//outputWnd.output=editor.GetCurrentLineText();
			//outputWindow.Render("##OutputWindow", ImVec2(imguipp::getx(), 580));
			//std::cout << outputWindow.output << "\n";
			//ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 42);
			ImGui::Spacing();	
			ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0);
			ImGui::PushFont(Consolas);
			TextEditor::GetInstance("##COutputEditor")->Render("##COutputWindow", ImVec2(imguipp::getx(), 640 * (0.64f)));
			ImGui::PopFont();
			ImGui::PopStyleVar();
			ImGui::Spacing();
			ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0);
			ImGui::PushFont(Consolas);
			TextEditor::GetInstance("##OutputEditor")->Render("##OutputWindow", ImVec2(imguipp::getx(), 640 * (0.345)));
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
	}

}

void Menu::Theme()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	// to do create spec class 
	float aspectRatio = 16 /9.f;
	float width = 1300.f;
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
		if (ImGui::BeginMenu("Menu"))
		{
			ImGui::EndMenu();
		}
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 30);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.3, 0.2, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1, 0.3, 0.2, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1, 0.3, 0.2, 1));

		if (ImGui::Button(ICON_FA_SIGN_OUT" ", ImVec2(0, 0))) {
			msg.message = WM_QUIT;
		}
		
		ImGui::PopStyleColor(3);
		ImGui::SetCursorPosX(ImGui::GetWindowWidth()/2);
		if (ImGui::Button(" CALICO", ImVec2(0, 0))) {
			msg.message = WM_QUIT;
		}
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