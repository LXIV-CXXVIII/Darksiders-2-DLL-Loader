#include "stdafx.h"

#include "dinput8\dinputWrapper.h"
#include <Windows.h>
#include "LibraryLoader.h"

// Export DINPUT8
tDirectInput8Create oDirectInput8Create;

typedef int (WINAPI *ZWSETINFORMATIONTHREAD)(HANDLE, THREAD_INFORMATION_CLASS, PVOID, ULONG);

// Pointer for calling original MessageBoxW.
ZWSETINFORMATIONTHREAD fpZwSetInformationThread = NULL;

DWORD WINAPI MainThread() {
#ifdef DEBUG
	FILE* fp;
	AllocConsole();
	SetConsoleTitleA("DS2LM - Debug Console");
	freopen_s(&fp, "CONOUT$", "w", stdout);
#endif
	load_dll_in_directory(dllModFolderName, dlls);

	return 0;
}

// Detour function
int WINAPI tZwSetInformationThread(HANDLE threadHandle, THREAD_INFORMATION_CLASS threadInfoClass, PVOID threadInfo, ULONG threadInfoLength)
{
	if (threadInfoClass == 0x11) // ThreadHideFromDebugger
	{
		return 0x1; // return STATUS_SUCCESS as if we set the ThreadHideFromDebugger flag
	}

	return fpZwSetInformationThread(threadHandle, threadInfoClass, threadInfo, threadInfoLength); // return the original function if any other info class
}

BOOL InitInstance(HMODULE hModule)
{
	/*
	if (MH_Initialize() != MH_OK)
	{
		ExitProcess(0xDEADBEEF);

	}

	// or you can use the new helper function like this.
	if (MH_CreateHookApi(L"ntdll", "NtSetInformationThread", &tZwSetInformationThread, reinterpret_cast<LPVOID*>(&fpZwSetInformationThread)) != MH_OK)
	{
		ExitProcess(MH_CreateHookApi(L"ntdll", "NtSetInformationThread", &tZwSetInformationThread, reinterpret_cast<LPVOID*>(&fpZwSetInformationThread)));
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
	{
		ExitProcess(0xBEEEEEEF);
	}
	*/

	// Load the real dinput8.dll
	HMODULE hMod;
	wchar_t dllPath[MAX_PATH];
	GetSystemDirectory(dllPath, MAX_PATH);
	lstrcatW(dllPath, L"\\dinput8.dll");
	hMod = LoadLibrary(dllPath);
	oDirectInput8Create = (tDirectInput8Create)GetProcAddress(hMod, "DirectInput8Create");

	// Call the main thread
//    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&Begin, hModule, 0, nullptr);

	MainThread();

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
		InitInstance(hModule);
		break;
	case DLL_PROCESS_DETACH:
		ExitInstance();
		break;
	}
	return TRUE;
}