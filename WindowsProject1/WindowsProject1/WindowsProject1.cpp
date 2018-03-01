// WindowsProject1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WindowsProject1.h"

using namespace DirectX;

#define MAX_LOADSTRING 100
#define BACKBUFFER_HEIGHT 800
#define BACKBUFFER_WIDTH 800

#define safeRelease(finished) if(finished->Release()) { finished = nullptr; }

////////////////MY STRUCTS///////////////////
struct MY_VERTEX
{
	XMFLOAT3 pos;
	XMFLOAT4 rgba;
};

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

/////////////////MY GLOBALS/////////////////////////////
ID3D11Device *device;
IDXGISwapChain *swapChain;
ID3D11DeviceContext *context;
D3D11_VIEWPORT viewport;
DXGI_SWAP_CHAIN_DESC scd;
ID3D11Resource *buffer;
ID3D11RenderTargetView *view;

float colors[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

////////////////////////////////////////////////////////

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))					// use peek message if does'nt work
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
   scd.BufferCount = 1;
   scd.Windowed = true;
   scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
   scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
   scd.SampleDesc.Count = 1;
   scd.SampleDesc.Quality = 0;
   scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
   scd.BufferDesc.Height = BACKBUFFER_HEIGHT;
   scd.BufferDesc.Width = BACKBUFFER_WIDTH;
   scd.BufferDesc.RefreshRate.Numerator = 60;
   scd.BufferDesc.RefreshRate.Denominator = 1;
   scd.OutputWindow = hWnd;

   UINT flag = 0;
#if _DEBUG
   flag = D3D11_CREATE_DEVICE_DEBUG;
#endif

   D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flag, NULL, NULL,
	   D3D11_SDK_VERSION, &scd, &swapChain, &device, NULL, &context);

   swapChain->GetBuffer(0, __uuidof(buffer), reinterpret_cast<void**>(&buffer));
   device->CreateRenderTargetView(buffer, NULL, &view);

   viewport.Height = BACKBUFFER_HEIGHT;
   viewport.Width = BACKBUFFER_WIDTH;
   viewport.MinDepth = 0.0f;
   viewport.MaxDepth = 1.0f;
   viewport.TopLeftX = 0;
   viewport.TopLeftY = 0;
   context->RSSetViewports(1, &viewport);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

			//Set window color to something and clear it for redrawing in next frame/////////////////////////////////////////
			context->OMSetRenderTargets(1, &view, 0);

			UINT var = 1;
			context->RSGetViewports(&var, &viewport);

			context->ClearRenderTargetView(view, colors);    //colors array decide the color of our backbuffer
			///////////////////////////////////////////////////////////////////////


			//call this at end of case WM_PAINT
			swapChain->Present(0, 0);
			//////////////////////////////////

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:

		safeRelease(context);
		safeRelease(swapChain);
		safeRelease(device);
		safeRelease(buffer);
		safeRelease(view);

        PostQuitMessage(0);
        break;
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
