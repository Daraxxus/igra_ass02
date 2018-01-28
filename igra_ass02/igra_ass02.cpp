// igra_ass02.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "igra_ass02.h"
#include "Tank.h"
#include "Firing.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include <math.h>
#include <vector>

#define MAX_LOADSTRING 100
bool keys[256];

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
GLuint PixelFormat;
HDC hDC = NULL;
HWND hWnd = NULL;
HGLRC hRC = NULL;
int InitOpenGL();
void DrawGLScene();
void ReSizeGLScene(GLsizei width, GLsizei height);
int width = 720;
int height = 480;
int EXTRA_HEIGHT = 58;
int EXTRA_WIDTH = 20;
int rotateDeg = 1;
bool fired = false;
double cd = 1.00 ;
int maxProj = 10;

Tank tank;

__int64 startTimeInCounts = 0;
__int64 lastTimeInCounts = 0;
__int64 countsPerSecond;

double deltaTime;

void StartTimer();
double GetTimePassedSinceLastTime();
double GetTimePassedSinceStart();
void SetupLight();

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

static PIXELFORMATDESCRIPTOR pfd{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,// Version Number
	PFD_DRAW_TO_WINDOW | // Format Must Support Window
	PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
	PFD_DOUBLEBUFFER,// Must Support Double Buffering
	PFD_TYPE_RGBA, // Request An RGBA Format
	16, // Select Our Color Depth = 16
	0, 0, 0, 0, 0, 0, // Color Bits Ignored
	0,// No Alpha Buffer
	0,// Shift Bit Ignored
	0,// No Accumulation Buffer
	0, 0, 0, 0, // Accumulation Bits Ignored
	32, // 32 Bit Z-Buffer (Depth Buffer)
	0,// No Stencil Buffer
	0,// No Auxiliary Buffer
	PFD_MAIN_PLANE,// Main Drawing Layer
	0,// Reserved
	0, 0, 0// Layer Masks Ignored
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_IGRA_ASS02, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IGRA_ASS02));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
			DrawGLScene();

			SwapBuffers(hDC);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IGRA_ASS02));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_IGRA_ASS02);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   100, 100, width + EXTRA_WIDTH, height + EXTRA_HEIGHT,
	   nullptr, nullptr, hInstance, nullptr);
   ::SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) &~WS_SIZEBOX &~WS_MINIMIZEBOX &~WS_MAXIMIZEBOX); //Diable minimize, maximise and resize functions
   InitOpenGL();

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   ReSizeGLScene(width, height);
   StartTimer();
   Tank::Player = tank;
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
        }
	case WM_KEYDOWN:
	{
		if (GetAsyncKeyState(VK_SPACE))
		{
			if (!fired && (deltaTime < cd)) {
				Firing::HandleKeyDown(tank.ReturnCurrentPosition());
				fired = true;
				deltaTime = 0;
			}
		}
		keys[wParam] = TRUE;

		break;
	}
	case WM_KEYUP:
		if (fired && (deltaTime < cd)) {
			fired = false;
		}
		keys[wParam] = FALSE;

		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_TIMER:
		rotateDeg++;
		return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


void ReSizeGLScene(GLsizei width, GLsizei height) {
	if (height == 0)// Prevent A Divide By Zero By
		height = 1; // Making Height Equal One
					// Reset The Current Viewport
	glViewport(0, 0, width, height);
	// Select The Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset The Projection Matrix
	glLoadIdentity();
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height,	0.1f, 100.0f);
	//gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int InitOpenGL() {
	// Get Device Dontext
	if (!(hDC = GetDC(hWnd))) {
		MessageBox(NULL, L"Can't Create A GL Device Context.",
			L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	// Check if Windows can find a matching Pixel Format
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {
		MessageBox(NULL, L"Can't Find A Suitable PixelFormat.",
			L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Try to set pixel format
	if (!SetPixelFormat(hDC, PixelFormat, &pfd)) {
		MessageBox(NULL, L"Can't Set The PixelFormat.", L"ERROR",
			MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	// Get a Rendering context
	if (!(hRC = wglCreateContext(hDC))) {
		MessageBox(NULL, L"Can't Create A GL Rendering Context.",
			L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	// Activate Rendering Context
	if (!wglMakeCurrent(hDC, hRC)) {
		MessageBox(NULL, L"Can't Activate The GL RenderingContext.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	SetupLight();
	StartTimer();
	return 1;
}

void Draw3D_AxisSystem() {
	glBegin(GL_LINES);
	// Draw positive x-axis as red
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);
	// Draw positive y-axis as green
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);
	// Draw positive z-axis as blue
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);
	glEnd();
}

void StartTimer() {
	__int64 currentTimeInCounts;
	// countsPerSecond depends on your PC
	if (!QueryPerformanceFrequency(
		(LARGE_INTEGER *)&countsPerSecond)) {
		MessageBox(NULL, L"QueryPerformanceFrequency Failed.",L"ERROR",MB_OK | MB_ICONINFORMATION);
			return;
	}
	QueryPerformanceCounter(
		(LARGE_INTEGER *)&currentTimeInCounts);
	startTimeInCounts = currentTimeInCounts;
	lastTimeInCounts = currentTimeInCounts;
}

double GetTimePassedSinceStart() {
	__int64 currentTimeInCounts;
	double timePassedSeconds;
	// Calculate time passed in seconds since timer was started
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTimeInCounts);
	timePassedSeconds = (currentTimeInCounts - startTimeInCounts) /
		(double)countsPerSecond;

	return timePassedSeconds;
}

double GetTimePassedSinceLastTime() {
	__int64 currentTimeInCounts, timePassedSinceLastTimeInCounts;
	// Calculate time passed in seconds since last call to
	// GetTimePassedSinceLastTime
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTimeInCounts);
	timePassedSinceLastTimeInCounts =
		currentTimeInCounts - lastTimeInCounts;
	double timePassedSinceLastTimeInSeconds =
		(currentTimeInCounts - lastTimeInCounts) /
		(double)countsPerSecond;
	lastTimeInCounts = currentTimeInCounts;
	return timePassedSinceLastTimeInSeconds;
}

void SetupLight() {
	glShadeModel(GL_SMOOTH);
	GLfloat LightAmbient[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat LightDiffuse[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat LightSpecular[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat LightPosition[] = { 10, 10, 10, 0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT0);
}

void DrawTerrain()
{
	glBegin(GL_QUADS);
	glColor3f(0.5, 1, 1);
	glVertex3f(-50, 0, 50);
	glVertex3f(-50, 0, -50);
	glVertex3f(50, 0, -50);
	glVertex3f(50, 0, 50);
	glEnd();
}

void DrawGLScene() {
	tank.Update(deltaTime);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	gluLookAt(20, 10, 20,
		0, 0, 0,
		0, 1, 0);
	
	Draw3D_AxisSystem();
	DrawTerrain();
	
	deltaTime = GetTimePassedSinceLastTime();

	tank.DrawTank();
	for (int i = 0; i < Firing::shell.size(); i++)
	{
		Firing::shell[i]->Update(deltaTime);
		Firing::shell[i]->DrawProjectile();
	}
}

