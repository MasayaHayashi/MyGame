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
#include <memory>

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

	std::unique_ptr<LPDIRECT3DDEVICE9> pDirectXDevice;
	std::unique_ptr<LPDIRECT3D9>  pDirectXObj;
	std::unique_ptr<LPD3DXEFFECT> pEffectObj;

	/*
	LPDIRECT3DDEVICE9	pDirectXDevice  = nullptr;
	LPDIRECT3D9			pDirectXObj		= nullptr;
	LPD3DXEFFECT		pEffectObj		= nullptr;
	*/

	BOOL				isWindowMode	= true;

	LPD3DXFONT			pD3DXFont	 = nullptr;
	CHAR				debug[1024]  = { "\0" };

	HRESULT initDebugProc();
	void	updateDebugProc();
	const void	drawDebugProc();
	void	finalizeDebugProc();

};

#endif