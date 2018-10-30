//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// エントリーポイント
// main.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include <Windows.h>
#include "Application.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	
	UNREFERENCED_PARAMETER(lpCmdLine);	

	Application applivationObj;

	applivationObj.initialize();

	while (applivationObj.isAppContinuation())
	{
		applivationObj.update();
		applivationObj.draw();
	}

	applivationObj.finalize();

	return 0;
}