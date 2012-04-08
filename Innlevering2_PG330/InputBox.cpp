#include "InputBox.h"

InputBox::InputBox(void)
{
}

InputBox::~InputBox(void)
{
}
#include <windows.h>
	 
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	 
	static char sClassName[]  = "MyClass";
	static HINSTANCE zhInstance = NULL;
	 
	/*int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	        WNDCLASSEX WndClass;
	        HWND hwnd;
	        MSG Msg;
	 
	        zhInstance = hInstance;
	 
	        WndClass.cbSize        = sizeof(WNDCLASSEX);
	        WndClass.style         = NULL;
	        WndClass.lpfnWndProc   = WndProc;
	        WndClass.cbClsExtra    = 0;
	        WndClass.cbWndExtra    = 0;
	        WndClass.hInstance     = zhInstance;
	        WndClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	        WndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	        WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	        WndClass.lpszMenuName  = NULL;
	        WndClass.lpszClassName = sClassName;
	        WndClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	 
	        if(!RegisterClassEx(&WndClass)) {
	                MessageBox(0, "Error Registering Class!", "Error!", MB_OK);
	                return 0;
	        }
	 
	        hwnd = CreateWindowEx(WS_EX_STATICEDGE, sClassName, "db Tutorial", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
	 
	CW_USEDEFAULT,
	                     320, 240, NULL, NULL, zhInstance, NULL);
	 
	        if(hwnd == NULL) {
	                MessageBox(0, "Error Creating Window!", "Error!",MB_OK);
	                return 0;
	        }
	 
	        ShowWindow(hwnd, nCmdShow);
	        UpdateWindow(hwnd);
	 
	        while(GetMessage(&Msg, NULL, 0, 0)) {
	                TranslateMessage(&Msg);
	                DispatchMessage(&Msg);
	        }
	 
	        return Msg.wParam;
	}*/
	 
	LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	        switch(Message) {
	                case WM_CLOSE:
	                        DestroyWindow(hwnd);
	                        break;
	                case WM_DESTROY:
	                        PostQuitMessage(0);
	                        break;
	                default:
	                        return DefWindowProc(hwnd, Message, wParam, lParam);
	        }
	        return 0;
	}
