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
#include <array>

// ===== �N���X��` =====
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