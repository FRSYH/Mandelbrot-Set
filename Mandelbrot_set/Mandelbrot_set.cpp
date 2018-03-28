//============================================================================
// Name        : draw.cpp
// Author      : Martino
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <windows.h>
#include <stdlib.h> 
#include <string.h> 
#include <tchar.h>  
using namespace std;

#define MAX_ITERATION 100

COLORREF randomRGBColor() {
	return RGB(rand() % 256, rand() % 256, rand() % 256);
}

void randomColor(HDC hdc, int xres, int yres) {
	for (int j = 0; j<yres; j++)
		for (int i = 0; i<xres; i++)
			SetPixel(hdc, i, j, randomRGBColor());
}

void computeAndDrawMandelbrotSet(HDC hdc, int width, int height) {

	double c_re = 0.0;
	double c_im = 0.0;
	double traslazione_re = -0.5;
	double traslazione_im = 0.0;
	double x_center = width / 2.0;
	double y_center = height / 2.0;
	double x = 0.0, y = 0.0;
	int i = 0;
	double x_new = 0;
	COLORREF black = RGB(0, 0, 0);
	COLORREF white = RGB(255, 255, 255);

	COLORREF colors[MAX_ITERATION];

	for (int i = 0; i < MAX_ITERATION; i++) {
		colors[i] = RGB(0, i * 5 % 255, i + 40 % 255);
		//colors[i] = RGB(i+1, i+5%255, i + 10 % 255);
	}

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			// real and immaginary component of complex number c
			c_re = (col - x_center) * 2.5 / width;
			c_im = (row - y_center) * 2.5 / width;
			// axis traslation
			c_re += traslazione_re;
			c_im += traslazione_im;

			x = 0, y = 0;
			i = 0;
			while (((x*x) + (y*y) <= 16) && (i < MAX_ITERATION)) {
				x_new = (x * x) - (y * y) + c_re;
				y = (2 * x*y) + c_im;
				x = x_new;
				i++;
			}
			// set color of pixel according to its divergence speed
			if (i < MAX_ITERATION) {
				SetPixel(hdc, col, row, colors[i]);
			}
			else {
				SetPixel(hdc, col, row, black);
			}
		}
	}
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	int yres = 950;
	int xres = 950;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, World!");
	switch (message) {
	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);
		computeAndDrawMandelbrotSet(hdc, xres, yres);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	static TCHAR szWindowClass[] = _T("win32app");
	static TCHAR szTitle[] = _T("Mandelbrot Set");


	//crea la finestra//
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	//registrare la finestra//
	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Win32 Guided Tour"), NULL);
		return 1;
	}

	// The parameters to CreateWindow explained: 
	// szWindowClass: the name of the application 
	// szTitle: the text that appears in the title bar 
	// WS_OVERLAPPEDWINDOW: the type of window to create 
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y) 
	// 500, 100: initial size (width, length) 
	// NULL: the parent of this window 
	// NULL: this application does not have a menu bar 
	// hInstance: the first parameter from WinMain 
	// NULL: not used in this application 

	HWND hWnd = CreateWindow(szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		950,
		950,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (!hWnd) {
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Win32 Guided Tour"), NULL);
		return 1;
	}

	// The parameters to ShowWindow explained : 
	// hWnd: the value returned from CreateWindow 
	// nCmdShow: the fourth parameter from WinMain 
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
