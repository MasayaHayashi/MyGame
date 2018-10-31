//����������������������������������������������
// Application.cpp
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "Application.h"
#include "DirectX3D.h"
#include <Windows.h>
#include <time.h>
#include <memory>

//������������������������������
// �R���X�g���N�^
//������������������������������
Application::Application()
{
	directX3dObj.reset(NEW DirectX3D());
}

//������������������������������
// �f�X�g���N�^
//������������������������������
Application::~Application()
{

}

//������������������������������
// ������
//������������������������������
void Application::initialize(HINSTANCE& instance, INT& cmdShow)
{
	
#if _DEBUG
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	HWND windowHandle = createWindow(instance);

	// �t���[���J�E���g������
	timeBeginPeriod(1);
	execLastTime = fpsLastTime = timeGetTime();
	currentTime  = frameCount = 0;

	ShowWindow(windowHandle, cmdShow);
	UpdateWindow(windowHandle);

	directX3dObj->init(windowHandle);
}

//������������������������������
// ���C�����[�v
//������������������������������
const void Application::mainLoop()
{
	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			currentTime = timeGetTime();
			if ((currentTime - fpsLastTime) >= 500)	// 0.5�b���ƂɎ��s
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

//������������������������������
// �X�V
//������������������������������
void Application::update()
{

}

//������������������������������
// �`��
//������������������������������
const void Application::draw()
{
	directX3dObj->draw();
}

//������������������������������
// �I������
//������������������������������
void Application::finalize()
{
	
}

//������������������������������
// �v���V�[�W��
//������������������������������
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
		case VK_ESCAPE:					// [ESC]�L�[�������ꂽ
			DestroyWindow(wnd);			// �E�B���h�E��j������悤�w������
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(wnd, msg, wparam, lparam);
}

//������������������������������
// �A�v���p������
//������������������������������
const bool Application::isAppContinuation()
{
	return appContinuation;
}

//������������������������������
// 
//������������������������������
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

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
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