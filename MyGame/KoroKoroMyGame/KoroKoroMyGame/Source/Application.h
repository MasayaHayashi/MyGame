//����������������������������������������
// application.h
// �A�v���P�[�V�����N���X
// Author : Masaya Hayashi
//����������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef APPLICATION_H
#define APPLICATION_H

// ===== �C���N���[�h�� =====
#include <string>
#include <memory>
#include "Linklib.h"
#include "d3dx9.h"

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

// ===== �萔�E�}�N����` =====
#if defined(DEBUG) || defined(_DEBUG)
	#define NEW  new(_NORMAL_BLOCK, __FILE__, __LINE__)  // new �ɂ�郁�������[�N���o
#else
	#define NEW  new									 // �����[�X�̏ꍇ�ʏ�̃������m��
#endif

// ===== �N���X�̑O���錾 =====
class DirectX3D;

// ===== �N���X��` =====
class Application
{
public :
	static constexpr UINT ScreenWidth		= 1280;
	static constexpr UINT ScreenHeight		= 720;
	static constexpr FLOAT ScreenCenterX	= ScreenWidth  * 0.5f;
	static constexpr FLOAT ScreenCenterY	= ScreenHeight * 0.5f;
	static constexpr INT   Vertex			= 4;
	static constexpr INT   Polygon			= 2;

	Application();
	~Application();

	void initialize(HINSTANCE& instance, INT& cmdShow);
	void update();
	const void draw();
	void finalize();

	const void mainLoop();

	HWND createWindow(HINSTANCE instance);

	const bool isAppContinuation();

private :
	static constexpr CHAR* ClassName   = "aaa";
	static constexpr CHAR* WindowName  = "aaa";
	static constexpr INT   FvfVertex2d = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	static constexpr INT   FvfVertex3d = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	std::unique_ptr <DirectX3D> directX3dObj;

	DWORD execLastTime;
	DWORD fpsLastTime;
	DWORD currentTime;
	DWORD frameCount;

	MSG msg;

	INT fpsCnt;

	bool appContinuation = true;
};

#endif