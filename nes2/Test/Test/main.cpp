#include <stdlib.h>
#include <stdio.h>

#include <nes_datatype.h>
#include <aygshell.h>
#include <ddraw.h>

#include <Windows.h>

struct KeyMapping
{
	int VirtualKeyCode;
	int KeyCode;
};

typedef HRESULT (WINAPI ddCreate)( GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter );

extern "C"
{
extern uint16 *LCDBUF;
extern Color* frameBuffer;
extern int8  KEY[8];
HWND hwnd = 0;
HDC dc = 0;

extern int mrc_init();
extern void NEStimer(int data);
}

#define MAKERGB(r, g, b) (uint16) (((b >> 3)) | ((g >> 3) << 5)) | ((r >> 3) << 10) 

void Guard(HRESULT hr, wchar_t* errorStr)
{
	if(hr != DD_OK)
	{
		MessageBoxW(0, errorStr, L"Error", MB_OK);

		exit(0);
	}
}

void ProcessInput()
{

	KeyMapping mapping[] = {
		{ VK_RETURN, 4 },
		{ VK_LEFT, 1 },
		{ VK_RIGHT, 0},
		{ VK_UP, 3},
		{ VK_DOWN, 2},
		{ 1, 7 }
	};

	if(GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		exit(0);
	
	for(int i = 0; i < sizeof(mapping) / sizeof(KeyMapping); i++)
	{
		KEY[mapping[i].KeyCode] = GetAsyncKeyState(mapping[i].VirtualKeyCode) & 0x8000 ? 1 : 0;
		
	}
}

int WinMain(HINSTANCE inst, HINSTANCE prevInst, LPWSTR cmdLine, int showCmd)
{
	BITMAPINFO info;
	memset(&info, 0, sizeof(info));
	info.bmiHeader.biBitCount = 16;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biHeight = -240;
	info.bmiHeader.biWidth = 240;
	info.bmiHeader.biCompression = BI_RGB;
	info.bmiHeader.biSize = sizeof(info);
	int res;

	if(!mrc_init())
		MessageBoxW(0, L"Obser", L"Obosralsa", MB_OK);

	


	hwnd = CreateWindowW(L"static", L"Emulator", WS_VISIBLE | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, 0, 0);
	dc = GetDC(hwnd);

	SHFullScreen(hwnd, SHFS_HIDETASKBAR | SHFS_HIDESTARTICON | SHFS_HIDESIPBUTTON);

	HINSTANCE i = LoadLibrary(L"ddraw.dll");
	ddCreate* DDCreate;
	DDCreate = (ddCreate*)GetProcAddress(i, L"DirectDrawCreate");

	if(DDCreate == 0)
		MessageBoxW(0, L"DD failed", L"Error", MB_OK);
	
	IDirectDraw* dd;
	Guard(DDCreate(0, &dd, 0), L"DDraw create failed");

	/*GXOpenDisplay(hwnd, GX_FULLSCREEN);
	
	unsigned char* ptr = (unsigned char*)GXBeginDraw();*/

	unsigned char* ptr;
	HBITMAP bmp = CreateDIBSection(dc, &info, DIB_RGB_COLORS, (void**)&ptr, 0, 0);
	
	HDC dcBmp = CreateCompatibleDC(dc);
	SelectObject(dcBmp, bmp);
	
 
	while(IsWindow(hwnd))
	{
		ProcessInput();

			NEStimer(2);

		for(int i = 0; i < 240; i++)
		{
//		memcpy((unsigned char*)fbInfo.pFramePointer + (i * 240 * 2), (unsigned char*)(LCDBUF) + (i * 240 * 2), 240 * 2);
		}

		//BitBlt(dc, 0, 0, info.bmiHeader.biWidth, -info.bmiHeader.biHeight, dcBmp, 0, 0, SRCCOPY);
		
		SetDIBitsToDevice(dc, 0, 0, info.bmiHeader.biWidth, -info.bmiHeader.biHeight, 0, 0, 0, -info.bmiHeader.biHeight, LCDBUF, &info, DIB_RGB_COLORS);
		
		
		//Sleep(1000 / 60);
	}
	
	
	return 0;
}