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
#include "Linklib.h"
#include "d3dx9.h"
// メモリリーク検出用
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

// ===== 定数・マクロ定義 =====
#if defined(DEBUG) || defined(_DEBUG)
	#define NEW  new(_NORMAL_BLOCK, __FILE__, __LINE__)  // new によるメモリリーク検出でCPPファイル名と行数出力指定
#else
	#define NEW  new									 // リリースの場合通常のメモリ確保へ
#endif

// ===== クラス定義 =====
class Application
{
public :
	static constexpr FLOAT ScreenWidth		= 1280;
	static constexpr FLOAT ScreenHeight		= 720;
	static constexpr FLOAT ScreenCenterX	= ScreenWidth  * 0.5f;
	static constexpr FLOAT ScreenCenterY	= ScreenHeight * 0.5f;
	static constexpr INT   Vertex			= 4;
	static constexpr INT   Polygon			= 2;

	Application();
	~Application();

	void initialize(HINSTANCE instance);
	void update();
	const void draw();
	void finalize();

	HWND createWindow(HINSTANCE instance);

	const bool isAppContinuation();

private :
	static constexpr CHAR* ClassName   = "aaa";
	static constexpr CHAR* WindowName  = "aaa";
	static constexpr INT   FvfVertex2d = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	static constexpr INT   FvfVertex3d = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	DWORD execLastTime;
	DWORD fpsLastTime;
	DWORD currentTime;
	DWORD frameCount;

	bool appContinuation;
};

#endif