//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// DirectX3D.h
// DirectX9.0c用クラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef DIRECTX_3D_H
#define DIRECTX_3D_H

// ===== インクルード部 =====
#include "d3dx9.h"
#include <array>

// ===== クラス定義 =====
class DirectX3D
{
public :
	DirectX3D();
	~DirectX3D();

	HRESULT init(HWND& wnd);

	HRESULT initDebugProc();

	const void draw();

//	static void printDebugProc(CHAR *pFmt);

private :
	LPDIRECT3D9			pD3D		 = nullptr;
	LPDIRECT3DDEVICE9	pD3DDevice	 = nullptr;


	LPD3DXEFFECT		pEffect		 = nullptr;

	LPD3DXFONT			pD3DXFont	 = nullptr;
	CHAR		debug[1024]  = { "\0" };

	BOOL				isWindowMode = true;

	const void	drawDebugProc();
};

#endif