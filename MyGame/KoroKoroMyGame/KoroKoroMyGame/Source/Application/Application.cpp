//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Application.cpp
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Application.h"
#include "../DirectX3D/DirectX3D.h"
#include "../Audio/MyAudiere.h"
#include "../SceneManager/SceneManager.h"
#include "../KeyBoard/Keyboard.h"
#include <Windows.h>
#include <time.h>
#include <memory>

// ===== 静的メンバ =====
bool Application::appContinuation = true;

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Application::Application()
{
	directX3dPtr.reset(NEW DirectX3D());
	myAudierePtr.reset(NEW MyAudiere());
	SceneManager::create();
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

	directX3dPtr->initialize(windowHandle);
	keyBoardObj.initialize(instance,windowHandle);
	SceneManager::initialize();

	sceneLoadObj.initialize();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メインループ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Application::mainLoop()
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
				DirectX3D::printDebug("FPS:%d\n", fpsCnt);
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
	keyBoardObj.update();

	sceneLoadObj.update();

	DirectX3D::update();

	SceneManager::updateFade();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Application::draw() const
{
	directX3dPtr->draw();

	SceneManager::drawFade();
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
		Application::closeApp();

		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wparam)
		{
		case VK_ESCAPE:					// [ESC]キーが押された
			Application::closeApp();

		//	DestroyWindow(wnd);			// ウィンドウを破棄するよう指示する
			PostQuitMessage(0);

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
// アプリ終了
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Application::closeApp()
{
	SceneManager::finalize();

	appContinuation = false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ウィンドウ生成
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