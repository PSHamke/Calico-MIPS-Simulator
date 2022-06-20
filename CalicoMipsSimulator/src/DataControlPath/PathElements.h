#pragma once
#include <string>
class PathRect {
public:
	PathRect(std::string& name,ImVec2& upperLeft, ImVec2& lowerRight, ImU32 color, float rounding = 0.0f, ImDrawFlags flags = 0, float thickness = 1.0f) :
		m_Name(name),m_UpperLeft(upperLeft), m_LowerRight(lowerRight), m_Color(color), m_Rounding(rounding), m_Flags(flags), m_Thickness(thickness) {}

public:
	std::string m_Name;
	ImVec2 m_UpperLeft;
	ImVec2 m_LowerRight;
	ImU32 m_Color;
	float m_Rounding;
	ImDrawFlags m_Flags;
	float m_Thickness;
};

class PathTriangle {
public:
	PathTriangle(std::string& name, ImVec2& point1, ImVec2& point2, ImVec2& point3, ImU32 color, float thickness = 1.0f) :
		m_Name(name), m_Point1(point1), m_Point2(point2), m_Point3(point3), m_Color(color), m_Thickness(thickness) {}
public:
	std::string m_Name;
	ImVec2 m_Point1;
	ImVec2 m_Point2;
	ImVec2 m_Point3;
	ImU32 m_Color;
	float m_Thickness;
};

class PathLine {

};

class PathEllipse {
public:
	PathEllipse(std::string& name, ImVec2& center, float radiusX,float radiusY,float angle, ImU32 color,int numSegments = 64, float thickness = 1.0f) :
		m_Name(name), m_Center(center), m_RadiusX(radiusX), m_RadiusY(radiusY), m_Angle(angle), m_Color(color),m_NumSegments(numSegments),m_Thickness(thickness) {}

public:
	std::string m_Name;
	ImVec2 m_Center;
	float m_RadiusX;
	float m_RadiusY;
	float m_Angle;
	ImU32 m_Color;
	int m_NumSegments;
	float m_Thickness;
};

class PathCircle {
public:
	PathCircle(std::string& name, ImVec2& center, float radius, ImU32 color, int numSegments = 32, float thickness = 1.0f) :
		m_Name(name), m_Center(center), m_Radius(radius), m_Color(color), m_NumSegments(numSegments), m_Thickness(thickness) {}
public:
	std::string m_Name;
	ImVec2 m_Center;
	float m_Radius;
	ImU32 m_Color;
	int m_NumSegments;
	float m_Thickness;
};

class PathPolyLine {
public:
	PathPolyLine(std::string& name, std::vector<ImVec2>& points, int numPoints, ImU32 color,ImDrawFlags flags, float thickness = 1.0f) :
		m_Name(name), m_Points(points), m_PointsCount(numPoints), m_Color(color),m_Flags(flags), m_Thickness(thickness) {}
public:
	std::string m_Name;
	std::vector<ImVec2> m_Points;
	int m_PointsCount;
	ImU32 m_Color;
	ImDrawFlags m_Flags;
	float m_Thickness;
};

class PathText {
public:
	PathText(std::string& name, ImVec2& position, ImU32 color, const char* text,float fontSize) :
		m_Name(name), m_Position(position), m_Color(color), m_Data(text), m_FontSize(fontSize) {}
public:
	std::string m_Name;
	ImVec2 m_Position;
	ImU32 m_Color;
	const char* m_Data;
	float m_FontSize;
};
