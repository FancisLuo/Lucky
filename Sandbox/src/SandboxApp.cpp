
//#include <Lucky.h>
//
//class Sandbox : public Lucky::Application
//{
//public:
//	Sandbox() {}
//
//	~Sandbox() {}
//};
//
//int main(int argc, char** argv)
//{
//	Lucky::LuckyLog::Init();
//
//	//Lucky::LuckyLog::GetCoreLogger()->warn("initialized!!");
//	LK_CORE_WARN("Initialized!!!");
//
//	Sandbox* sandbox = new Sandbox();
//	sandbox->Run();
//	delete sandbox;
//}



#include "SandboxCommon.h"
#include "D3D12Wrapper.h"

#define MAX_LOADSTRING 100

// 全局变量:
HWND				hMainWnd;                                   // 当前窗口
HINSTANCE			hInst;										// 当前实例
WCHAR				szTitle[MAX_LOADSTRING];                    // 标题栏文本
WCHAR				szWindowClass[MAX_LOADSTRING];              // 主窗口类名
WCHAR               szMenuName[MAX_LOADSTRING];
int                 gClientWidth                    = 800;
int                 gClientHeight                   = 600;

D3D12Wrapper        gWrapper;


// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
int                 Run();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    LPWSTR title        = lstrcpynW(szTitle,        L"App Title",   MAX_LOADSTRING);
    LPWSTR wndClassName = lstrcpynW(szWindowClass,  L"Sandbox App", MAX_LOADSTRING);
    LPWSTR menuName     = lstrcpynW(szMenuName,     L"Menu",        MAX_LOADSTRING);

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    if (!gWrapper.InitializeDirect3D(hMainWnd, gClientWidth, gClientHeight))
    {
        gWrapper.Destroy();
        return FALSE;
    }

    return Run();
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};

    wcex.cbSize         = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = szMenuName;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, IDC_ARROW);

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    hMainWnd = CreateWindow(
        szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        gClientWidth, gClientHeight,
        nullptr, nullptr,
        hInst, nullptr
    );

    if (!hMainWnd)
    {
        return FALSE;
    }

    ShowWindow(hMainWnd, nCmdShow);
    UpdateWindow(hMainWnd);

    return TRUE;
}

int Run()
{
    MSG msg = { 0 };

    // 主消息循环:
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
    {
        //int wmId = LOWORD(wParam);
        //// 分析菜单选择:
        //switch (wmId)
        //{
        //default:
        //    return DefWindowProc(hWnd, msg, wParam, lParam);
        //}
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 在此处添加使用 hdc 的任何绘图代码...
        EndPaint(hWnd, &ps);

        gWrapper.Update();
        gWrapper.Render();
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);

        gWrapper.Destroy();
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}