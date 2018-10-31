//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// application.h
// アプリケーションクラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef APPLICATION_H
#define APPLICATION_H

// ===== インクルード部 =====
#include <string>
#include <memory>
#include "Linklib.h"
#include "d3dx9.h"

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

// ===== 定数・マクロ定義 =====
#if defined(DEBUG) || defined(_DEBUG)
	#define NEW  new(_NORMAL_BLOCK, __FILE__, __LINE__)  // new によるメモリリーク検出
#else
	#define NEW  new									 // リリースの場合通常のメモリ確保
#endif

// ===== クラスの前方宣言 =====
class DirectX3D;

// ===== クラス定義 =====
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