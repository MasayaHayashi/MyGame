//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Application.cpp
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Application.h"
#include "DirectX3D.h"
#include <Windows.h>
#include <time.h>
#include <memory>

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Application::Application()
{
	directX3dObj.reset(NEW DirectX3D());
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
void Application::initialize(HINSTANCE& instance, INT& cmdShow)
{
	
#if _DEBUG
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	HWND windowHandle = createWindow(instance);

	// フレームカウント初期化
	timeBeginPeriod(1);
	execLastTime = fpsLastTime = timeGetTime();
	currentTime  = frameCount = 0;

	ShowWindow(windowHandle, cmdShow);
	UpdateWindow(windowHandle);

	directX3dObj->init(windowHandle);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メインループ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const void Application::mainLoop()
{
	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			currentTime = timeGetTime();
			if ((currentTime - fpsLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				fpsCnt = frameCount * 1000 / (currentTime - fpsLastTime);
#endif
				fpsLastTime = currentTime;
				frameCount = 0;
			}

			if ((currentTime - execLastTime) >= (1000 / 60))
			{
#ifdef _DEBUG
				//PrintDebugProc("FPS:%d\n", fpsCnt);
#endif
				execLastTime = currentTime;

				update();

				draw();

				frameCount++;
			}
		}
	}
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
	directX3dObj->draw();
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
		nullptr,
		LoadCursor(nullptr, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		nullptr,
		ClassName,
		nullptr
	};
	HWND wndHandle;

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
		nullptr,
		nullptr,
		instance,
		nullptr);

	return wndHandle;
}