#include "ImDrawer.h"
#include "Log.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include <string>
#include <iostream>


void AddTextVertical(ImDrawList* DrawList, const char* text, ImVec2 pos, ImU32 text_color) {
	pos.x = IM_ROUND(pos.x);
	pos.y = IM_ROUND(pos.y);
	ImFont* font = GImGui->Font;
	
	const ImFontGlyph* glyph;
	char c;
	ImGuiContext& g = *GImGui;
	ImVec2 text_size = ImGui::CalcTextSize(text)/2;
	while ((c = *text++)) {
		glyph = font->FindGlyph(c);
		if (!glyph) continue;

		DrawList->PrimReserve(6, 4);
		DrawList->PrimQuadUV(
			pos + ImVec2(glyph->Y0, -glyph->X0),
			pos + ImVec2(glyph->Y0, -glyph->X1),
			pos + ImVec2(glyph->Y1, -glyph->X1),
			pos + ImVec2(glyph->Y1, -glyph->X0),

			ImVec2(glyph->U0, glyph->V0),
			ImVec2(glyph->U1, glyph->V0),
			ImVec2(glyph->U1, glyph->V1),
			ImVec2(glyph->U0, glyph->V1),
			text_color);
		pos.y -= glyph->AdvanceX;
	}
}
void AddHalfEllipse(ImDrawList* _ptrDrawList, const ImVec2& _center, float _radiusA, float _radiusB, float _tilt, const ImColor& _color, int _numSegments, float _thickness) {
	// Refer from https://github.com/kuravih/imgui_canvas
	ImVector<ImVec2> points;
	points.resize(_numSegments);

	float r, theta, dtheta = IM_PI / _numSegments;
	// float ecc2=1-ImPow(_radiusA/_radiusB,2);
	for (int i = 0; i < _numSegments ; i++) {
		theta = dtheta * i;
		// theta = theta - (ecc2/8 + ImPow(ecc2,2)/16 + 71*ImPow(ecc2,3)/2048)*ImSin(2*theta) + (5*(ImPow(ecc2,2) + ImPow(ecc2,3))/256)*ImSin(4*theta)  + (29*ImPow(ecc2,3)/6144)*ImSin(6*theta); // https://math.stackexchange.com/questions/2093569/points-on-an-ellipse
		r = _radiusA * _radiusB / ImSqrt(ImPow(_radiusA * ImCos(theta), 2) + ImPow(_radiusB * ImSin(theta), 2));
		points[i] = _center + ImVec2(r * ImCos(theta + _tilt), r * ImSin(theta + _tilt));
	}

	_ptrDrawList->AddPolyline(points.Data, _numSegments, _color, ImDrawFlags_None, _thickness);
}
void ImDrawer::IDraw()
{
	static float sx = 100.0f;
	static float sy = 240.0f;
	static float thickness = 3.0f;
	static int ngon_sides = 6;
	ImVec2 p = ImGui::GetCursorScreenPos();
	const ImVec2 r = ImGui::GetContentRegionAvail();
	//CL_CORE_INFO("X {0} Y {0}", p.x, p.y);
	const ImColor col = ImColor(1.0f,1.0f,1.0f);
	const float spacing = 10.0f;
	const ImDrawFlags corners_tl_br = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersBottomRight;
	const float rounding = sx / 5.0f;
	float x = p.x + 170.0f;
	float y = p.y + 450.0f;
	ImFont* font = ImGui::GetFont();
	m_Scale = 0.8;
	p.x = p.x - 25;
	p.y = p.y + 50;
	// Unpack all shapes to draw them all in a single call
	// Draw Rectangles PC -- Instruction Mem -- Registers -- Data Mem -- MUX
	// Draw Ellipses Shift -- Sign Extend -- Control -- ALU COntrol
	// Draw Triangles  
	// Draw Lines
	// Draw Texts
	// Draw Circles
	// Draw PolyLines ALU
	
	m_DrawList->AddText(font,30.0f, ImVec2(500-(m_CurrentSceneHeader.length()*5),-40) + p, col, m_CurrentSceneHeader.c_str());
	PathShapes* noblePath= m_ShapesVec[(int)m_CurrentScene];
	for (const auto& rects : noblePath->GetPathRects()) {
		m_DrawList->AddRect(rects.m_UpperLeft+p, rects.m_LowerRight+p, rects.m_Color, rects.m_Rounding, rects.m_Flags, rects.m_Thickness);
	}
	
	for (const auto& ellipses : noblePath->GetPathEllipses()) {
		AddEllipse(m_DrawList, ellipses.m_Center+p, ellipses.m_RadiusX, ellipses.m_RadiusY, ellipses.m_Angle, ellipses.m_Color, ellipses.m_NumSegments, ellipses.m_Thickness);
	}
	

	for (const auto& halfEllipses : noblePath->GetPathHalfEllipses()) {
		AddHalfEllipse(m_DrawList, halfEllipses.m_Center + p, halfEllipses.m_RadiusX, halfEllipses.m_RadiusY, halfEllipses.m_Angle, halfEllipses.m_Color, halfEllipses.m_NumSegments, halfEllipses.m_Thickness);
	}

	//AddHalfEllipse(m_DrawList, p + ImVec2(335.0f * 0.45f, 905.26f * 0.45f), 25 * 0.45, 25 * 0.45, 0, IM_COL32(0xff, 0xff, 0xff, 0xff), 64, 1.0);
	
	for (auto& polylines : noblePath->GetPathPolyLines()) {
		std::vector<ImVec2> temp = polylines.m_Points;
		float x = 0.0f;
		for (auto& it : temp) {
			it += p;
		}
		m_DrawList->AddPolyline(&temp[0], polylines.m_Points.size(), polylines.m_Color, polylines.m_Flags, polylines.m_Thickness);
	}
	for (const auto& triangles : noblePath->GetPathTriangles()) {
		m_DrawList->AddTriangleFilled(triangles.m_Point1+p, triangles.m_Point2+p, triangles.m_Point3+p, triangles.m_Color);
	}
	for (const auto& circles : noblePath->GetPathCircles()) {
		m_DrawList->AddCircleFilled(circles.m_Center+p, circles.m_Radius, circles.m_Color, circles.m_NumSegments);
	}
	// Positioning Issue + '\n' character  
	for (const auto& pathTexts : noblePath->GetPathTexts()) {
		m_DrawList->AddText(font,pathTexts.m_FontSize,pathTexts.m_Position+p, pathTexts.m_Color, pathTexts.m_Data);
		//AddTextVertical(m_DrawList, pathTexts.m_Data, pathTexts.m_Position+p, pathTexts.m_Color);
	}
	//m_DrawList->AddRect(noblePath.GetPathRects()[0].m_UpperLeft+p,noblePath[0].GetPathRects()[0]+p, noblePath.GetPathRects()[0].m_Color,
}

void ImDrawer::Init()
{
	//ImU32 color = IM_COL32(0xff, 0x5b, 0xfc, 0xff);
	ImU32 color = IM_COL32(0xff, 0xff, 0xff, 0xff);
	ImU32 color2 = IM_COL32(0xff, 0xff, 0xff, 0xff);
	float thickness = 1.0f;
	float rounding = 0.0f;
	float scale = 0.8f;
	//scale = m_Scale;

	PathShapes* noblePathOrig = new PathShapes(std::string("NoblePathOrig"));
	PathShapes* noblePathTemp = new PathShapes(std::string("NoblePathTemp"));
	std::vector <PathRect> pathRects;
	std::vector <PathEllipse> pathEllipses;
	std::vector <PathHalfEllipse> pathHalfEllipses;
	std::vector <PathPolyLine> pathPolyLines;
	std::vector <PathTriangle> pathTriangles;
	std::vector <PathCircle> pathCircles;
	std::vector <PathText> pathTexts;

	m_InstructionGroups["add"] = 1;
	m_InstructionGroups["and"] = 1;
	m_InstructionGroups["or"] = 1;
	m_InstructionGroups["mul"] = 1;
	m_InstructionGroups["slt"] = 1;
	m_InstructionGroups["sll"] = 1;
	m_InstructionGroups["srl"] = 1;
	m_InstructionGroups["sub"] = 1;
	m_InstructionGroups["beq"] = 2;
	m_InstructionGroups["bne"] = 2;
	m_InstructionGroups["j"] = 3;
	m_InstructionGroups["jr"] = 4;
	m_InstructionGroups["jal"] = 5;
	m_InstructionGroups["lui"] = 6;
	m_InstructionGroups["muli"] = 6;
	m_InstructionGroups["slti"] = 6;
	m_InstructionGroups["addi"] = 6;
	m_InstructionGroups["lw"] = 7;
	m_InstructionGroups["sw"] = 8;
	


	//pathRects.push_back({ std::string("Canvas"), ImVec2(10 * scale, +20 * scale), ImVec2(+1300 * scale, +800 * scale), color, 0.0f, ImDrawFlags_None, 1.0 });
	{
		//pathRects.push_back({ PRect("Canvas",scale,10,20,1300,800,color,rounding,ImDrawFlags_None,thickness) });
		pathRects.push_back({ PRect("PC",scale,70.f,400.f,120.f,520.f,color,rounding,ImDrawFlags_None,thickness) });
		pathRects.push_back({ PRect("InsMem",scale,170.f,450.f,270.f,590.f,color,rounding,ImDrawFlags_None,thickness) });
		pathRects.push_back({ PRect("RegFile",scale,500.f,420.f,650.f,620.f,color,rounding,ImDrawFlags_None,thickness) });
		pathRects.push_back({ PRect("DataMem",scale,930.f,501.f,1030.f,641.f,color,rounding,ImDrawFlags_None,thickness) });
		pathRects.push_back({ PRdRect("MUX1",scale,426.15f,606.16f,451.18f,657.16f,color,12.5f,ImDrawFlags_None,thickness) });
		pathRects.push_back({ PRdRect("MUX2",scale,450.22f,507.f,475.25f,558.f,color,12.5f,ImDrawFlags_None,thickness) });
		pathRects.push_back({ PRdRect("MUX3",scale,730.23f,520.65f,755.27f,571.65f,color,12.5f,ImDrawFlags_None,thickness) });
		pathRects.push_back({ PRdRect("MUX4",scale,1080.25f,507.f,1105.25f,558.f,color,12.5f,ImDrawFlags_None,thickness) });
		pathRects.push_back({ PRdRect("MUX5",scale,1190.25f,505.28f,1215.25f,556.28f,color,12.5f,ImDrawFlags_None,thickness) });
		pathRects.push_back({ PRdRect("MUX6",scale,975.25,120.f,1000.25f,170.f,color,12.5f,ImDrawFlags_None,thickness) });
		pathRects.push_back({ PRdRect("MUX7",scale,1050.25f,120.f,1075.25f,170.f,color,12.5f,ImDrawFlags_None,thickness) });
		pathRects.push_back({ PRdRect("MUX8",scale,1120.25f,203.5f,1145.25f,253.5f,color,12.5f,ImDrawFlags_None,thickness) });
	}

	{
		pathEllipses.push_back({ PEllipse("Control",scale,500.5f,300.25f,100,57.8f,0,color,64,thickness) });
		pathEllipses.push_back({ PEllipse("ALUControl",scale,797.f,662.f,50,31.38f,0,color,64,thickness) });
		pathEllipses.push_back({ PEllipse("SignExtend",scale,570.25f,690.f,50,32.45f,0,color,64,thickness) });
		pathEllipses.push_back({ PEllipse("ShiftLeft1",scale,450.31f,90.6f,30,24.175f,0,color,64,thickness) });
		pathEllipses.push_back({ PEllipse("ShiftLeft2",scale,711.16f,263.11f,30,24.175f,0,color,64,thickness) });
	}

	{
		std::vector<ImVec2> aluPath1;
		aluPath1.emplace_back(PPoint(220.25f, 80.f, scale));
		aluPath1.emplace_back(PPoint(290.f, 100.f, scale));
		aluPath1.emplace_back(PPoint(290.f, 150.f, scale));
		aluPath1.emplace_back(PPoint(220.f, 170.f, scale));
		aluPath1.emplace_back(PPoint(220.f, 140.f, scale));
		aluPath1.emplace_back(PPoint(241.f, 125.f, scale));
		aluPath1.emplace_back(PPoint(220.f, 110.f, scale));
		aluPath1.emplace_back(PPoint(220.25f, 80.f, scale));

		pathPolyLines.push_back({ PPolyLines(("ALU1"),aluPath1,(int)aluPath1.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> aluPath2;
		aluPath2.emplace_back(PPoint(780.25f, 484.f, scale));
		aluPath2.emplace_back(PPoint(780.25f, 440.f, scale));
		aluPath2.emplace_back(PPoint(880.25f, 470.f, scale));
		aluPath2.emplace_back(PPoint(880.25f, 540.f, scale));
		aluPath2.emplace_back(PPoint(780.25f, 570.f, scale));
		aluPath2.emplace_back(PPoint(780.25f, 526.f, scale));
		aluPath2.emplace_back(PPoint(810.25f, 505.f, scale));
		aluPath2.emplace_back(PPoint(780.25f, 484.f, scale));

		pathPolyLines.push_back({ PPolyLines(("ALU2"),aluPath2,(int)aluPath2.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> aluPath3;
		aluPath3.emplace_back(PPoint(781.5f, 194.f, scale));
		aluPath3.emplace_back(PPoint(781.5f, 150.f, scale));
		aluPath3.emplace_back(PPoint(881.5f, 180.f, scale));
		aluPath3.emplace_back(PPoint(881.5f, 250.f, scale));
		aluPath3.emplace_back(PPoint(781.5f, 280.f, scale));
		aluPath3.emplace_back(PPoint(781.5f, 236.f, scale));
		aluPath3.emplace_back(PPoint(811.5f, 215.f, scale));
		aluPath3.emplace_back(PPoint(781.5f, 194.f, scale));

		pathPolyLines.push_back({ PPolyLines(("ALU3"),aluPath3,(int)aluPath3.size(),color,ImDrawFlags_None,thickness) });
	}

	{
		pathTriangles.push_back({ PSTriangle("PCTri",60.f,455.f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("IMEMTri",160.25f,455.f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("ALU1Tri1",210.25f,85.f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("ALU1Tri2",210.25f,150.f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("ALU2Tri1",770.25f,457.f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("ALU2Tri2",770.25f,543.f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("ALU3Tri1",771.f,167.f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("ALU3Tri2",771.5f,253.f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("SL1Tri",416.f,85.33f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("SL2Tri",677.0f,255.33f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("ControlTri",433.0f,285.33f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("ALUControlTri",755.50f,657.75f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("ReadReg1",490.0f,425.0f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("ReadReg2",490.0f,465.0f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("WriteReg",490.0f,527.0f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("WriteDataReg",490.0f,565.0f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("Address",920.0f,520.0f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("WriteDataMem",920.0f,593.5f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("SignExtend",527.75f,685.f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("MUX1Tri1",416.15f,615.0f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("MUX1Tri2",416.15f,645.5f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("MUX2Tri1",440.1f,514.5f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("MUX2Tri2",440.15f,540.5f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("MUX3Tri1",720.15f,528.13f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("MUX3Tri2",720.15f,553.75f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("MUX4Tri1",1070.f,514.5f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("MUX4Tri2",1070.f,540.12f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("MUX5Tri1",1180.f,512.85f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("MUX5Tri2",1180.15f,539.5f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("MUX6Tri1",965.f,123.25f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("MUX6Tri2",965.f,153.0f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("MUX7Tri1",1040.15f,121.09f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("MUX7Tri2",1040.15f,153.71f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("MUX8Tri1",1110.15f,205.0f,10.f,scale,color,thickness) });
		pathTriangles.push_back({ PSTriangle("MUX8Tri2",1110.15f,245.0f,10.f,scale,color,thickness) });







	}

	{
		pathCircles.push_back({ PCircle("Node1",145.25f,460.8f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node2",320.25f,519.8f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node3",349.0f,590.65f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node4",380.25f,429.75f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node5",380.25f,470.15f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node6",380.25f,520.0f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node7",400.30f,545.0f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node8",435.15f,470.0f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node9",490.15f,689.5f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node10",505.0f,128.0f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node11",572.5f,91.0f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node12",672.25f,558.0f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node13",691.0f,533.75f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node14",700.0f,129.0f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node15",720.0f,462.15f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node16",909.75f,108.25f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node17",905.25f,524.5f,3.f,scale,color,32,thickness) });
		pathCircles.push_back({ PCircle("Node18",655.45f,390.0f,3.f,scale,color,32,thickness) });

	}

	// PathPolyLines
	{

		std::vector<ImVec2> mux8ToPc;
		mux8ToPc.emplace_back(PPoint(60.25f, 460.f, scale));
		mux8ToPc.emplace_back(PPoint(30.f, 460.f, scale));
		mux8ToPc.emplace_back(PPoint(30.f, 40.f, scale));
		mux8ToPc.emplace_back(PPoint(1171.15f, 45.15f, scale));
		mux8ToPc.emplace_back(PPoint(1171.15f, 230.f, scale));
		mux8ToPc.emplace_back(PPoint(1145.15f, 230.f, scale));

		pathPolyLines.push_back({ PPolyLines(("Mux8ToPc"),mux8ToPc,(int)mux8ToPc.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> pcToNode1;
		pcToNode1.emplace_back(PPoint(120.25f, 460.f, scale));
		pcToNode1.emplace_back(PPoint(145.25f, 460.f, scale));
		/*pcToAlu1.emplace_back(PPoint(145.25f, 90.f, scale));
		pcToAlu1.emplace_back(PPoint(210.25f, 90.f, scale));*/
		pathPolyLines.push_back({ PPolyLines(("PcToNode1"),pcToNode1,(int)pcToNode1.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node1ToAlu1;
		node1ToAlu1.emplace_back(PPoint(145.25f, 460.f, scale));
		node1ToAlu1.emplace_back(PPoint(145.25f, 90.f, scale));
		node1ToAlu1.emplace_back(PPoint(210.25f, 90.f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node1ToAlu1"),node1ToAlu1,(int)node1ToAlu1.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node1ToReadAddress;
		node1ToReadAddress.emplace_back(PPoint(145.25f, 460.f, scale));
		node1ToReadAddress.emplace_back(PPoint(160.25f, 460.f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node1ToReadAddress"),node1ToReadAddress,(int)node1ToReadAddress.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> incToAlu1;
		incToAlu1.emplace_back(PPoint(195.20f, 155.5f, scale));
		incToAlu1.emplace_back(PPoint(210.25f, 155.5f, scale));
		pathPolyLines.push_back({ PPolyLines(("IncToAlu1"),incToAlu1,(int)incToAlu1.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> insMemToNode2;
		insMemToNode2.emplace_back(PPoint(270.25f, 520.f, scale));
		insMemToNode2.emplace_back(PPoint(320.25f, 520.f, scale));
		pathPolyLines.push_back({ PPolyLines(("InsMemToNode2"),insMemToNode2,(int)insMemToNode2.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node2ToSL1;
		node2ToSL1.emplace_back(PPoint(320.25f, 520.f, scale));
		node2ToSL1.emplace_back(PPoint(320.25f, 90.25f, scale));
		node2ToSL1.emplace_back(PPoint(416.0f, 90.25f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node2ToSL1"),node2ToSL1,(int)node2ToSL1.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node2ToNode6;
		node2ToNode6.emplace_back(PPoint(320.25f, 520.f, scale));
		node2ToNode6.emplace_back(PPoint(380.25f, 520.f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node2ToNode6"),node2ToNode6,(int)node2ToNode6.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node6ToNode5;
		node6ToNode5.emplace_back(PPoint(380.25f, 520.f, scale));
		node6ToNode5.emplace_back(PPoint(380.25f, 470.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node6ToNode5"),node6ToNode5,(int)node6ToNode5.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node6ToNode7;
		node6ToNode7.emplace_back(PPoint(380.25f, 520.f, scale));
		node6ToNode7.emplace_back(PPoint(380.25f, 545.0f, scale));
		node6ToNode7.emplace_back(PPoint(400.25f, 545.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node6ToNode7"),node6ToNode7,(int)node6ToNode7.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node7ToMux2Tri2;
		node7ToMux2Tri2.emplace_back(PPoint(400.25f, 545.0f, scale));
		node7ToMux2Tri2.emplace_back(PPoint(440.25f, 545.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node7ToMux2Tri2"),node7ToMux2Tri2,(int)node7ToMux2Tri2.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node7ToNode9;
		node7ToNode9.emplace_back(PPoint(400.25f, 545.0f, scale));
		node7ToNode9.emplace_back(PPoint(400.25f, 690.0f, scale));
		node7ToNode9.emplace_back(PPoint(490.25f, 690.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node7ToNode9"),node7ToNode9,(int)node7ToNode9.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node9ToSignExt;
		node9ToSignExt.emplace_back(PPoint(490.25f, 690.0f, scale));
		node9ToSignExt.emplace_back(PPoint(527.75f, 690.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node9ToSignExt"),node9ToSignExt,(int)node9ToSignExt.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node9ToALUControl;
		node9ToALUControl.emplace_back(PPoint(490.25f, 690.0f, scale));
		node9ToALUControl.emplace_back(PPoint(490.25f, 755.0f, scale));
		node9ToALUControl.emplace_back(PPoint(750.25f, 755.0f, scale));
		node9ToALUControl.emplace_back(PPoint(750.25f, 662.75f, scale));
		node9ToALUControl.emplace_back(PPoint(755.25f, 662.75f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node9ToALUControl"),node9ToALUControl,(int)node9ToALUControl.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node5ToNode8;
		node5ToNode8.emplace_back(PPoint(380.25f, 470.0f, scale));
		node5ToNode8.emplace_back(PPoint(435.25f, 470.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node5ToNode8"),node5ToNode8,(int)node5ToNode8.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node8ToReadReg2;
		node8ToReadReg2.emplace_back(PPoint(435.25f, 470.0f, scale));
		node8ToReadReg2.emplace_back(PPoint(490.25f, 470.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node8ToReadReg2"),node8ToReadReg2,(int)node8ToReadReg2.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node8ToMux2Tri1;
		node8ToMux2Tri1.emplace_back(PPoint(435.25f, 470.0f, scale));
		node8ToMux2Tri1.emplace_back(PPoint(435.25f, 519.50f, scale));
		node8ToMux2Tri1.emplace_back(PPoint(440.25f, 519.50f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node8ToMux2Tri1"),node8ToMux2Tri1,(int)node8ToMux2Tri1.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node5ToNode4;
		node5ToNode4.emplace_back(PPoint(380.25f, 470.0f, scale));
		node5ToNode4.emplace_back(PPoint(380.25f, 429.75f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node5ToNode4"),node5ToNode4,(int)node5ToNode4.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node4ToReadReg1;
		node4ToReadReg1.emplace_back(PPoint(380.25f, 429.75f, scale));
		node4ToReadReg1.emplace_back(PPoint(490.25f, 429.75f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node4ToReadReg1"),node4ToReadReg1,(int)node4ToReadReg1.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node4ToControl;
		node4ToControl.emplace_back(PPoint(380.25f, 429.75f, scale));
		node4ToControl.emplace_back(PPoint(380.25f, 290.0f, scale));
		node4ToControl.emplace_back(PPoint(433.0f, 290.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node4ToControl"),node4ToControl,(int)node4ToControl.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> mux2ToMux1Tri1;
		mux2ToMux1Tri1.emplace_back(PPoint(475.25f, 532.0f, scale));
		mux2ToMux1Tri1.emplace_back(PPoint(478.25f, 532.0f, scale));
		mux2ToMux1Tri1.emplace_back(PPoint(478.25f, 578.70f, scale));
		mux2ToMux1Tri1.emplace_back(PPoint(381.25f, 578.70f, scale));
		mux2ToMux1Tri1.emplace_back(PPoint(381.25f, 619.70f, scale));
		mux2ToMux1Tri1.emplace_back(PPoint(416.25f, 619.70f, scale));
		pathPolyLines.push_back({ PPolyLines(("Mux2ToMux1Tri1"),mux2ToMux1Tri1,(int)mux2ToMux1Tri1.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> raToMux1Tri2;
		raToMux1Tri2.emplace_back(PPoint(381.25f, 650.50f, scale));
		raToMux1Tri2.emplace_back(PPoint(416.25f, 650.50f, scale));
		pathPolyLines.push_back({ PPolyLines(("RaToMux1Tri2"),raToMux1Tri2,(int)raToMux1Tri2.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> mux1ToWriteReg;
		mux1ToWriteReg.emplace_back(PPoint(451.25f, 631.25f, scale));
		mux1ToWriteReg.emplace_back(PPoint(488.25f, 631.25f, scale));
		mux1ToWriteReg.emplace_back(PPoint(488.25f, 532.0f, scale));
		mux1ToWriteReg.emplace_back(PPoint(490.25f, 532.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Mux1ToWriteReg"),mux1ToWriteReg,(int)mux1ToWriteReg.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> mux5ToWriteData;
		mux5ToWriteData.emplace_back(PPoint(1215.25f, 530.0f, scale));
		mux5ToWriteData.emplace_back(PPoint(1229.0f, 530.0f, scale));
		mux5ToWriteData.emplace_back(PPoint(1229.65f, 770.0f, scale));
		mux5ToWriteData.emplace_back(PPoint(482.65f, 770.0f, scale));
		mux5ToWriteData.emplace_back(PPoint(482.65f, 570.0f, scale));
		mux5ToWriteData.emplace_back(PPoint(490.25f, 570.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Mux5ToWriteData"),mux5ToWriteData,(int)mux5ToWriteData.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> readData1ToNode15;
		readData1ToNode15.emplace_back(PPoint(650.25f, 462.0f, scale));
		readData1ToNode15.emplace_back(PPoint(720.0f, 462.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("ReadData1ToNode15"),readData1ToNode15,(int)readData1ToNode15.size(),color,ImDrawFlags_None,thickness) });


		std::vector<ImVec2> node15ToAlu2Tri1;
		node15ToAlu2Tri1.emplace_back(PPoint(720.0f, 462.0f, scale));
		node15ToAlu2Tri1.emplace_back(PPoint(770.25f, 462.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node15ToAlu2Tri1"),node15ToAlu2Tri1,(int)node15ToAlu2Tri1.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node15ToMux8Tri2;
		node15ToMux8Tri2.emplace_back(PPoint(720.0f, 462.0f, scale));
		node15ToMux8Tri2.emplace_back(PPoint(720.25f, 420.0f, scale));
		node15ToMux8Tri2.emplace_back(PPoint(1100.25f, 420.0f, scale));
		node15ToMux8Tri2.emplace_back(PPoint(1100.25f, 250.0f, scale));
		node15ToMux8Tri2.emplace_back(PPoint(1110.25f, 250.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node15ToMux8Tri2"),node15ToMux8Tri2,(int)node15ToMux8Tri2.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> readData2ToNode13;
		readData2ToNode13.emplace_back(PPoint(650.25f, 533.25f, scale));
		readData2ToNode13.emplace_back(PPoint(690.85f, 533.25f, scale));
		pathPolyLines.push_back({ PPolyLines(("ReadData2ToNode13"),readData2ToNode13,(int)readData2ToNode13.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node13ToMux3Tri1;
		node13ToMux3Tri1.emplace_back(PPoint(690.85f, 533.25f, scale));
		node13ToMux3Tri1.emplace_back(PPoint(720.25f, 533.25f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node13ToMux3Tri1"),node13ToMux3Tri1,(int)node13ToMux3Tri1.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node13ToMemWriteData;
		node13ToMemWriteData.emplace_back(PPoint(690.85f, 533.25f, scale));
		node13ToMemWriteData.emplace_back(PPoint(690.85f, 600.25f, scale));
		node13ToMemWriteData.emplace_back(PPoint(920.25f, 600.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node13ToMemWriteData"),node13ToMemWriteData,(int)node13ToMemWriteData.size(),color,ImDrawFlags_None,thickness) });


		std::vector<ImVec2> signExtToNode12;
		signExtToNode12.emplace_back(PPoint(602.75f, 690.0f, scale));
		signExtToNode12.emplace_back(PPoint(672.75f, 690.0f, scale));
		signExtToNode12.emplace_back(PPoint(672.75f, 558.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("SignExtToNode12"),signExtToNode12,(int)signExtToNode12.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node12ToMux3Tri2;
		node12ToMux3Tri2.emplace_back(PPoint(672.75f, 558.0f, scale));
		node12ToMux3Tri2.emplace_back(PPoint(720.25f, 558.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node12ToMux3Tri2"),node12ToMux3Tri2,(int)node12ToMux3Tri2.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node12ToSL2;
		node12ToSL2.emplace_back(PPoint(672.75f, 558.0f, scale));
		node12ToSL2.emplace_back(PPoint(672.75f, 260.0f, scale));
		node12ToSL2.emplace_back(PPoint(677.25f, 260.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node12ToSL2"),node12ToSL2,(int)node12ToSL2.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> mux3ToAlu2Tri2;
		mux3ToAlu2Tri2.emplace_back(PPoint(755.25f, 548.0f, scale));
		mux3ToAlu2Tri2.emplace_back(PPoint(770.25f, 548.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Mux3ToAlu2Tri2"),mux3ToAlu2Tri2,(int)mux3ToAlu2Tri2.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> aluControlToAlu2;
		aluControlToAlu2.emplace_back(PPoint(828.35f, 661.25f, scale));
		aluControlToAlu2.emplace_back(PPoint(848.35f, 661.25f, scale));
		aluControlToAlu2.emplace_back(PPoint(848.35f, 549.5f, scale));
		pathPolyLines.push_back({ PPolyLines(("AluControlToAlu2"),aluControlToAlu2,(int)aluControlToAlu2.size(),color,ImDrawFlags_None,thickness) });


		std::vector<ImVec2> alu2ToNode17;
		alu2ToNode17.emplace_back(PPoint(880.5f, 524.65f, scale));
		alu2ToNode17.emplace_back(PPoint(905.5f, 524.65f, scale));
		pathPolyLines.push_back({ PPolyLines(("Alu2ToNode17"),alu2ToNode17,(int)alu2ToNode17.size(),color,ImDrawFlags_None,thickness) });

		std::vector <ImVec2> node17ToAddress;
		node17ToAddress.emplace_back(PPoint(905.5f, 524.65f, scale));
		node17ToAddress.emplace_back(PPoint(920.0f, 524.65f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node17ToAddress"),node17ToAddress,(int)node17ToAddress.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node17ToMux4Tri2;
		node17ToMux4Tri2.emplace_back(PPoint(905.5f, 524.65f, scale));
		node17ToMux4Tri2.emplace_back(PPoint(905.5f, 660.5f, scale));
		node17ToMux4Tri2.emplace_back(PPoint(1060.25f, 660.5f, scale));
		node17ToMux4Tri2.emplace_back(PPoint(1060.25f, 545.0f, scale));
		node17ToMux4Tri2.emplace_back(PPoint(1070.25f, 545.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node17ToMux4Tri2"),node17ToMux4Tri2,(int)node17ToMux4Tri2.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> readDataMemToMux4Tri1;
		readDataMemToMux4Tri1.emplace_back(PPoint(1030.0f, 520.0f, scale));
		readDataMemToMux4Tri1.emplace_back(PPoint(1070.25f, 520.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("ReadDataMemToMux4Tri1"),readDataMemToMux4Tri1,(int)readDataMemToMux4Tri1.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> mux4ToMux5Tri2;
		mux4ToMux5Tri2.emplace_back(PPoint(1105.25f, 532.0f, scale));
		mux4ToMux5Tri2.emplace_back(PPoint(1160.25f, 532.0f, scale));
		mux4ToMux5Tri2.emplace_back(PPoint(1160.25f, 544.50f, scale));
		mux4ToMux5Tri2.emplace_back(PPoint(1180.35f, 544.5f, scale));
		pathPolyLines.push_back({ PPolyLines(("Mux4ToMux5Tri2"),mux4ToMux5Tri2,(int)mux4ToMux5Tri2.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> alu1ToNode10;
		alu1ToNode10.emplace_back(PPoint(290.0f, 128.25f, scale));
		alu1ToNode10.emplace_back(PPoint(505.0f, 128.25f, scale));
		pathPolyLines.push_back({ PPolyLines(("Alu1ToNode10"),alu1ToNode10,(int)alu1ToNode10.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node10ToNode11;
		node10ToNode11.emplace_back(PPoint(505.0f, 128.25f, scale));
		node10ToNode11.emplace_back(PPoint(505.0f, 107.25f, scale));
		node10ToNode11.emplace_back(PPoint(572.5f, 107.25f, scale));
		node10ToNode11.emplace_back(PPoint(572.5f, 91.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node10ToNode11"),node10ToNode11,(int)node10ToNode11.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node11ToMux7Tri1;
		node11ToMux7Tri1.emplace_back(PPoint(572.5f, 91.0f, scale));
		node11ToMux7Tri1.emplace_back(PPoint(1024.5f, 91.0f, scale));
		node11ToMux7Tri1.emplace_back(PPoint(1024.5f, 126.0f, scale));
		node11ToMux7Tri1.emplace_back(PPoint(1039.5f, 126.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node11ToMux7Tri1"),node11ToMux7Tri1,(int)node11ToMux7Tri1.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node10ToNode14;
		node10ToNode14.emplace_back(PPoint(505.0f, 128.25f, scale));
		node10ToNode14.emplace_back(PPoint(700.0f, 129.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node10ToNode14"),node10ToNode14,(int)node10ToNode14.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node14ToAlu3Tri1;
		node14ToAlu3Tri1.emplace_back(PPoint(700.0f, 129.0f, scale));
		node14ToAlu3Tri1.emplace_back(PPoint(700.0f, 172.0f, scale));
		node14ToAlu3Tri1.emplace_back(PPoint(771.25f, 172.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node14ToAlu3Tri1"),node14ToAlu3Tri1,(int)node14ToAlu3Tri1.size(),color,ImDrawFlags_None,thickness) });


		std::vector<ImVec2> node14ToNode16;
		node14ToNode16.emplace_back(PPoint(700.0f, 129.0f, scale));
		node14ToNode16.emplace_back(PPoint(700.0f, 108.25f, scale));
		node14ToNode16.emplace_back(PPoint(910.0f, 108.25f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node14ToNode16"),node14ToNode16,(int)node14ToNode16.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node16ToMux6Tri1;
		node16ToMux6Tri1.emplace_back(PPoint(910.0f, 108.25f, scale));
		node16ToMux6Tri1.emplace_back(PPoint(939.75f, 108.25f, scale));
		node16ToMux6Tri1.emplace_back(PPoint(939.75f, 128.25f, scale));
		node16ToMux6Tri1.emplace_back(PPoint(965.0f, 128.25f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node16ToMux6Tri1"),node16ToMux6Tri1,(int)node16ToMux6Tri1.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node16ToMux5Tri1;
		node16ToMux5Tri1.emplace_back(PPoint(910.0f, 108.25f, scale));
		node16ToMux5Tri1.emplace_back(PPoint(910.75f, 199.25f, scale));
		node16ToMux5Tri1.emplace_back(PPoint(1069.75f, 199.25f, scale));
		node16ToMux5Tri1.emplace_back(PPoint(1069.75f, 469.25f, scale));
		node16ToMux5Tri1.emplace_back(PPoint(1159.75f, 469.25f, scale));
		node16ToMux5Tri1.emplace_back(PPoint(1159.75f, 518.0f, scale));
		node16ToMux5Tri1.emplace_back(PPoint(1180.0f, 518.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node16ToMux5Tri1"),node16ToMux5Tri1,(int)node16ToMux5Tri1.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> sl1ToNode11;
		sl1ToNode11.emplace_back(PPoint(474.5f, 91.0f, scale));
		sl1ToNode11.emplace_back(PPoint(572.5f, 91.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("SL1ToNode11"),sl1ToNode11,(int)sl1ToNode11.size(),color,ImDrawFlags_None,thickness) });


		std::vector<ImVec2> sl2ToAlu3Tri2;
		sl2ToAlu3Tri2.emplace_back(PPoint(735.0f, 258.0f, scale));
		sl2ToAlu3Tri2.emplace_back(PPoint(771.25f, 258.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("SL2ToAlu3Tri2"),sl2ToAlu3Tri2,(int)sl2ToAlu3Tri2.size(),color,ImDrawFlags_None,thickness) });


		std::vector<ImVec2> alu3ToMux6Tri2;
		alu3ToMux6Tri2.emplace_back(PPoint(881.5f, 215.0f, scale));
		alu3ToMux6Tri2.emplace_back(PPoint(951.5f, 215.0f, scale));
		alu3ToMux6Tri2.emplace_back(PPoint(951.5f, 158.0f, scale));
		alu3ToMux6Tri2.emplace_back(PPoint(965.35f, 158.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Alu3ToMux6Tri2"),alu3ToMux6Tri2,(int)alu3ToMux6Tri2.size(),color,ImDrawFlags_None,thickness) });


		std::vector<ImVec2> mux6ToMux7Tri2;
		mux6ToMux7Tri2.emplace_back(PPoint(1000.25f, 145.0f, scale));
		mux6ToMux7Tri2.emplace_back(PPoint(1020.25f, 145.0f, scale));
		mux6ToMux7Tri2.emplace_back(PPoint(1020.25f, 158.0f, scale));
		mux6ToMux7Tri2.emplace_back(PPoint(1040.25f, 158.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Mux6ToMux7Tri2"),mux6ToMux7Tri2,(int)mux6ToMux7Tri2.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> mux7ToMux8Tri1;
		mux7ToMux8Tri1.emplace_back(PPoint(1075.25f, 145.0f, scale));
		mux7ToMux8Tri1.emplace_back(PPoint(1085.25f, 145.0f, scale));
		mux7ToMux8Tri1.emplace_back(PPoint(1085.25f, 210.0f, scale));
		mux7ToMux8Tri1.emplace_back(PPoint(1110.0f, 210.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Mux7ToMux8Tri1"),mux7ToMux8Tri1,(int)mux7ToMux8Tri1.size(),color,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> regDstToNode3;
		regDstToNode3.emplace_back(PPoint(500.0f, 200.0f, scale));
		regDstToNode3.emplace_back(PPoint(500.0f, 194.0f, scale));
		regDstToNode3.emplace_back(PPoint(610.75f, 194.0f, scale));
		regDstToNode3.emplace_back(PPoint(610.75f, 160.0f, scale));
		regDstToNode3.emplace_back(PPoint(349.0f, 160.0f, scale));
		regDstToNode3.emplace_back(PPoint(349.0f, 590.75f, scale));
		pathPolyLines.push_back({ PPolyLines(("RegDstToNode3"),regDstToNode3,(int)regDstToNode3.size(),color2,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node3ToMux2;
		node3ToMux2.emplace_back(PPoint(349.0f, 590.75f, scale));
		node3ToMux2.emplace_back(PPoint(462.75f, 590.75f, scale));
		node3ToMux2.emplace_back(PPoint(462.75f, 570.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node3ToMux2"),node3ToMux2,(int)node3ToMux2.size(),color2,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node3ToAluControl;
		node3ToAluControl.emplace_back(PPoint(349.0f, 590.75f, scale));
		node3ToAluControl.emplace_back(PPoint(349.0f, 780.0f, scale));
		node3ToAluControl.emplace_back(PPoint(810.75, 780.0f, scale));
		node3ToAluControl.emplace_back(PPoint(810.75f, 707.40f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node3ToAluControl"),node3ToAluControl,(int)node3ToAluControl.size(),color2,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> jumpToMux7;
		jumpToMux7.emplace_back(PPoint(524.86f, 209.50f, scale));
		jumpToMux7.emplace_back(PPoint(660.25f, 209.50f, scale));
		jumpToMux7.emplace_back(PPoint(660.25f, 59.65f, scale));
		jumpToMux7.emplace_back(PPoint(1063.25f, 59.65f, scale));
		jumpToMux7.emplace_back(PPoint(1063.25f, 108.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("JumpToMux7"),jumpToMux7,(int)jumpToMux7.size(),color2,ImDrawFlags_None,thickness) });


		std::vector<ImVec2> jumpRegToMux8;
		jumpRegToMux8.emplace_back(PPoint(541.60f, 230.0f, scale));
		jumpRegToMux8.emplace_back(PPoint(760.25f, 230.0f, scale));
		jumpRegToMux8.emplace_back(PPoint(760.25f, 300.0f, scale));
		jumpRegToMux8.emplace_back(PPoint(1132.50f, 300.0f, scale));
		jumpRegToMux8.emplace_back(PPoint(1132.50f, 266.5f, scale));
		pathPolyLines.push_back({ PPolyLines(("JumpRegToMux8"),jumpRegToMux8,(int)jumpRegToMux8.size(),color2,ImDrawFlags_None,thickness) });


		//Branch Control
		std::vector<ImVec2> branchToAnd1;
		branchToAnd1.emplace_back(PPoint(551.50f, 254.5f, scale));
		branchToAnd1.emplace_back(PPoint(660.00f, 254.5f, scale));
		branchToAnd1.emplace_back(PPoint(660.00f, 320.25f, scale));
		branchToAnd1.emplace_back(PPoint(870.25f, 320.25f, scale));
		pathPolyLines.push_back({ PPolyLines(("BranchToAnd1"),branchToAnd1,(int)branchToAnd1.size(),color2,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> alu2ZeroToAnd2;
		alu2ZeroToAnd2.emplace_back(PPoint(880.25f, 483.0f, scale));
		alu2ZeroToAnd2.emplace_back(PPoint(900.0f, 483.0f, scale));
		alu2ZeroToAnd2.emplace_back(PPoint(900.0f, 430.0f, scale));
		alu2ZeroToAnd2.emplace_back(PPoint(860.0f, 430.0f, scale));
		alu2ZeroToAnd2.emplace_back(PPoint(860.0f, 350.0f, scale));
		alu2ZeroToAnd2.emplace_back(PPoint(870.25f, 350.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Alu2ZeroToAnd2"),alu2ZeroToAnd2,(int)alu2ZeroToAnd2.size(),color2,ImDrawFlags_None,thickness) });

		//

		std::vector<ImVec2> memReadToMem;
		memReadToMem.emplace_back(PPoint(557.0f, 280.0f, scale));
		memReadToMem.emplace_back(PPoint(650.25f, 280.0f, scale));
		memReadToMem.emplace_back(PPoint(650.25f, 329.5f, scale));
		memReadToMem.emplace_back(PPoint(840.25f, 329.5f, scale));
		memReadToMem.emplace_back(PPoint(840.25f, 369.5f, scale));
		memReadToMem.emplace_back(PPoint(1170.25f, 369.5f, scale));
		memReadToMem.emplace_back(PPoint(1170.25f, 671.5f, scale));
		memReadToMem.emplace_back(PPoint(980.0f, 671.5f, scale));
		memReadToMem.emplace_back(PPoint(980.0f, 641.25f, scale));
		pathPolyLines.push_back({ PPolyLines(("MemReadToMem"),memReadToMem,(int)memReadToMem.size(),color2,ImDrawFlags_None,thickness) });


		std::vector<ImVec2> memToRegToMux4;
		memToRegToMux4.emplace_back(PPoint(558.25f, 300.25f, scale));
		memToRegToMux4.emplace_back(PPoint(640.25f, 300.25f, scale));
		memToRegToMux4.emplace_back(PPoint(640.25f, 340.0f, scale));
		memToRegToMux4.emplace_back(PPoint(831.25f, 340.0f, scale));
		memToRegToMux4.emplace_back(PPoint(831.25f, 388.0f, scale));
		memToRegToMux4.emplace_back(PPoint(1093.25f, 388.0f, scale));
		memToRegToMux4.emplace_back(PPoint(1093.25f, 495.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("MemToRegToMux4"),memToRegToMux4,(int)memToRegToMux4.size(),color2,ImDrawFlags_None,thickness) });


		std::vector <ImVec2> aluOPToAluControl;
		aluOPToAluControl.emplace_back(PPoint(557.0f, 320.0f, scale));
		aluOPToAluControl.emplace_back(PPoint(630.25f, 320.0f, scale));
		aluOPToAluControl.emplace_back(PPoint(630.25f, 350.0f, scale));
		aluOPToAluControl.emplace_back(PPoint(679.25f, 350.0f, scale));
		aluOPToAluControl.emplace_back(PPoint(679.25f, 730.0f, scale));
		aluOPToAluControl.emplace_back(PPoint(798.0f, 730.0f, scale));
		aluOPToAluControl.emplace_back(PPoint(798.0f, 712.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("AluOPToAluControl"),aluOPToAluControl,(int)aluOPToAluControl.size(),color2,ImDrawFlags_None,thickness) });

		std::vector <ImVec2> memWriteToMem;
		memWriteToMem.emplace_back(PPoint(553.5f, 340.0f, scale));
		memWriteToMem.emplace_back(PPoint(620.25f, 340.0f, scale));
		memWriteToMem.emplace_back(PPoint(620.25f, 360.0f, scale));
		memWriteToMem.emplace_back(PPoint(820.25f, 360.0f, scale));
		memWriteToMem.emplace_back(PPoint(820.5f, 400.5f, scale));
		memWriteToMem.emplace_back(PPoint(980.5f, 400.5f, scale));
		memWriteToMem.emplace_back(PPoint(980.5f, 501.25f, scale));
		pathPolyLines.push_back({ PPolyLines(("MemWriteToMem"),memWriteToMem,(int)memWriteToMem.size(),color2,ImDrawFlags_None,thickness) });


		std::vector<ImVec2> aluSrcToMux3;
		aluSrcToMux3.emplace_back(PPoint(546.80f, 360.0f, scale));
		aluSrcToMux3.emplace_back(PPoint(610.25f, 360.0f, scale));
		aluSrcToMux3.emplace_back(PPoint(610.25f, 370.0f, scale));
		aluSrcToMux3.emplace_back(PPoint(742.25f, 370.0f, scale));
		aluSrcToMux3.emplace_back(PPoint(742.25f, 508.65f, scale));
		pathPolyLines.push_back({ PPolyLines(("AluSrcToMux3"),aluSrcToMux3,(int)aluSrcToMux3.size(),color2,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> regWriteToRegisters;
		regWriteToRegisters.emplace_back(PPoint(535.25f, 380.0f, scale));
		regWriteToRegisters.emplace_back(PPoint(575.25f, 380.0f, scale));
		regWriteToRegisters.emplace_back(PPoint(575.25f, 420.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("RegWriteToRegisters"),regWriteToRegisters,(int)regWriteToRegisters.size(),color2,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> jLinkToNode18;
		jLinkToNode18.emplace_back(PPoint(526.0f, 390.0f, scale));
		jLinkToNode18.emplace_back(PPoint(656.0f, 390.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("JLinkToNode18"),jLinkToNode18,(int)jLinkToNode18.size(),color2,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node18ToMux1;
		node18ToMux1.emplace_back(PPoint(656.0f, 390.0f, scale));
		node18ToMux1.emplace_back(PPoint(656.0f, 790.0f, scale));
		node18ToMux1.emplace_back(PPoint(439.0f, 790.0f, scale));
		node18ToMux1.emplace_back(PPoint(439.0f, 669.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node18ToMux1"),node18ToMux1,(int)node18ToMux1.size(),color2,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> node18ToMux5;
		node18ToMux5.emplace_back(PPoint(656.0f, 390.0f, scale));
		node18ToMux5.emplace_back(PPoint(800.25f, 390.0f, scale));
		node18ToMux5.emplace_back(PPoint(800.25f, 440.0f, scale));
		node18ToMux5.emplace_back(PPoint(1202.5f, 440.0f, scale));
		node18ToMux5.emplace_back(PPoint(1202.5f, 493.25f, scale));
		pathPolyLines.push_back({ PPolyLines(("Node18ToMux5"),node18ToMux5,(int)node18ToMux5.size(),color2,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> andToMux6;
		andToMux6.emplace_back(PPoint(930.25f, 335.0f, scale));
		andToMux6.emplace_back(PPoint(987.25f, 335.0f, scale));
		andToMux6.emplace_back(PPoint(987.25f, 183.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("AndToMux6"),andToMux6,(int)andToMux6.size(),color2,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> andPoly;
		andPoly.emplace_back(PPoint(907.25f, 360.0f, scale));
		andPoly.emplace_back(PPoint(870.25f, 360.0f, scale));
		andPoly.emplace_back(PPoint(870.25f, 310.0f, scale));
		andPoly.emplace_back(PPoint(907.25f, 310.0f, scale));
		pathPolyLines.push_back({ PPolyLines(("AndPoly"),andPoly,(int)andPoly.size(),color2,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> sl1In;
		sl1In.emplace_back(PPoint(406.5f, 85.0f, scale));
		sl1In.emplace_back(PPoint(412.25f, 97.25f, scale));
		pathPolyLines.push_back({ PPolyLines(("SL1In"),sl1In,(int)sl1In.size(),color2,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> sl1Out;
		sl1Out.emplace_back(PPoint(480.5f, 85.0f, scale));
		sl1Out.emplace_back(PPoint(486.25f, 97.25f, scale));
		pathPolyLines.push_back({ PPolyLines(("SL1Out"),sl1Out,(int)sl1Out.size(),color2,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> signExtIn;
		signExtIn.emplace_back(PPoint(516.5f, 684.5f, scale));
		signExtIn.emplace_back(PPoint(521.6f, 697.00f, scale));
		pathPolyLines.push_back({ PPolyLines(("SignExtIn"),signExtIn,(int)signExtIn.size(),color2,ImDrawFlags_None,thickness) });

		std::vector<ImVec2> signExtOut;
		signExtOut.emplace_back(PPoint(612.18f, 684.0f, scale));
		signExtOut.emplace_back(PPoint(619.75f, 696.5f, scale));
		pathPolyLines.push_back({ PPolyLines(("SignExtOut"),signExtOut,(int)signExtOut.size(),color2,ImDrawFlags_None,thickness) });
	}

	// Texts
	{
		color = ImU32(0xffcaf400);
		pathTexts.push_back({ PText("Alu1Inc",177.f,143.f,scale,color,"2",20.0f) });
		pathTexts.push_back({ PText("InsMemName",175.f,545.f,scale,color,"Instruction\nMemory",15.0f) });
		pathTexts.push_back({ PText("InsMemIns",203.5f,502.f,scale,color,"Instruction\n         [15-0]",12.0f) });
		pathTexts.push_back({ PText("InsMemRead",180.f,454.f,scale,color,"Read\nAddress",12.0f) });
		pathTexts.push_back({ PText("PC",82.f,450.f,scale,color,"PC",20.0f) });
		pathTexts.push_back({ PText("ALU1",247.f,113.f,scale,color,"Add",20.0f) });
		pathTexts.push_back({ PText("SL1",430.f,70.5f,scale,color,"Shift\nLeft 1",15.0f) });
		pathTexts.push_back({ PText("SL2",692.f,242.5f,scale,color,"Shift\nLeft 1",15.0f) });
		pathTexts.push_back({ PText("ControlIns",370.f,255.5f,scale,color,"Instruction\n    [15-12]",12.0f) });
		pathTexts.push_back({ PText("ReadReg1Ins",387.f,395.5f,scale,color,"Instruction\n    [11-9]",12.0f) });
		pathTexts.push_back({ PText("ReadReg2Ins",387.f,435.5f,scale,color,"Instruction\n    [8-6]",12.0f) });
		pathTexts.push_back({ PText("Mux2Ins",387.f,510.5f,scale,color,"Inst.\n [5-3]",12.0f) });
		pathTexts.push_back({ PText("SignExtIns",410.f,670.5f,scale,color,"Inst.   [5-0]",12.0f) });
		pathTexts.push_back({ PText("AluControlIns",495.f,735.5f,scale,color,"Inst. [2-0]",12.0f) });
		pathTexts.push_back({ PText("SignExt",545.f,670.5f,scale,color,"Sign\nExtend",15.0f) });
		pathTexts.push_back({ PText("RegName",530.f,588.5f,scale,color,"Registers",25.0f) });
		pathTexts.push_back({ PText("RegReadReg1",511.f,423.5f,scale,color,"Read\nRegister I",12.0f) });
		pathTexts.push_back({ PText("RegReadReg2",511.f,465.5f,scale,color,"Read\nRegister II",12.0f) });
		pathTexts.push_back({ PText("RegWriteReg",511.f,525.5f,scale,color,"Write\nRegister",12.0f) });
		pathTexts.push_back({ PText("RegWriteData",511.f,560.5f,scale,color,"Write\nData",12.0f) });
		pathTexts.push_back({ PText("RegReadData1",605.f,440.5f,scale,color,"Read\nData I",12.0f) });
		pathTexts.push_back({ PText("RegReadData2",605.f,520.5f,scale,color,"Read\nData II",12.0f) });
		pathTexts.push_back({ PText("AluControlName",770.f,645.5f,scale,color,"ALU\nControl",15.0f) });
		pathTexts.push_back({ PText("Alu2",815.f,490.5f,scale,color,"ALU",18.0f) });
		pathTexts.push_back({ PText("Alu2AluRes",835.5f,510.5f,scale,color,"ALU\nResult",12.0f) });
		pathTexts.push_back({ PText("Alu2AluZero",848.5f,473.5f,scale,color,"Zero",12.0f) });
		pathTexts.push_back({ PText("Alu3",815.5f,205.5f,scale,color,"Add",20.0f) });
		pathTexts.push_back({ PText("DataMemName",945.5f,540.5f,scale,color,"Data\nMemory",20.0f) });
		pathTexts.push_back({ PText("DataMemAddress",940.f,520.5f,scale,color,"Adress",13.0f) });
		pathTexts.push_back({ PText("DataMemWrite",940.f,592.5f,scale,color,"Write\nData",13.0f) });
		pathTexts.push_back({ PText("DataMemRead",990.f,507.5f,scale,color,"Read\nData",13.0f) });
		pathTexts.push_back({ PText("SL1Ins",310.f,70.5f,scale,color,"Instruction [11-0]",12.0f) });
		pathTexts.push_back({ PText("SL1Jmp",490.f,70.5f,scale,color,"Jump Address [15-0]",12.0f) });
		pathTexts.push_back({ PText("SL1PC",575.f,97.5f,scale,color,"PC + 2[15-12]",12.0f) });
		pathTexts.push_back({ PText("SL1In",400.f,97.5f,scale,color,"12",12.0f) });
		pathTexts.push_back({ PText("SL1Out",480.f,97.5f,scale,color,"13",12.0f) });
		pathTexts.push_back({ PText("SignExtIn",512.55f,669.0f,scale,color,"6",12.0f) });
		pathTexts.push_back({ PText("SignExtOut",605.f,669.0f,scale,color,"16",12.0f) });
		
		pathTexts.push_back({ PText("MUX1",435.f,597.5f,scale,color,"0\nM\nU\nX\n1",11.0f) });
		pathTexts.push_back({ PText("MUX2",459.f,500.5f,scale,color,"0\nM\nU\nX\n1",11.0f) });
		pathTexts.push_back({ PText("MUX3",740.f,514.5f,scale,color,"0\nM\nU\nX\n1",11.0f) });
		pathTexts.push_back({ PText("MUX4",1089.f,498.5f,scale,color,"1\nM\nU\nX\n0",11.0f) });
		pathTexts.push_back({ PText("MUX5",1200.f,498.5f,scale,color,"1\nM\nU\nX\n0",11.0f) });
		pathTexts.push_back({ PText("MUX6",984.f,113.5f,scale,color,"0\nM\nU\nX\n1",11.0f) });
		pathTexts.push_back({ PText("MUX7",1059.f,113.5f,scale,color,"1\nM\nU\nX\n0",11.0f) });
		pathTexts.push_back({ PText("MUX8",1128.f,194.5f,scale,color,"0\nM\nU\nX\n1",11.0f) });

		pathTexts.push_back({ PText("ControlName",458.f,280.5f,scale,color,"Control",25.0f) });

		pathTexts.push_back({ PText("Mux1Tri2",353.f,635.5f,scale,color,"0b111",12.0f) });

		pathTexts.push_back({ PText("RegDstControl",538.5f,177.f,scale,color,"RegDst",12.0f) });
		pathTexts.push_back({ PText("JumpControl",570.5f,195.f,scale,color,"Jump",12.0f) });
		pathTexts.push_back({ PText("JumpRegControl",570.5f,213.f,scale,color,"JumpReg",12.0f) });
		pathTexts.push_back({ PText("BranchControl",570.5f,237.f,scale,color,"Branch",12.0f) });
		pathTexts.push_back({ PText("MemReadControl",570.5f,260.f,scale,color,"MemRead",12.0f) });
		pathTexts.push_back({ PText("MemToRegControl",570.5f,283.f,scale,color,"MemToReg",12.0f) });
		pathTexts.push_back({ PText("AluOPControl",570.5f,304.f,scale,color,"ALUOP",12.0f) });
		pathTexts.push_back({ PText("MemWriteControl",570.5f,324.f,scale,color,"MemWrite",12.0f) });
		pathTexts.push_back({ PText("AluSrcControl",570.5f,344.f,scale,color,"ALUSrc",12.0f) });
		pathTexts.push_back({ PText("RegWriteControl",553.5f,364.f,scale,color,"RegWrite",12.0f) });
		pathTexts.push_back({ PText("JLinkControl",615.5f,375.f,scale,color,"JLink",12.0f) });

	}
	pathHalfEllipses.push_back({ PHalfEllipse("AndGate",scale,905.26f,335.0f,25.f,25.f,4.7f,color2,64,thickness) });

	noblePathOrig->SetPathRects(pathRects);
	noblePathOrig->SetPathEllipses(pathEllipses);
	noblePathOrig->SetPathPolyLines(pathPolyLines);
	noblePathOrig->SetPathTriangles(pathTriangles);
	noblePathOrig->SetPathCircles(pathCircles);
	noblePathOrig->SetPathTexts(pathTexts);
	noblePathOrig->SetPathHalfEllipses(pathHalfEllipses);

	noblePathTemp->SetPathRects(pathRects);
	noblePathTemp->SetPathEllipses(pathEllipses);
	noblePathTemp->SetPathPolyLines(pathPolyLines);
	noblePathTemp->SetPathTriangles(pathTriangles);
	noblePathTemp->SetPathCircles(pathCircles);
	noblePathTemp->SetPathTexts(pathTexts);
	noblePathTemp->SetPathHalfEllipses(pathHalfEllipses);

	m_ShapesVec.push_back(noblePathOrig);
	m_ShapesVec.push_back(noblePathTemp);

	
	std::string name;
	std::string pathStr;
	int index;
	int group;
	io::CSVReader<4>reader("data/InstructionPaths.csv");
	reader.read_header(io::ignore_missing_column, "index", "name", "group", "path_string");
	while (reader.read_row(index, name, group, pathStr)) {
		InstructionPath newPath2(index,group,name,pathStr,noblePathOrig,noblePathTemp);
		newPath2.Init();
		m_InstructionPaths[group] = newPath2;
		//CL_CORE_INFO("Init {}", newPath2.m_PathStr);
	}
	
}

void swap(ImVec2& cord)
{
	float temp = cord.x;
	cord.x = cord.y;
	cord.y = temp;
}

void ImDrawer::ITransform()
{
	////PathShapes& noblePath = m_ShapesVec[0];
	//for (auto& rects : noblePath.GetPathRects()) {
	//	swap(rects.m_UpperLeft);
	//	swap(rects.m_LowerRight);
	//}

	//for (auto& ellipses : noblePath.GetPathEllipses()) {
	//	swap(ellipses.m_Center);
	//	float temp = ellipses.m_RadiusX;
	//	ellipses.m_RadiusX = ellipses.m_RadiusY;
	//	ellipses.m_RadiusY = temp;
	//}

	////AddHalfEllipse(m_DrawList, p + ImVec2(905.26f * m_Scale, 335.0f * m_Scale), 25 * m_Scale, 25 * m_Scale, 4.7f, IM_COL32(0xff, 0xff, 0xff, 0xff), 64, 1.0);

	//for (auto& polylines : noblePath.GetPathPolyLines()) {
	//	for (auto& points : polylines.m_Points) {
	//		swap(points);
	//	}
	//}
	//for (auto& triangles : noblePath.GetPathTriangles()) {
	//	swap(triangles.m_Point1);
	//	swap(triangles.m_Point2);
	//	swap(triangles.m_Point3);
	//}
	//for (auto& circles : noblePath.GetPathCircles()) {
	//	swap(circles.m_Center);
	//}

	//for (auto& pathTexts : noblePath.GetPathTexts()) {
	//	swap(pathTexts.m_Position);
	//}
}

void AddEllipse(ImDrawList* _ptrDrawList, const ImVec2& _center, float _radiusA, float _radiusB, float _tilt, const ImColor& _color, int _numSegments, float _thickness) {
	// Refer from https://github.com/kuravih/imgui_canvas
	ImVector<ImVec2> points;
	points.resize(_numSegments);

	float r, theta, dtheta =  2 * IM_PI / _numSegments;
	// float ecc2=1-ImPow(_radiusA/_radiusB,2);
	for (int i = 0; i < _numSegments; i++) {
		theta = dtheta * i;
		// theta = theta - (ecc2/8 + ImPow(ecc2,2)/16 + 71*ImPow(ecc2,3)/2048)*ImSin(2*theta) + (5*(ImPow(ecc2,2) + ImPow(ecc2,3))/256)*ImSin(4*theta)  + (29*ImPow(ecc2,3)/6144)*ImSin(6*theta); // https://math.stackexchange.com/questions/2093569/points-on-an-ellipse
		r = _radiusA * _radiusB / ImSqrt(ImPow(_radiusA * ImCos(theta), 2) + ImPow(_radiusB * ImSin(theta), 2));
		points[i] = _center + ImVec2(r * ImCos(theta + _tilt), r * ImSin(theta + _tilt));
	}

	_ptrDrawList->AddPolyline(points.Data, _numSegments, _color, ImDrawFlags_Closed, _thickness);
}

