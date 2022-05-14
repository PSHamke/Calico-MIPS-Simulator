#include "menu.h"
#include "settings.h"
#include "functions.h"
#include "imguipp.h"
#include "icons.h"

#include "OutputWin.h"
extern ImFont* Consolas;
void Menu::Render(TextEditor& editor, TextEditor& editor2, OutputWindow& outputWindow)
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
	ImGui::SetColumnOffset(2, 720);
	//Right side
	{
		
		// Executor tab
		if (Settings::Tab == 1)
		{
			
			editor.SetShowWhitespaces(false);
			editor.SetReadOnly(false);
			editor.SetPalette(TextEditor::GetDarkPalette());
			editor.SetLanguageDefinition(TextEditor::LanguageDefinition::MIPS());

			editor2.SetShowWhitespaces(false);
			editor2.SetReadOnly(false);
			editor2.SetPalette(TextEditor::GetDarkPalette());
			editor2.SetLanguageDefinition(TextEditor::LanguageDefinition::MIPS());
			
			if (ImGui::Button(ICON_FA_CHECK" Execute", ImVec2(115, 34))) {}
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() +imguipp::getx()-145); // Set Stop button place
			if (ImGui::Button(ICON_FA_SQUARE" Stop", ImVec2(115, 34))) {}
			//if (ImGui::Button(ICON_FA_FILE" Execute from file", ImVec2(200, 35))) {}

			ImGui::Spacing();
			ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0);
			ImGui::PushFont(Consolas);
			editor.Render("##EditorWindow", ImVec2(imguipp::getx(), 580*(0.64f)));
			ImGui::PopFont();
			ImGui::PopStyleVar();
			//ImGui::Spacing();

			
			//if (ImGui::Button(ICON_FA_FILE" Execute from file", ImVec2(200, 35))) {}

			//ImGui::Spacing();
			ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0);
			ImGui::PushFont(Consolas);
			
			editor2.Render("##EditorWindow2", ImVec2(imguipp::getx(), 580*(0.35f)));
			ImGui::PopFont();
			ImGui::PopStyleVar();
			ImGui::Spacing();
		}

		// Dumper Tab
		else if (Settings::Tab == 2)
		{
			static ImGuiTextFilter filter;
			static std::vector<std::string> resources = 
			{
				"_cfx_internal",
				"mysql-async",
				"essentialmode",
				"async",
				"es_extended"
			};

			filter.Draw(ICON_FA_SEARCH" Search", 240);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
			if (ImGui::ListBoxHeader("##ResourcesList", ImVec2(imguipp::getx(), imguipp::gety() - 35)))
			{
				for (const auto& resource : resources)
				{
					if (filter.PassFilter(resource.c_str()))
					{
						if (ImGui::TreeNode(resource.c_str()))
							ImGui::TreePop();				
					}
				}
				ImGui::ListBoxFooter();
			}
			ImGui::PopStyleColor();
			if (ImGui::Button(ICON_FA_FOLDER_OPEN" Save to folder", ImVec2(205, 34))) {}
			
		}		
	}

	ImGui::NextColumn();
	{
		if (Settings::Tab == 1)
		{
			//outputWnd.output=editor.GetCurrentLineText();
			outputWindow.Render("##OutputWindow", ImVec2(imguipp::getx(), 580));
			//std::cout << outputWindow.output << "\n";
			
		}
	}

}

void Menu::Theme()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	// to do create spec class 
	float aspectRatio = 16 /9.f;
	float width = 1200.f;
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
}