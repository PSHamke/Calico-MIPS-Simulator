#include "Application.h"
#include "font.h"
#include "iconcpp.h"
#include "TextEditor.h"
#include "menu.h"
#include "Log.h"
#include "Core.h"
#include "MemoryView.h"
#include "RegisterView.h"
static bool Enabled = true;


LPDIRECT3DDEVICE9        g_pd3dDevice;
D3DPRESENT_PARAMETERS    g_d3dpp;
LPDIRECT3D9              g_pD3D;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool InitEditors();

ImFont* Consolas = nullptr;
ImFont* Consolas2 = nullptr;



Application::Application(const ApplicationSpecification& applicationSpecification /*= ApplicationSpecification()*/)
{
	this->m_Specification = applicationSpecification;
	Init();
}

Application::~Application()
{

}

void Application::Init()
{
	wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(0), 0, 0, 0, 0, L"Calico|MIPS Simulator", 0 };
	RegisterClassEx(&wc);
	hWnd = CreateWindow(wc.lpszClassName, L"Calico|MIPS Simulator", WS_POPUP, 0, 0, 5, 5, 0, 0, wc.hInstance, 0);

	if (!CreateDeviceD3D(hWnd)) {
		CleanupDeviceD3D();
		UnregisterClass(wc.lpszClassName, wc.hInstance);
		return;
	}

	

#if CL_DEBUG

	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, 5);
	Log::Init();
#endif

	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
	ImFontConfig icons_config;

	ImFontConfig CustomFont;
	CustomFont.FontDataOwnedByAtlas = false;


	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 2.5;
	icons_config.OversampleV = 2.5;

	io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 21.f, &CustomFont);
	io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 19.0f, &icons_config, icons_ranges);
	Consolas = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Consola.ttf", 17);
	io.Fonts->AddFontDefault();	
	Consolas2 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Consola.ttf", 12);
	

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(g_pd3dDevice);
	ZeroMemory(&msg, sizeof(msg));

	CL_CORE_ASSERT(InitEditors(), "Failed to initialize editors");
}

void Application::Run()
{
	
	int8_t a = 200;
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		Menu::Theme();
		ImGui::NewFrame();
		
		ImGui::Begin("CalicoX", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
		{
			//CL_CORE_INFO("Running!");
			Menu::TitleBar(msg);
			Menu::Render();
		}
		ImGui::End();
		
		ImGui::EndFrame();

		g_pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
		if (g_pd3dDevice->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			g_pd3dDevice->EndScene();
		}

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		HRESULT result = g_pd3dDevice->Present(0, 0, 0, 0);
		if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
			ResetDevice();
		}
		if (!Enabled) {
			msg.message = WM_QUIT;
		}
	}
	Shutdown();
}

void Application::Shutdown()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	TextEditor::FreeAllInstances();
	
	CleanupDeviceD3D();
	DestroyWindow(hWnd);
	UnregisterClass(wc.lpszClassName, wc.hInstance);
}

bool Application::CreateDeviceD3D(HWND hWnd)
{
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return false;
	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
		return false;
	return true;
}

void Application::CleanupDeviceD3D()
{
	if (g_pd3dDevice) {
		g_pd3dDevice->Release(); g_pd3dDevice = NULL;
	}
	if (g_pD3D) {
		g_pD3D->Release(); g_pD3D = NULL;
	}
}

void Application::ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT Result = g_pd3dDevice->Reset(&g_d3dpp);
	if (Result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

void Application::Close()
{
	Shutdown();
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != 0 && wParam != SIZE_MINIMIZED)
		{
			g_d3dpp.BackBufferWidth = LOWORD(lParam);
			g_d3dpp.BackBufferHeight = HIWORD(lParam);
			Application::ResetDevice();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Application* CreateApplication(int argc, char** argv)
{

	Application::ApplicationSpecification spec;
	float aspectRatio = 16 / 9.f;
	spec.Name = "Walnut Calico Example";
	spec.Width = 1500;
	spec.Height = spec.Width / aspectRatio;
	Application* app = new Application(spec);
	
	return app;
}

bool InitEditors() {
	
	TextEditor* mainEditor    = TextEditor::CreateInstance("##MainEditor");
	TextEditor* dataEditor    = TextEditor::CreateInstance("##DataEditor");
	TextEditor* cOutputEditor = TextEditor::CreateInstance("##COutputEditor");
	TextEditor* outputEditor  = TextEditor::CreateInstance("##OutputEditor");
	
	TextEditor* mainEditorNoble = TextEditor::CreateInstance("##NobleMainEditor");
	TextEditor* dataEditorNoble = TextEditor::CreateInstance("##NobleDataEditor");
	TextEditor* cOutputEditorNoble = TextEditor::CreateInstance("##NobleCOutputEditor");
	TextEditor* outputEditorNoble = TextEditor::CreateInstance("##NobleOutputEditor");

	if (!(mainEditor && dataEditor && cOutputEditor && outputEditor)) {
		return false;
	}
	
	MemoryView* memoryView = MemoryView::CreateInstance("##MainMemoryView");
	memoryView->SetFont(Consolas2);

	RegisterView* registerView = RegisterView::CreateInstance("##MainRegisterView");
	registerView->SetFont(Consolas2);

	MemoryView* memoryViewNoble = MemoryView::CreateInstance("##NobleMainMemoryView");
	memoryViewNoble->SetFont(Consolas2);

	RegisterView* registerViewNoble = RegisterView::CreateInstance("##NobleMainRegisterView");
	registerViewNoble->SetFont(Consolas2);
	
	
	mainEditor->SetStartText(".text");
	mainEditor->SetStartSegmentValue(0x4000000);
	mainEditor->SetText("main:\n\t");
	mainEditor->SetShowWhitespaces(false);
	mainEditor->SetReadOnly(false);
	mainEditor->SetPalette(TextEditor::GetDarkPalette());
	mainEditor->SetLanguageDefinition(TextEditor::LanguageDefinition::MIPS());
	
	
	dataEditor->SetStartText(".data");
	dataEditor->SetStartSegmentValue(0x1001000);
	dataEditor->SetShowWhitespaces(false);
	dataEditor->SetReadOnly(false);
	dataEditor->SetPalette(TextEditor::GetDarkPalette());
	dataEditor->SetLanguageDefinition(TextEditor::LanguageDefinition::MIPS());
	
	
	cOutputEditor->SetStartText("C Output");
	cOutputEditor->SetStartSegmentValue(0x4000000);
	cOutputEditor->SetShowWhitespaces(false);
	cOutputEditor->SetReadOnly(true);
	cOutputEditor->SetPalette(TextEditor::GetDarkPalette());
	cOutputEditor->SetLanguageDefinition(TextEditor::LanguageDefinition::C());
	cOutputEditor->SetFlag(Editor_No_Cursor);
	outputEditor->SetStartText("Output");
	outputEditor->SetStartSegmentValue(0x1001000);
	outputEditor->SetShowWhitespaces(false);
	outputEditor->SetReadOnly(true);
	outputEditor->SetPalette(TextEditor::GetDarkPalette());
	outputEditor->SetLanguageDefinition(TextEditor::LanguageDefinition::C());
	outputEditor->SetFlag(Editor_No_LineNumbers | Editor_No_Cursor | Editor_No_LineHiglight);

	mainEditorNoble->SetStartText(".text");
	mainEditorNoble->SetStartSegmentValue(0x4000000);
	mainEditorNoble->SetText("main:\n\t");
	mainEditorNoble->SetShowWhitespaces(false);
	mainEditorNoble->SetReadOnly(false);
	mainEditorNoble->SetPalette(TextEditor::GetDarkPalette());
	mainEditorNoble->SetLanguageDefinition(TextEditor::LanguageDefinition::Noble16());


	dataEditorNoble->SetStartText(".data");
	dataEditorNoble->SetStartSegmentValue(0x1001000);
	dataEditorNoble->SetShowWhitespaces(false);
	dataEditorNoble->SetReadOnly(false);
	dataEditorNoble->SetPalette(TextEditor::GetDarkPalette());
	dataEditorNoble->SetLanguageDefinition(TextEditor::LanguageDefinition::Noble16());


	cOutputEditorNoble->SetStartText("C Output");
	cOutputEditorNoble->SetStartSegmentValue(0x4000000);
	cOutputEditorNoble->SetShowWhitespaces(false);
	cOutputEditorNoble->SetReadOnly(true);
	cOutputEditorNoble->SetPalette(TextEditor::GetDarkPalette());
	cOutputEditorNoble->SetLanguageDefinition(TextEditor::LanguageDefinition::C());
	cOutputEditorNoble->SetFlag(Editor_No_Cursor);
	outputEditorNoble->SetStartText("Output");
	outputEditorNoble->SetStartSegmentValue(0x1001000);
	outputEditorNoble->SetShowWhitespaces(false);
	outputEditorNoble->SetReadOnly(true);
	outputEditorNoble->SetPalette(TextEditor::GetDarkPalette());
	outputEditorNoble->SetLanguageDefinition(TextEditor::LanguageDefinition::C());
	outputEditorNoble->SetFlag(Editor_No_LineNumbers | Editor_No_Cursor | Editor_No_LineHiglight);
	
	return true;

}
