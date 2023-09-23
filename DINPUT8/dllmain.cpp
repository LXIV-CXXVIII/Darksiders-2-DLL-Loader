#include "stdafx.h"

#include <iostream>
#include "dinput8\dinputWrapper.h"
#include <Windows.h>
#include "LibraryLoader.h"

// Export DINPUT8
tDirectInput8Create oDirectInput8Create;

DWORD WINAPI MainThread(char* argv) {
#ifdef DEBUG
	FILE* fp;
	AllocConsole();
	SetConsoleTitleA("DLL Loader for Darksiders 2");
	freopen_s(&fp, "CONOUT$", "w", stdout);
#endif

	auto phModule = GetModuleHandle(L"Darksiders2.exe");
	if (phModule) {
		load_dll_in_directory(dllModFolderName, dlls);
	};

	return 0;
}

BOOL InitInstance(HMODULE hModule, char* argv)
{
	// Load the real dinput8.dll
	HMODULE hMod;
	wchar_t dllPath[MAX_PATH];
	GetSystemDirectory(dllPath, MAX_PATH);
	lstrcatW(dllPath, L"\\dinput8.dll");
	hMod = LoadLibrary(dllPath);
	oDirectInput8Create = (tDirectInput8Create)GetProcAddress(hMod, "DirectInput8Create");

	// Call the main thread
    // CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&Begin, hModule, 0, nullptr);

	MainThread(argv);

	return true;
}

BOOL ExitInstance()
{
	//    End();

	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		InitInstance(hModule, __argv[0]);
		break;
	case DLL_PROCESS_DETACH:
		ExitInstance();
		break;
	}
	return TRUE;
}