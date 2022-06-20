#pragma once
#include  "imgui.h"
#include "PathShapes.h"
#include  <vector>
#include "csv.hpp"

class ImDrawer {
public:
	ImDrawer(const ImDrawer&) = delete;

	static ImDrawer& Get() {
		static ImDrawer instance;
		return instance;
	}
	static void SetDrawList(ImDrawList* drawList) {
		Get().ISetDrawList(drawList);
	}
	static void SetScaleRatio(float scaleRatio) {
		Get().ISetScaleRatio(scaleRatio);
	}
	static void Draw() {
		Get().IDraw();
	}
	static void Transform() {
		Get().ITransform();
	}
private:
	ImDrawer(){
		Init();
	}
	void ISetDrawList(ImDrawList* drawList) {
		m_DrawList = drawList;
	}
	void ISetScaleRatio(float scaleRatio) {
		m_Scale = scaleRatio;
	}
	void IDraw();
	void Init();
	void ITransform();
private:
	ImDrawList* m_DrawList;
	std::vector<PathShapes> m_ShapesVec;
	float m_Scale;
	
};


// Ellipse drawing 
void AddEllipse(ImDrawList* _ptrDrawList, const ImVec2& _center, float _radiusA, float _radiusB, float _tilt, const ImColor& _color, int _numSegments = 64, float _thickness = 1.0f);

#define PRect(name,sc,xul,yul,xlr,ylr,col,rd,fl,th)         std::string(name), ImVec2(xul* sc, +yul * sc), ImVec2(xlr * sc, ylr * sc), col,rd, fl, th
#define PRdRect(name,sc,xul,yul,xlr,ylr,col,rd,fl,th)       std::string(name), ImVec2(xul* sc, +(yul-rd)* sc), ImVec2(xlr * sc, (ylr+rd)* sc), col,rd, fl, th
#define PEllipse(name,sc,cx,cy,rx,ry,ang,col,segn,th)		std::string(name), ImVec2(cx*sc,cy*sc),rx*sc,ry*sc,ang,col,segn,th
#define PPolyLines(name,path,col,fl,th)						std::string(name), path, col, fl, th
#define PTriangle(name,p1x,p1y,p2x,p2y,p3x,p3y,sc,col,th)	std::string(name), ImVec2(p1x*sc,p1y*sc),ImVec2(p2x*sc,p2y*sc),ImVec2(p3x*sc,p3y*sc), col, th
#define PCircle(name,cx,cy,r,sc,col,th)                     std::string(name), ImVec2(cx*sc,cy*sc), r*sc, col,th
#define PText(name,xul,yul,sc,col,text,fs)						std::string(name), ImVec2(xul*sc,yul*sc),col,text,fs
#define PPoint(x,y,sc)										x*sc, y*sc
#define PSTriangle(name, xu,yu,l,sc,col,segn,th)					PTriangle(name, xu,yu, (xu+l),(yu+(l/2)), xu,(yu+l),sc, col,segn,th)