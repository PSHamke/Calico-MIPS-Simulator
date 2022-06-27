#include "InstructionPath.h"
void InstructionPath::Init()
{
	// create new path from ref path 
	std::string delimiter = ",";
	std::string tempStr = m_PathStr;
	std::string pathElem;
	PathCircle* tempCircle;
	PathPolyLine* tempPolyLine;
	PathEllipse* tempEllipse;
	PathRect* tempRect;
	PathText* tempText;
	PathTriangle* tempTriangle;
	PathHalfEllipse* tempHalfEllipse;
	size_t pos = 0;
	Sync();
	while ((pos = tempStr.find(delimiter)) != std::string::npos) { // inspect pathstr taken from csv 
		pathElem = tempStr.substr(2, pos - 2);

		switch (stoi(tempStr.substr(0, 1))) {
		case 1:
			tempPolyLine = m_PathPtrTemp->GetPathPolyLine(pathElem);
			if (tempPolyLine) {
				m_PathPolyLines.push_back(tempPolyLine);
			}
			else {
				CL_CORE_ERROR("Poly not found ! {0}", pathElem);
			}
				
			break;
			
		case 2:
			tempRect = m_PathPtrTemp->GetPathRect(pathElem);
			if (tempRect) {
				CL_CORE_INFO("Name {0} - Rect Name{1}", m_Name, tempRect->m_Name);
				m_PathRects.push_back(tempRect);
			}
			else {
				CL_CORE_ERROR("Rect not found ! {0}", pathElem);
			}
			break;
			
		case 3:
			tempTriangle = m_PathPtrTemp->GetPathTriangle(pathElem);
			if (tempTriangle) {
				m_PathTriangles.push_back(tempTriangle);
			}else {
				CL_CORE_ERROR("Tri not found ! {0}", pathElem);
			}
				
			break;
		case 4:
			tempText = m_PathPtrTemp->GetPathText(pathElem);
			if (tempText) {

				m_PathTexts.push_back(tempText);
			}
			else {
				CL_CORE_ERROR("Text not found ! {0}", pathElem);
			}
			break;
		case 5:
			tempEllipse = m_PathPtrTemp->GetPathEllipse(pathElem);
			if (tempEllipse) {
				m_PathEllipses.push_back(tempEllipse);
			}
			else {
				CL_CORE_ERROR("Ellipse not found ! {0}", pathElem);
			}
			break;
		case 6:
			tempCircle = m_PathPtrTemp->GetPathCircle(pathElem);
			if (tempCircle) {
				m_PathCircles.push_back(tempCircle);
			}
			else {
				CL_CORE_ERROR("Circle not found ! {0}", pathElem);
			}
				
			break;
		case 7:
			tempHalfEllipse = m_PathPtrTemp->GetPathHalfEllipse(pathElem);
			if (tempHalfEllipse) {
				m_PathHalfEllipses.push_back(tempHalfEllipse);
			}
			else {
				CL_CORE_ERROR("Circle not found ! {0}", pathElem);
			}
			break;
		default:
			CL_CORE_WARN("No case!!");
			break;
		}


		tempStr.erase(0, pos + delimiter.length());

	}
	
}
void InstructionPath::Activate() {

	Sync();
	CL_CORE_INFO("Activate! {}", m_Name);
	for (auto& it : m_PathPtrTemp->GetPathCircles()) {
		it.m_Color = IM_COL32(0x45, 0x45, 0x45, 0xff);
	}
	for (auto& it : m_PathPtrTemp->GetPathPolyLines()) {
		it.m_Color = IM_COL32(0x45, 0x45, 0x45, 0xff);
	}
	for (auto& it : m_PathPtrTemp->GetPathTriangles()) {
		it.m_Color = IM_COL32(0x45, 0x45, 0x45, 0xff);
	}
	for (auto& it : m_PathPtrTemp->GetPathRects()) {
		it.m_Color = IM_COL32(0x45, 0x45, 0x45, 0xff);
	}
	for (auto& it : m_PathPtrTemp->GetPathEllipses()) {
		it.m_Color = IM_COL32(0x45, 0x45, 0x45, 0xff);
	}
	for (auto& it : m_PathPtrTemp->GetPathTexts()) {
		it.m_Color = IM_COL32(0x45, 0x45, 0x45, 0xff);
	}
	for (auto& it : m_PathPtrTemp->GetPathHalfEllipses()) {
		it.m_Color = IM_COL32(0x45, 0x45, 0x45, 0xff);
	}
	for (auto& it : m_PathPolyLines) {
		//CL_CORE_ERROR("Props {0}, {1}, {2}", it->m_Color, it->m_Name, it->m_Thickness);
		it->m_Color = IM_COL32(0xff, 0x5b, 0xfc, 0xff);
		it->m_Thickness = 2.0f;
	}
	for (auto& it : m_PathRects) {
		CL_CORE_ERROR("Props {0}, {1}, {2}", it->m_Color, it->m_Name, it->m_Thickness);
		it->m_Color = IM_COL32(0xff, 0x5b, 0xfc, 0xff);
		it->m_Thickness = 2.0f;
	}
	for (auto& it : m_PathTriangles) {
		//CL_CORE_ERROR("Props {0}, {1}, {2}", it->m_Color, it->m_Name, it->m_Thickness);
		it->m_Color = IM_COL32(0xff, 0x5b, 0xfc, 0xff);
		it->m_Thickness = 2.0f;
	}
	for (auto& it : m_PathCircles) {
		//CL_CORE_ERROR("Props {0}, {1}, {2}", it->m_Color, it->m_Name, it->m_Thickness);
		it->m_Color = IM_COL32(0xff, 0x5b, 0xfc, 0xff);
		it->m_Thickness = 2.0f;
	}
	for (auto& it : m_PathTexts) {
		//CL_CORE_ERROR("Props {0}, {1}, {2}", it->m_Color, it->m_Name, it->m_FontSize);
		it->m_Color = IM_COL32(0xff, 0x5b, 0xfc, 0xff);
		
	}
	for (auto& it : m_PathEllipses) {
		//CL_CORE_ERROR("Props {0}, {1}, {2}", it->m_Color, it->m_Name);
		it->m_Color = IM_COL32(0xff, 0x5b, 0xfc, 0xff);
		it->m_Thickness = 2.0f;

	}
	for (auto& it : m_PathHalfEllipses) {
		//CL_CORE_ERROR("Props {0}, {1}, {2}", it->m_Color, it->m_Name);
		it->m_Color = IM_COL32(0xff, 0x5b, 0xfc, 0xff);
		it->m_Thickness = 2.0f;

	}
}

void InstructionPath::Sync() {
	m_PathPtrTemp->SetPathCircles(m_PathPtrOrig->GetPathCircles());
	m_PathPtrTemp->SetPathEllipses(m_PathPtrOrig->GetPathEllipses());
	m_PathPtrTemp->SetPathTexts(m_PathPtrOrig->GetPathTexts());
	m_PathPtrTemp->SetPathTriangles(m_PathPtrOrig->GetPathTriangles());
	m_PathPtrTemp->SetPathHalfEllipses(m_PathPtrOrig->GetPathHalfEllipses());
	m_PathPtrTemp->SetPathPolyLines(m_PathPtrOrig->GetPathPolyLines());
	m_PathPtrTemp->SetPathRects(m_PathPtrOrig->GetPathRects());
}