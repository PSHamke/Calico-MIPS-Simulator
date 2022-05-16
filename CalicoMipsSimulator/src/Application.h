#pragma once
#include <string>
#include <iostream>
#include <Windows.h>
#include <thread>
#include <d3d9.h>
#include <imgui.h>
#include "Platform\DirectX-Win32\imgui_impl_dx9.h"
#include "Platform\\DirectX-Win32\imgui_impl_win32.h"



class Application
{
public:
	struct ApplicationSpecification
	{
		std::string Name = "Walnut App";
		uint32_t Width = 1600;
		uint32_t Height = 900;
	};
	Application(const ApplicationSpecification& applicationSpecification = ApplicationSpecification());
	~Application();

	void Run();
	void Close();
private:
	void Init();
	void Shutdown();
public:
	static bool CreateDeviceD3D(HWND hWnd);
	static void CleanupDeviceD3D();
	static void ResetDevice();
private:
	ApplicationSpecification m_Specification;
	bool m_Running = false;
	WNDCLASSEX wc;
	HWND hWnd;
	MSG msg;
};
Application* CreateApplication(int argc, char** argv);

