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
#include <atlbase.h>

// ===== クラス定義 =====
class DirectX3D
{
public :
	DirectX3D();
	~DirectX3D();

	HRESULT init(HWND& wnd);
	const void draw();

	static LPDIRECT3DDEVICE9 getDevice();

private :
	CComPtr<IDirect3DDevice9> directXDevice = nullptr;
	CComPtr<IDirect3D9>		  directXObj	= nullptr;
	CComPtr<ID3DXEffect>	  effectObj		= nullptr;

	BOOL				isWindowMode	= true;

	LPD3DXFONT			pD3DXFont	 = nullptr;
	CHAR				debug[1024]  = { "\0" };

	HRESULT initDebugProc();
	void	updateDebugProc();
	const void	drawDebugProc();
	void	finalizeDebugProc();

};

#endif