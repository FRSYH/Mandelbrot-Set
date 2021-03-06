#include <windows.h>
#include <stdlib.h> 
#include <string.h> 
#include <tchar.h>  
#include <math.h>
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

void setUpColors(COLORREF colors[]) {
	colors[0] = RGB(66, 30, 15);
	colors[1] = RGB(25, 7, 26);
	colors[2] = RGB(9, 1, 47);
	colors[3] = RGB(4, 4, 73);
	colors[4] = RGB(0, 7, 100);
	colors[5] = RGB(12, 44, 138);
	colors[6] = RGB(24, 82, 177);
	colors[7] = RGB(57, 125, 209);
	colors[8] = RGB(134, 181, 229);
	colors[9] = RGB(211, 236, 248);
	colors[10] = RGB(241, 233, 191);
	colors[11] = RGB(248, 201, 95);
	colors[12] = RGB(255, 170, 0);
	colors[13] = RGB(204, 120, 0);
	colors[14] = RGB(253, 87, 0);
	colors[15] = RGB(106, 52, 3);
}


int TestMandelbrotConvergence(double c_re, double c_im) {
	
	int iteration = 0;
	double x = 0.0, y = 0.0, x_new = 0.0;

	while (((x*x) + (y*y) <= 16) && (iteration < MAX_ITERATION)) {
		x_new = (x * x) - (y * y) + c_re;
		y = (2 * x*y) + c_im;
		x = x_new;
		iteration++;
	}
	return iteration;
}

void DrawPixel(HDC hdc, int col, int row, COLORREF colors[], int iteration) {
	// set color of pixel according to its divergence speed
	if (iteration < MAX_ITERATION) {
		SetPixel(hdc, col, row, colors[iteration % 16]);
	}else {
		SetPixel(hdc, col, row, RGB(0,0,0));
	}
}

double PixelCoordinatesToComplexPlaneCoordinates(int axis, double axis_center, double zoom, int width) {
	return 	(axis - axis_center) * zoom / width;
}

void computeAndDrawMandelbrotSet(HDC hdc, double zoom, double shift_x, double shift_y) {

	double c_re = 0.0;
	double c_im = 0.0;
	HWND window = WindowFromDC(hdc);
	RECT rect;
	int width, height = 0;
	if (GetWindowRect(window, &rect))
	{
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}
	double x_center = width / 2.0;
	double y_center = height / 2.0;
	int iteration = 0;
	COLORREF colors[16];
	setUpColors(colors);


	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			// real and immaginary component of complex number c in complex plane
			c_re = PixelCoordinatesToComplexPlaneCoordinates(col, x_center, zoom, width);
			c_im = PixelCoordinatesToComplexPlaneCoordinates(row, y_center, zoom, width);
			// axis traslation
			c_re += shift_x;
			c_im += shift_y;
			iteration = TestMandelbrotConvergence(c_re, c_im);
			DrawPixel(hdc, col, row, colors, iteration);
			iteration = 0;
		}
	}
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	double shift_x = -0.5;
	double shift_y = 0.0;
	double zoom = 3.5;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, World!");
	switch (message) {
	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);
		computeAndDrawMandelbrotSet(hdc, zoom, shift_x, shift_y);
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
		1300,
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
