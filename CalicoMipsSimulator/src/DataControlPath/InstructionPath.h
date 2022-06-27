#pragma once
#include "PathShapes.h"
#include "Log.h"
class InstructionPath {
public: 
	InstructionPath() = default;
	InstructionPath(int index, int group, std::string name, std::string pathStr, PathShapes* pathPtrOrig, PathShapes* pathPtrTemp) :
	m_Index(index),m_Group(group),m_Name(name),m_PathStr(pathStr),m_PathPtrOrig(pathPtrOrig),m_PathPtrTemp(pathPtrTemp)
	{};
private:
	int m_Index;
	int m_Group;
	std::string m_Name;
	std::string m_PathStr;
	PathShapes* m_PathPtrOrig;
	PathShapes* m_PathPtrTemp;

	std::vector <PathRect*> m_PathRects;
	std::vector <PathEllipse*> m_PathEllipses;
	std::vector <PathHalfEllipse*> m_PathHalfEllipses;
	std::vector <PathPolyLine*> m_PathPolyLines;
	std::vector <PathTriangle*> m_PathTriangles;
	std::vector <PathCircle*> m_PathCircles;
	std::vector <PathText*> m_PathTexts;
	

public:
	void Init();
	void Activate();
private:
	void Sync();
};