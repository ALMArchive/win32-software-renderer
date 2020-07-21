#include <windows.h>
#include "defines.h"
#include "bitmap.h"
#include "stars_3d.h"
#include "weird_gradient.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define RESIZE false

global_variable bool Running;

global_variable BITMAPINFO BitmapInfo;
global_variable bitmap Bitmap;
global_variable star_field StarField;
global_variable i32 BitmapWidth;
global_variable i32 BitmapHeight;

internal void
Win32ResizeDIBSection(i32 Width, i32 Height) {
	BitmapDestroy(&Bitmap);

	BitmapWidth = Width;
	BitmapHeight = Height;

	BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
	BitmapInfo.bmiHeader.biWidth = BitmapWidth;
	BitmapInfo.bmiHeader.biHeight = BitmapHeight;
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;

	Bitmap = BitmapCreate(BitmapWidth, BitmapHeight);
}

internal void
Win32UpdateWindow(HDC DeviceContext, RECT *ClientRect, int X, int Y, int Width, int Height) {
	i32 WindowWidth = ClientRect->right - ClientRect->left;
	i32 WindowHeight = ClientRect->bottom - ClientRect->top;

	StretchDIBits(DeviceContext,
		0, 0, WindowWidth, WindowHeight,
		0, 0, BitmapWidth, BitmapHeight,
		Bitmap.Memory,
		&BitmapInfo,
		DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK
MainWindowCallback(HWND Window,
	UINT Message,
	WPARAM WParam,
	LPARAM LParam)
{
	LRESULT Result = 0;

	switch (Message)
	{
		case WM_SIZE: {
			RECT ClientRect;
			GetClientRect(Window, &ClientRect);
			i32 Width = ClientRect.right - ClientRect.left;
			i32 Height = ClientRect.bottom - ClientRect.top;
			Win32ResizeDIBSection(Width, Height);
		} break;

		case WM_CLOSE: {
			OutputDebugStringA("WM_CLOSE\n");
			Running = false;
		} break;

		case WM_ACTIVATEAPP: {
			OutputDebugStringA("WM_ACTIVATEAPP\n");
		} break;
		
		case WM_DESTROY: {
			Running = false;
		} break;

		case WM_PAINT: {
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			i32 X = Paint.rcPaint.left;
			i32 Y = Paint.rcPaint.top;
			i32 Width = Paint.rcPaint.right - X;
			i32 Height = Paint.rcPaint.bottom - Y;

			RECT ClientRect;
			GetClientRect(Window, &ClientRect);

			Win32UpdateWindow(DeviceContext, &ClientRect, X, Y, Width, Height);
			EndPaint(Window, &Paint);
		} break;

		default: {
			Result = DefWindowProc(Window, Message, WParam, LParam);
		} break;
	}

	return Result;
}

int CALLBACK
WinMain(HINSTANCE Instance,
	HINSTANCE PrevInstance,
	LPSTR CommandLine,
	int ShowCode)
{
	WNDCLASS WindowClass = {};

	WindowClass.lpfnWndProc = MainWindowCallback;
	WindowClass.style = CS_VREDRAW | CS_HREDRAW;
	WindowClass.hInstance = Instance;
	WindowClass.lpszClassName = "SoftwareRenderingWindowClass";

	if (RegisterClassA(&WindowClass)) {
		u64 WindowStyles = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		if (!RESIZE) {
			WindowStyles ^= WS_THICKFRAME | WS_MAXIMIZEBOX;
		}
		HWND Window = CreateWindowExA(
			0,
			WindowClass.lpszClassName,
			"Software Rendering",
			WindowStyles,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			0,
			0,
			Instance,
			0);

		if (Window) {
			i32 XOffset = 0;
			i32 YOffset = 0;

			StarField = StarFieldCreate(4096, 64.0f, 64.0f);
			Running = true;
			while(Running) {
				MSG Message;
				while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE)) {
					if (Message.message == WM_QUIT) {
						Running = false;
					}
					TranslateMessage(&Message);
					DispatchMessageA(&Message);

				}

				StarFieldUpdateAndRender(&Bitmap, &StarField, 0.001f);
				//RenderWeirdGradient(&Bitmap, XOffset, YOffset);

				HDC DeviceContext = GetDC(Window);
				RECT ClientRect;
				GetClientRect(Window, &ClientRect);

				i32 WindowWidth = ClientRect.right - ClientRect.left;
				i32 WindowHeight = ClientRect.bottom - ClientRect.top;

				Win32UpdateWindow(DeviceContext, &ClientRect, 0, 0, WindowWidth, WindowHeight);
				ReleaseDC(Window, DeviceContext);

				++XOffset;
				YOffset += 2;

			}
		}
	}
	else
	{

	}

	return 0;
}
