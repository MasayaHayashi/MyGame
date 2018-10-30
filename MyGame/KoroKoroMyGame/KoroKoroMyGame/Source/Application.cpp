//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Application.cpp
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Application.h"
#include <Windows.h>
#include <time.h>

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Application::Application()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Application::~Application()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Application::initialize(HINSTANCE instance, int nCmdShow)
{

#if _DEBUG
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	HWND windowHandle = createWindow(instance);

	if (windowHandle < 0)
	{
		return;
	}

	// フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	execLastTime = fpsLastTime = timeGetTime();
	currentTime = frameCount = 0;

	//ランダムの初期化
	srand((unsigned)time(NULL));

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(windowHandle, nCmdShow);
	UpdateWindow(windowHandle);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Application::update()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const void Application::draw()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Application::finalize()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プロシージャ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wparam)
		{
		case VK_ESCAPE:					// [ESC]キーが押された
			DestroyWindow(wnd);			// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(wnd, msg, wparam, lparam);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// アプリ継続中か
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const bool Application::isAppContinuation()
{
	return appContinuation;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HWND Application::createWindow(HINSTANCE instance)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		instance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		ClassName,
		NULL
	};
	HWND wndHandle;
	MSG msg;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	wndHandle = CreateWindowEx(0,
		ClassName,
		WindowName,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME, /*& ~WS_MAXIMIZEBOX,*/
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		ScreenWidth + GetSystemMetrics(SM_CXDLGFRAME)  * 2,
		ScreenHeight + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		NULL,
		NULL,
		instance,
		NULL);

	return wndHandle;
}