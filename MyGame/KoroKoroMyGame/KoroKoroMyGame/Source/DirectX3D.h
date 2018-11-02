//����������������������������������������
// DirectX3D.h
// DirectX9.0c�p�N���X
// Author : Masaya Hayashi
//����������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef DIRECTX_3D_H
#define DIRECTX_3D_H

// ===== �C���N���[�h�� =====
#include "d3dx9.h"
#include <memory>

// ===== �N���X��` =====
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