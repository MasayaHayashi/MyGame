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
#include <wrl.h>
#include <string>

// ===== クラス定義 =====
class DirectX3D
{
public :
	static const constexpr INT VertexSize  = 4;
	static const constexpr INT PolygonSize = 2;

	DirectX3D();
	~DirectX3D();

	HRESULT initialize(HWND& wnd);

	void draw() const;
	static void printDebug(CHAR *fmt,...);

	static LPDIRECT3DDEVICE9  getDevice();
	static LPD3DXEFFECT		  getEffect();

private :
	static Microsoft::WRL::ComPtr<IDirect3DDevice9>   directXDevice;
	static Microsoft::WRL::ComPtr<ID3DXEffect>		  directXEffect;
	Microsoft::WRL::ComPtr<IDirect3D9>				  directXObj		= nullptr;
	Microsoft::WRL::ComPtr<ID3DXFont>				  directXFont		= nullptr;

	bool		 isWindowMode	= true;

	static CHAR  debug[1024];	// TODO : string型に変更

	HRESULT		initializeDebugProc();
	void		updateDebugProc();
	void		drawDebugProc() const;
	void		finalizeDebugProc();

};

#endif