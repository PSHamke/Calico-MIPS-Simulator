#include "OutputWin.h"

void OutputWindow::Render(const char* aTitle, const ImVec2& aSize, bool aBorder) {
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.266f, 0.211f, 0.211f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::BeginChild(aTitle, aSize, aBorder, ImGuiWindowFlags_HorizontalScrollbar |ImGuiWindowFlags_NoMove);
	ImGui::Text("Output");
	ImGui::Text((this->output).c_str());
	ImGui::EndChild();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}