//
// application.h
// AvP[VNX
// Author : Masaya Hayashi
//

// ===== ½dCN[hh~ =====
#ifndef APPLICATION_H
#define APPLICATION_H

// ===== CN[h =====
#include <string>
#include "Linklib.h"
#include "d3dx9.h"
// [Nop
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

// ===== èE}Nè` =====
#if defined(DEBUG) || defined(_DEBUG)
	#define NEW  new(_NORMAL_BLOCK, __FILE__, __LINE__)  // new Éæé[NoÅCPPt@C¼ÆsoÍwè
#else
	#define NEW  new									 // [XÌêÊíÌmÛÖ
#endif

// ===== NXè` =====
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