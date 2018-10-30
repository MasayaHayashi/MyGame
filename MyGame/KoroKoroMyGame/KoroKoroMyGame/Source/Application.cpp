//����������������������������������������������
// Application.cpp
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "Application.h"
#include <Windows.h>
#include <time.h>

//������������������������������
// �R���X�g���N�^
//������������������������������
Application::Application()
{

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

	// �t���[���J�E���g������
	timeBeginPeriod(1);				// ����\��ݒ�
	execLastTime = fpsLastTime = timeGetTime();
	currentTime = frameCount = 0;

	//�����_���̏�����
	srand((unsigned)time(NULL));

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(windowHandle, nCmdShow);
	UpdateWindow(windowHandle);
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
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		ClassName,
		NULL
	};
	HWND wndHandle;
	MSG msg;

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
		NULL,
		NULL,
		instance,
		NULL);

	return wndHandle;
}