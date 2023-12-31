#include "gui.h"
#include <cstdlib>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_glfw.h"//del

#include "globals.h"
#include <corecrt_math.h>
#include "../font/icons.h"
#include "../font/font.h"
#include "../DiscordRpc/Class/Discord.h"

#include <windows.h>
#include <shellapi.h>
#include "../imgui/imgui_toggle.h"
#include "imguidesign.h"//del
#include <iostream> // Include for console output (optional)
#include <fstream>  // Include for file stream operations
#include <string>
#include <Lmcons.h>
#include <chrono>
#include <cmath>
#include <vector>//del


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

long __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;

	switch (message)
	{
	case WM_SIZE: {
		if (gui::device && wideParameter != SIZE_MINIMIZED)
		{
			gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
			gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
			gui::ResetDevice();
		}
	}return 0;

	case WM_SYSCOMMAND: {
		if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;

	case WM_LBUTTONDOWN: {
		gui::position = MAKEPOINTS(longParameter); // set click points
	}return 0;

	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{ };

			GetWindowRect(gui::window, &rect);

			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			if (gui::position.x >= 0 &&
				gui::position.x <= gui::WIDTH &&
				gui::position.y >= 0 && gui::position.y <= 19)
				SetWindowPos(
					gui::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
		}

	}return 0;

	}

	return DefWindowProc(window, message, wideParameter, longParameter);
}

void gui::CreateHWindow(const char* windowName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = "class001";
	windowClass.hIconSm = 0;

	RegisterClassEx(&windowClass);

	window = CreateWindowEx(
		0,
		"class001",
		windowName,
		WS_POPUP,
		100,
		100,
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0)
		return false;

	return true;
}

void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO(); (void)io;

	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowTitleAlign = { 0.5f, 0.5f };
	style->WindowPadding = { 15, 15 };
	style->ChildRounding = 2.f;
	style->WindowRounding = 0.f;
	style->ScrollbarRounding = 1.f;
	style->FrameRounding = 2.f;
	style->ItemSpacing = { 8, 8 };
	style->ScrollbarSize = 3.f;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.96f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.11f, 0.11f, 0.14f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.32f, 0.32f, 0.58f, 0.30f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.17f, 0.00f, 0.52f, 0.26f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.27f, 0.38f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.29f, 0.37f, 0.62f, 0.54f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.33f, 0.33f, 0.68f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.81f, 0.66f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.35f, 0.37f, 0.48f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.33f, 0.35f, 0.49f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.42f, 0.32f, 0.67f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.50f, 0.41f, 0.73f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.58f, 0.50f, 1.00f, 0.35f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.29f, 0.84f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.24f, 0.80f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
	ImFontConfig icons_config;

	ImFontConfig CustomFont;
	CustomFont.FontDataOwnedByAtlas = false;

	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 3;
	icons_config.OversampleV = 3;

	io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 20, &CustomFont);
	io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 32.5f, &icons_config, icons_ranges);

	io.Fonts->AddFontDefault();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
		{
			isRunning = !isRunning;
			return;
		}
	}

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}
//bools
static bool showing = true;
static float size = 450;
static bool my_bool;
bool show_circle_overlay = false;
HWND target_hwnd;
bool show_exit_button = true;
static auto current_tab = 0;

static bool values[] = { true, true, true, true, true, true, true, true };
size_t value_index = 0;
bool my_checkbox_state = false;
constexpr auto butn_tall = 48;
int buttonPressCount = 0;

int status = system("tasklist | find /i \"csgo.exe\" > nul");

//bools
static bool circleshowing = true;//del
static bool fovcircle = false;//del
static float circlesize = 450;//del
bool openNewWindow = false;

//del
//skinchanger stuff
const char* ak_skins[] = { "None", "Neon Rider", "Bloodsport", "Wasteland Rebel", "Fuel Injector", "Vulcan", "Frontside Misty", "Point Disarray", "Redline", "Uncharted", "Elite Build" };
const char* m4_skins[] = { "None", "Howl", "The Emperor", "Neo-Noir", "Buzz Kill", "Cyber Security", "Desolate Space", "Dragon King", "In Living Color", "Asiimov", "Spider Lily" };
const char* m4_silencer_skins[] = { "None", "Printsreen", "Player Two", "Chantico's Fire", "Golden Coil", "Hyber Beast", "Cyrex", "Nightmare", "Leaded Glass", "Decimator", "Icarus Fell" };
const char* awp_skins[] = { "None", "Containment Breach", "Wild Fire", "Neo-Noir", "Hyper Beast", "Asiimov", "Lightning Strike", "Fade", "The Prince", "Gungnir", "Medusa", "Dragon Lore", "Fever Dream", "Elite Build", "Redline", "Electric Hive", "BOOM", "Atheris", "Wrom God", "Ping DDPAT", };
//here


//game launch code
std::string FindCSGOPath() {
	// Default Steam installation path
	const char* steamPath = "D:\\SteamLibrary\\steamapps\\common\\Counter-Strike Global Offensive";

	// Check if CS:GO executable exists
	std::string csgoPath = std::string(steamPath) + "\\csgo.exe";
	if (std::ifstream(csgoPath.c_str())) {
		return csgoPath;
	}

	// If not found, return an empty string
	return "";
}

//rgb stuff (idk i might delete)
float hue = 0.0f;
float colorSpeed = 0.05f; // You can adjust the speed of the color change here

void RenderFlashingRGBText() {
	// Calculate the RGB color based on the current hue
	ImVec4 textColor;
	textColor.x = sinf(hue) * 0.5f + 0.5f;
	textColor.y = sinf(hue + 2.0f * 3.14159265359f / 3.0f) * 0.5f + 0.5f;
	textColor.z = sinf(hue + 4.0f * 3.14159265359f / 3.0f) * 0.5f + 0.5f;
	textColor.w = 1.0f; // Alpha

	// Set the text color
	ImGui::TextColored(textColor, "Status: Undetected");

	// Increase the hue value for the next frame
	hue += colorSpeed;
	if (hue > 1.0f) {
		hue -= 1.0f;
	}
}

void RGBText() {
	// Call the function to render flashing RGB text
	RenderFlashingRGBText();
	// You can call RenderFlashingRGBText() here or in any other function
}

//cheat status message
void CheatStatusMessageBox() {
	MessageBox(NULL, "Cheat Online!", "Quantum CSGO Menu", MB_ICONINFORMATION | MB_OK);
}


void gui::Render() noexcept
{
	//login
	//static ImVec4 active = ImGuiPP::ToVec4(219, 190, 0, 255);
	//static ImVec4 inactive = ImGuiPP::ToVec4(255, 255, 255, 255);
	//ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	//if (ImGui::Begin("Login", NULL, 
		//ImGuiWindowFlags_NoResize |
		//ImGuiWindowFlags_NoSavedSettings))
	//{
		//constexpr auto button_height = 50;
		//ImGuiPP::CenterText("Input Password Below", 1, TRUE);
		//ImGui::InputText("##passwordinput", &inputpassword, CHAR_MAX, ImGuiInputTextFlags_Password);

		//if (ImGui::Button("Login", { ImGui::GetContentRegionAvail().x, button_height }))
		//{
			//if (&inputpassword == password)
			//{
				//logged = true;
			//}
			//del
			//else
			//{
				//ImGui::TextColored(ImColor(232, 17, 35), "Wrong Password or Username. Try again.");
			//}
			//del
		//}

	//}ImGui::End();
	//login end

	//login (not used anymore)
	//if (logged)
	//{
		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
		ImGui::Begin(
			"Quantum CSGO Menu - By MavenCoding157",
			&isRunning,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove
		);

		if (ImGui::BeginChild(
			1,
			{ ImGui::GetContentRegionAvail().x * 0.25f, ImGui::GetContentRegionAvail().y },
			true)) {
			constexpr auto button_height = 60;
			if (ImGui::Button("Home", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 9; }
			ImGui::Spacing();

			if (ImGui::Button("ESP", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 0; }
			ImGui::Spacing();

			if (ImGui::Button("Aim", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 10; }
			ImGui::Spacing();

			if (ImGui::Button("Skin Changer", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 12; }
			ImGui::Spacing();

			if (ImGui::Button("Misc", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 2; }
			ImGui::Spacing();

			if (ImGui::Button("Launch CSGO", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 11; }
			ImGui::Spacing();

			if (ImGui::Button("Other", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 3; }
			ImGui::Spacing();

			if (ImGui::Button("Future Features", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 4; }
			ImGui::Spacing();


			if (ImGui::Button("Settings", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 5; }
			ImGui::Spacing();


			if (ImGui::Button("About", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 6; }
			ImGui::Spacing();


			if (ImGui::Button("CSGO Status", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 8; }
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Only works is application started before hand");
			ImGui::Spacing();

			//colored text
			static float Alpha = 255;
			static bool Tick = false;
			static float Speed = 1.0f;

			if (Tick || Alpha > -255)
			{
				Tick = true;
				if (!(Alpha <= 0))
					Alpha -= Speed;
				else if (Alpha <= 0)
					Tick ^= 1;
			}

			else if (!Tick || Alpha != 255)
			{
				Tick = false;
				if (!(Alpha >= 255))
					Alpha += Speed;
				else if (Alpha >= 255)
					Tick ^= 1;
			}

			if (ImGui::Button("      Creator:\nMavenCoding157", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 7; }

			ImGui::EndChild();
		}

		ImGui::SameLine();

		if (ImGui::BeginChild(
			2,
			ImGui::GetContentRegionAvail(),
			true)) {

			switch (current_tab) {
			case 9:
				ImGui::BeginChild("Home", ImVec2(535, 0), true);
				{
					ImGuiPP::CenterText("Quantum Menu", 1, TRUE);

					ImGui::TextColored(ImColor(220, 190, 0, 255), "Version: 2.0");
					//(for rgb text)
					RenderFlashingRGBText();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGuiPP::CenterText("Cheat Status", 1, TRUE);
					//(The only reason I added this is cause it looks coll btw)
					if (ImGui::Button("Check Cheat Status", { ImGui::GetContentRegionAvail().x, butn_tall })) 
					{
						CheatStatusMessageBox();
					}
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Checks Cheat Status.");


					//static char text[256] = "https://github.com/MavenCoding157/ShooterZ-menu";
					//ImGui::Text("	  API Key [BETA]");
					//ImGui::SetNextItemWidth(147);
					//if (ImGui::InputText("", text, 265, ImGuiInputTextFlags_Password)) {
						// Do something when the text is changed
					//}

					//if (ImGui::Button("Copy", { ImGui::GetContentRegionAvail().x, butn_tall })) {
						//ImGui::SetClipboardText(text);
					//}
					// Draw left section contents
				}
				ImGui::EndChild();
				break;

			case 0:
				ImGuiPP::CenterText("ESP", 1, TRUE);
				ImGui::Toggle("ESP", &globals::glow);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("See players through walls");

				ImGui::ColorEdit4("ESP colour", globals::glowColor);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Choose colours");
				ImGui::Spacing();
				ImGui::Toggle("Radar hack", &globals::radar);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("See players on radar");
				ImGui::Spacing();
				ImGui::Toggle("Chams", &globals::chams);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("See players better");

				break;

			case 10:
				ImGuiPP::CenterText("Aim", 1, TRUE);

				ImGui::Toggle("TriggerBot", &globals::TriggerBot);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Shoots players | hold Left CTRL to use");
				ImGui::SliderInt("Trigger delay", g_Options.trigger_delay, 1, 50);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Trigger Delay [In early BETA stages]");

				ImGui::Spacing();

				ImGui::Toggle("Legit aimbot", &globals::aimbot);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Legit aimbot [BETA] | hold Left CTRL to use");

				ImGui::Spacing();

				ImGui::Toggle("Recoil Control", &globals::RCS);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("RCS [BETA] (Doesnt work atm)");
				ImGui::Spacing();

				ImGui::Text("Dont run aimbot and triggerbot at the \nsame time other wise it might not work.");
				ImGui::Spacing();
				ImGui::Text("Also don't run the skin changer at the same \ntime as any aim hack as it bugs the game out.");
				break;

			case 12:
				ImGuiPP::CenterText("Skin Changer [BETA]", 1, TRUE);

				ImGui::Spacing();
				ImGui::Spacing();
				ImGuiPP::CenterText("Skins Below", 1, TRUE);
				ImGui::Spacing();
				ImGui::Combo("AK47", g_Options.ak_skin, ak_skins, IM_ARRAYSIZE(ak_skins), 0);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Skin Changer");

				
				ImGui::Combo("M4A4", g_Options.m4_skin, m4_skins, IM_ARRAYSIZE(m4_skins), 0);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Skin Changer");

				
				ImGui::Combo("M4A1-S", g_Options.m4_silencer_skin, m4_silencer_skins, IM_ARRAYSIZE(m4_silencer_skins), 0);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Skin Changer");

		
				ImGui::Combo("AWP", g_Options.awp_skin, awp_skins, IM_ARRAYSIZE(awp_skins), 0);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Skin Changer");

				ImGui::Spacing();
				ImGui::Text("Also don't run any aim hacks at the same \ntime as the skin changer as it bugs the game out.");
				break;

			case 2:
				ImGuiPP::CenterText("Misc", 1, TRUE);
				ImGui::BeginChild("Home", ImVec2(150, 0), true);
				{
					ImGui::Toggle("bhop", &globals::bhop);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Makes movement easier");
					ImGui::Spacing();
					ImGui::Toggle("Anti-Flash", &globals::flashDur);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Cant be flashed");
					ImGui::Spacing();
					ImGui::Toggle("Anti-AFK", &globals::AntiAFK);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Makes it so you cant go idle");
					ImGui::Spacing();
					if (ImGui::Button("Spectators List [BETA")) {
						openNewWindow = true; // Set the flag to true when the button is pressed
					}
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Shows People Spectating You [BETA]");

					//new window
					if (openNewWindow) {
						// Use SetNextWindowPos to specify the window position
						ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

						ImGui::Begin("Spectator List", &openNewWindow, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

						ImGui::Text("Coming Soon...");
						// Add content to your new window here
						ImGui::End();
					}
					
				}
				ImGui::EndChild();
				ImGui::SameLine();
				ImGui::BeginChild("REMEMBER", ImVec2(0, 0), true);
				{
					ImGui::Toggle("Normal FOV", &globals::norFOV);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Changes FOV back to normal");
					ImGui::Spacing();
					ImGui::Toggle("FOV Changer", &globals::FOV);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Changes FOV");

					ImGui::Spacing();
					ImGuiPP::CenterText("Thirdperson", 1, TRUE);
					ImGui::Spacing();
					ImGui::Toggle("Thirdperson mode", &globals::thirdperson);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Thirdperson duhh");
					ImGui::Spacing();
					ImGui::Toggle("Normal thirdperson FOV", &globals::norFOV);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Changes FOV back to normal");

					ImGui::Spacing();
					ImGuiPP::CenterText("Extra Features", 1, TRUE);
					ImGui::Spacing();
					ImGui::Button("Coming Soon...");
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Coming Soon...");

				}
				ImGui::EndChild();
				break;

			case 3:
				ImGuiPP::CenterText("Other", 1, TRUE);
				void renderUI();
				{
					if (show_exit_button) {
						if (ImGui::Button("PANIC BUTTON", { ImGui::GetContentRegionAvail().x, butn_tall })) {
							exit(0);
						}
					}
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Exits program");
				}
				ImGui::Spacing();
				if (ImGui::Button("My GitHub", { ImGui::GetContentRegionAvail().x, butn_tall }))
					ShellExecute(0, 0, "https://github.com/MavenCoding157", 0, 0, SW_SHOW);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("My GitHub");
				ImGui::Spacing();
				if (ImGui::Button("My Youtube", { ImGui::GetContentRegionAvail().x, butn_tall }))
					ShellExecute(0, 0, "https://www.youtube.com/channel/UCkP2YjZfvZIfArYbAUyRLsg", 0, 0, SW_SHOW);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("My Youtube");
				ImGui::Spacing();
				if (ImGui::Button("FREE GAMES", { ImGui::GetContentRegionAvail().x, butn_tall }))
					ShellExecute(0, 0, "https://drive.google.com/drive/folders/1myezzlndx8HAv9wtVErD2hoIAP_5Q4g-", 0, 0, SW_SHOW);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("FREE GAMES");
				ImGui::Spacing();
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				break;

			case 4:
				ImGuiPP::CenterText("Bugs and Future Features", 1, TRUE);
				ImGui::Spacing();
				ImGui::Text("[+] Configs tab will be coming soon (you will also be able to make and \n     save configs)");
				ImGui::Spacing();
				ImGui::Text("[+] We will improve the UI");
				ImGui::Spacing();
				ImGui::Text("[+] Fix RCS and add a aimbot fov slider and etc");
				ImGui::Spacing();
				ImGui::Text("[+] We will fix issues with the skin changer lagging the game as it is \n     permenantly on atm");
				//del
				break;

			case 5:
				ImGuiPP::CenterText("Settings", 1, TRUE);
				ImGui::Checkbox("Block reports [BETA]", &checkboxes::Blockreports);//coming soon
				break;

			case 6:
				ImGuiPP::CenterText("Made by: MavenCoding157", 1, TRUE);
				ImGui::Text("P.S the best external CSGO cheat menu");
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Text(":)");

				break;

			case 8:
				ImGuiPP::CenterText("Proccess status", 1, TRUE);

				if (status == 0) {
					ImGui::Text("csgo is running");
				}
				else {

					ImGui::Text("csgo is not running");
				}

				ImGui::PopStyleColor();

				break;

			case 7:
				ImGuiPP::CenterText("Creator: MavenCoding157", 1, TRUE);
				break;

			case 11:
				ImGuiPP::CenterText("Game Launcher", 1, TRUE);
				constexpr auto button_height = 50;
				ImGui::ListBoxHeader("", ImVec2(ImGuiPP::GetX(), ImGuiPP::GetY() - 36.5));
				{
					ImGui::Selectable("CSGO");
					ImGui::SetItemDefaultFocus();

				}ImGui::ListBoxFooter();

				if (ImGui::Button("Launch CSGO", { ImGui::GetContentRegionAvail().x, button_height }))
				{
					std::string csgoPath = FindCSGOPath();
					if (!csgoPath.empty()) {
						// Launch CS:GO
						ShellExecute(NULL, "open", csgoPath.c_str(), NULL, NULL, SW_SHOWDEFAULT);
					}
					else {
						// CS:GO not found
						// Handle the case where CS:GO is not installed
					}
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Launchs CSGO [BETA] (If nothing happens its probally an error in the code)");
				break;

			}

			ImGui::EndChild();
		}

		ImGui::End();
}

