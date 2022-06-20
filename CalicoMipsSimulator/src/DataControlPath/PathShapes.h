#pragma once
#include "imgui.h"
#include <vector>
#include "PathElements.h"
#include <string>
class PathShapes {
public:
	PathShapes(std::string& pathName) {
		m_PathName = pathName;
	}
	
	std::vector<PathRect>& GetPathRects() { return m_PathRects; }
	std::vector<PathTriangle>& GetPathTriangles() { return m_PathTriangles; }
	std::vector<PathLine>& GetPathLines() { return m_PathLines; }
	std::vector<PathPolyLine>& GetPathPolyLines() { return m_PathPolyLines; }
	std::vector<PathEllipse>& GetPathEllipses() { return m_PathEllipses; }
	std::vector<PathCircle>& GetPathCircles() { return m_PathCircles; }
	std::vector<PathText>& GetPathTexts() { return m_PathTexts; }
	std::string& GetPathName() { return m_PathName; }
	
	void SetPathRects(std::vector<PathRect>& pathRects) { m_PathRects = pathRects; }
	void SetPathTriangles(std::vector<PathTriangle>& pathTriangles) { m_PathTriangles = pathTriangles; }
	void SetPathLines(std::vector<PathLine>& pathLines) { m_PathLines= pathLines; }
	void SetPathPolyLines(std::vector<PathPolyLine>& pathPolyLines) { m_PathPolyLines = pathPolyLines; }
	void SetPathEllipses(std::vector<PathEllipse>& pathEllipses) { m_PathEllipses = pathEllipses; }
	void SetPathCircles(std::vector<PathCircle>& pathCircles) { m_PathCircles = pathCircles; }
	void SetPathTexts(std::vector<PathText>& pathTexts) { m_PathTexts = pathTexts; }
	void SetPathName(std::string& pathName) { m_PathName = pathName; }
	
private:
	std::vector<PathRect> m_PathRects;
	std::vector<PathTriangle> m_PathTriangles;
	std::vector<PathLine> m_PathLines;
	std::vector<PathPolyLine> m_PathPolyLines;
	std::vector<PathEllipse> m_PathEllipses;
	std::vector<PathCircle> m_PathCircles;
	std::vector<PathText> m_PathTexts;
	std::string m_PathName;
};


