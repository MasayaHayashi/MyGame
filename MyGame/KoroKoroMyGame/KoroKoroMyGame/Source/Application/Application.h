//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// application.h
// アプリケーションクラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef APPLICATION_H
#define APPLICATION_H

// ===== インクルード部 =====
#include "../CrtDebug/CrtDebug.h"
#include <string>
#include <memory>
#include "../Linklib.h"
#include "d3dx9.h"
#include "../KeyBoard/Keyboard.h"
#include "../Scene/Load/SceneLoad.h"

// ===== クラスの前方宣言 =====
class DirectX3D;
class MyAudiere;
class Keyboard;

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
	void draw() const ;
	void finalize();

	void mainLoop();

	static void closeApp();
	static void errorMassage();

	HWND createWindow(HINSTANCE instance);

	const bool isAppContinuation();

private :
	static constexpr CHAR* ClassName   = "KoroKoroMyGame";
	static constexpr CHAR* WindowName  = "KoroKoroMyGame";
	static constexpr INT   FvfVertex2d = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	static constexpr INT   FvfVertex3d = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	std::unique_ptr <DirectX3D> directX3dPtr;
	std::unique_ptr <MyAudiere> myAudierePtr;
	
	Keyboard  keyBoardObj;
	SceneLoad sceneLoadObj;

	DWORD execLastTime;
	DWORD fpsLastTime;
	DWORD currentTime;
	DWORD frameCount;

	MSG msg;

	INT fpsCnt;

	static bool appContinuation;
};

#endif