#include <Windows.h>
#//include <varargs>

void DbgMessageBox(wchar_t* str)
{
	MessageBox(0, str, L"Error", MB_OK);
}

void PatchRegistryKeyI(wchar_t* path, wchar_t* key, DWORD val)
{
	HKEY hk;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, path, 0, KEY_READ | KEY_WRITE | KEY_SET_VALUE, &hk) != ERROR_SUCCESS)
	{
		DbgMessageBox(L"Failed to open registry path");

		return;
	}

	if(RegSetValueExW(hk, key, 0, REG_DWORD, (BYTE*)&val, sizeof(val)) != ERROR_SUCCESS)
	{
		DbgMessageBox(L"Failed to patch registry subkey");

		return;
	}

	RegCloseKey(hk);
}

BOOL __stdcall WinEnumCallback(HWND hwnd, LPARAM lp)
{
	wchar_t text[255];
	GetWindowTextW(hwnd, (LPWSTR)&text, 255);
	

	DbgMessageBox((LPWSTR)&text);

	GetClassName(hwnd, (LPWSTR)&text, 255);

	DbgMessageBox((LPWSTR)&text);

	return true;
}

void PatchRegistry()
{
	PatchRegistryKeyI(L"Explorer", L"ViewAll", 1);
	PatchRegistryKeyI(L"Explorer", L"ShowExt", 1);
	PatchRegistryKeyI(L"Explorer", L"ShowSys", 1);
	PatchRegistryKeyI(L"Explorer", L"ExpandControlPanel", 1);
	PatchRegistryKeyI(L"\\SOFTWARE\\Microsoft\\Shell\\OnTop", L"Default", 1);
}

void CopyDlls()
{
	const wchar_t* searchPath = L"\\Storage Card\\Dll\\";
	const wchar_t* searchPattern = L"\\Storage Card\\Dll\\*.*";

	WIN32_FIND_DATAW findData;
	HANDLE findFile = FindFirstFileW(searchPattern, &findData);

	while(findFile != INVALID_HANDLE_VALUE)
	{
		wchar_t srcPath[255];
		wchar_t dstPath[255];

		wsprintfW((LPWSTR)&srcPath, L"%s%s", searchPath, &findData.cFileName);
		wsprintfW((LPWSTR)&dstPath, L"\\Windows\\%s", findData.cFileName);
		CopyFileW((LPCWSTR)&srcPath, (LPCWSTR)&dstPath, true);

		findFile = FindNextFile(findFile, &findData) ? findFile : INVALID_HANDLE_VALUE;
	}
}

int WinMain(HINSTANCE inst, HINSTANCE prevInst, LPWSTR cmdLine, int nShowCmd)
{
	PatchRegistry();
	CopyDlls();

	// Launch explorer
	PROCESS_INFORMATION info;
	CreateProcess(L"\\Windows\\explorer.exe", 0, 0, 0, 0, 0, 0, 0, 0, &info);

	// Terminate launcher
	HWND hwnd = FindWindowW(L"NSGL_WINDOW", 0);
	if(hwnd)
	{
		DWORD procId;
		GetWindowThreadProcessId(hwnd, &procId);

		HANDLE proc = OpenProcess(PROCESS_TERMINATE, false, procId);
		TerminateProcess(proc, 0);
		CloseHandle(proc);
	}
}