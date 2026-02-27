#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#define ID_EDIT1 101
#define ID_EDIT2 102
#define ID_ADD   201
#define ID_SUB   202
#define ID_MUL   203
#define ID_DIV   204

HWND hEdit1, hEdit2;

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {

		case WM_CREATE: {

			CreateWindow("STATIC", "Please input two numbers",
				WS_VISIBLE | WS_CHILD,
				40, 15, 200, 20,
				hwnd, NULL, NULL, NULL);

			hEdit1 = CreateWindow("EDIT", "",
				WS_VISIBLE | WS_CHILD | WS_BORDER,
				65, 45, 120, 20,
				hwnd, (HMENU)ID_EDIT1, NULL, NULL);

			hEdit2 = CreateWindow("EDIT", "",
				WS_VISIBLE | WS_CHILD | WS_BORDER,
				65, 75, 120, 20,
				hwnd, (HMENU)ID_EDIT2, NULL, NULL);

			CreateWindow("BUTTON", "+",
				WS_VISIBLE | WS_CHILD,
				40, 115, 40, 30,
				hwnd, (HMENU)ID_ADD, NULL, NULL);

			CreateWindow("BUTTON", "-",
				WS_VISIBLE | WS_CHILD,
				85, 115, 40, 30,
				hwnd, (HMENU)ID_SUB, NULL, NULL);

			CreateWindow("BUTTON", "*",
				WS_VISIBLE | WS_CHILD,
				130, 115, 40, 30,
				hwnd, (HMENU)ID_MUL, NULL, NULL);

			CreateWindow("BUTTON", "/",
				WS_VISIBLE | WS_CHILD,
				175, 115, 40, 30,
				hwnd, (HMENU)ID_DIV, NULL, NULL);

			break;
		}

		case WM_COMMAND: {

			char t1[100], t2[100];
			GetWindowText(hEdit1, t1, 100);
			GetWindowText(hEdit2, t2, 100);

			double a = atof(t1);
			double b = atof(t2);
			double result = 0;

			switch(LOWORD(wParam)) {
				case ID_ADD: result = a + b; break;
				case ID_SUB: result = a - b; break;
				case ID_MUL: result = a * b; break;
				case ID_DIV:
					if(b == 0) {
						MessageBox(hwnd, "Cannot divide by zero", "Error", MB_OK);
						return 0;
					}
					result = a / b;
					break;
				default: return 0;
			}

			char output[100];

			sprintf(output, "%.6f", result);
			MessageBox(hwnd, output, "Result", MB_OK);
			break;
		}
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize	 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor	 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = CreateSolidBrush(RGB(2, 124, 255));
	wc.lpszClassName = "WindowClass";
	wc.hIcon	 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm	 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		0,
		"WindowClass",
		"My Calculator",
		WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		250,
		200,
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}