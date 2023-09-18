#include "gui.h"
#include "globals.h"
#include "hacks.h"
#include <thread>
#include <iostream>
#include "../DiscordRpc/Class/Discord.h"//del

//discord thing idk
Discord* g_Discord;

//popup
void StartMessageBox() {
	MessageBox(NULL, "Cheat Successfully Enabled!", "Quantum CSGO Menu", MB_ICONINFORMATION | MB_OK);
}

int __stdcall wWinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow)

{
	
	// Display the message box when the application is run
	StartMessageBox();

	g_Discord->Initialize();
	g_Discord->Update();

	Memory mem{ "csgo.exe" };

	globals::clientAddress = mem.GetModuleAddress("client.dll");
	globals::engineAddress = mem.GetModuleAddress("engine.dll");//del
	
	
	std::thread(hacks::VisualsThread, mem).detach();
	
	// create gui
	gui::CreateHWindow("Quantum Menu");
	gui::CreateDevice();
	gui::CreateImGui();

	while (gui::isRunning)
	{
		gui::BeginRender();
		gui::Render();
		gui::EndRender();

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	// destroy gui
	gui::DestroyImGui();
	gui::DestroyDevice();
	gui::DestroyHWindow();

	return EXIT_SUCCESS;
}