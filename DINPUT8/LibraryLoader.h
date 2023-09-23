#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

std::wstring dllModFolderName = L"plugins";
std::vector<HMODULE> dlls;

inline bool ends_with(std::wstring const& value, std::wstring const& ending) {

	if (ending.size() > value.size()) { return false; }
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());

};

void load_dll_in_directory(const std::wstring& name, std::vector<HMODULE>& v) {

	HMODULE temp = nullptr;

	wchar_t FullPath[MAX_PATH];
	wchar_t FullPathFolder[MAX_PATH];
	wchar_t TempPath[MAX_PATH];

	GetModuleFileNameW(NULL, FullPathFolder, MAX_PATH);

	wcscpy_s(FullPathFolder, std::wstring(FullPathFolder).substr(0, std::wstring(FullPathFolder).find_last_of(L"\\/")).c_str());
	lstrcatW(FullPathFolder, L"\\");
	lstrcatW(FullPathFolder, name.c_str());
	lstrcatW(FullPathFolder, L"\\");
	wcscpy_s(FullPath, FullPathFolder);
	lstrcatW(FullPath, L"*");
	
	WIN32_FIND_DATA* pData = {};//new WIN32_FIND_DATA();
	WIN32_FIND_DATA data;
	HANDLE hFind;

	if ((hFind = FindFirstFile((LPCWSTR)FullPath, &data)) != INVALID_HANDLE_VALUE) {
	
		do {

			wcscpy_s(TempPath, FullPathFolder);
			lstrcatW(TempPath, data.cFileName);

			if (ends_with(TempPath,L".dll")) {

				try {

#ifdef DEBUG
					printf("%ls\n", TempPath);
					printf("%ls\n", FullPath);
					printf("%ls\n", FullPathFolder);
#endif
					temp = LoadLibraryW(TempPath);
					v.push_back(temp);
				
				} catch(int ex) {

					ex;
				
				};

			}

		} while (FindNextFileW(hFind, &data) != 0);
		FindClose(hFind);

	};
};