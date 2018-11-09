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
#include <atlbase.h>
#include <string>

// ===== �N���X��` =====
class DirectX3D
{
public :
	static const constexpr INT VertexSize  = 4;
	static const constexpr INT PolygonSize = 2;

	DirectX3D();
	~DirectX3D();

	HRESULT initialize(HWND& wnd);

	const void draw();
	static const void printDebug(CHAR *fmt,...);

	static const LPDIRECT3DDEVICE9	getDevice();
	static const LPD3DXEFFECT 		getEffect();

private :
	static CComPtr<IDirect3DDevice9>  directXDevice;
	static CComPtr<ID3DXEffect>		  directXEffect;
	CComPtr<IDirect3D9>				  directXObj		= nullptr;
	CComPtr<ID3DXFont>				  directXFont		= nullptr;

	bool		 isWindowMode	= true;

	static CHAR  debug[1024];	// TODO : string�^�ɕύX

	HRESULT		initializeDebugProc();
	void		updateDebugProc();
	const void	drawDebugProc();
	void		finalizeDebugProc();

};

#endif