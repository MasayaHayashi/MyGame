//����������������������������������������������������������������
// Figure.h
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef FIGURE_H
#define FIGURE_H

// ===== �C���N���[�h�� =====
#include "d3dx9.h"
#include <atlbase.h>

// ===== �N���X�̑O����` =====

// ===== �N���X��` =====
class Figure
{
public:
	static constexpr  D3DCOLOR HitBoxColor = D3DCOLOR_ARGB(128, 255, 0, 0);
	static constexpr  D3DCOLOR DeaultBoxColor = D3DCOLOR_ARGB(128, 0, 255, 255);
	static constexpr  D3DCOLOR EnemyBoxColor = D3DCOLOR_ARGB(128, 128, 128, 0);
	static constexpr  D3DCOLOR FieldBoxColor = D3DCOLOR_ARGB(128, 0, 128, 0);

	Figure();							// �R���X�g���N�^
	Figure(D3DXVECTOR3 ,D3DXVECTOR3);	// �R���X�g���N�^
	~Figure();						// �f�X�g���N�^

	HRESULT MakeVtxCube(D3DXVECTOR3 , D3DXVECTOR3 );		// �����̒��_����
	void CreateTexture();									// �e�N�X�`������
	void InitFigure();
	void InitFigure(D3DXVECTOR3 , D3DXVECTOR3 , D3DXVECTOR3 );
	void UpdateFigure(D3DXMATRIX,D3DXCOLOR);
	void DrawFigure();
	void UninitFigure();

	D3DXVECTOR3 GetSize();
	D3DXVECTOR3 GetPos();
	void SetPos(D3DXVECTOR3);

	void SetMatColor(D3DCOLOR);
	void SetUsedFlg(bool);
private:
#define FVF_TVERTEX	    (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

	bool							bUsed;				// �g�p�t���O
	D3DXVECTOR3						Pos;				// �ʒu
	D3DXVECTOR3						CenterPos;			// ���S�ʒu
	D3DXVECTOR3						Size;				// �傫��
	INT								nNumVertex;			// �����_��
	INT								nNumVertexIndex;	// ���C���f�b�N�X��
	INT								nNumPolygon;		// ���|���S����

	CComPtr<IDirect3DVertexBuffer9> cubeVtx;
	CComPtr<IDirect3DTexture9 >		cubeTexture;

	LPD3DXMESH						pBBox;
	D3DXMATRIX						mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX						mtxTranslate;
	D3DXMATRIX						mtxRot;
	D3DXMATRIX						mtxScale;

	D3DXVECTOR3						Translate;
	D3DXVECTOR3						Rot;
	D3DXVECTOR3						Scale;

	D3DXMATERIAL					Material;			// �}�e���A��
	D3DXCOLOR						Color;				// �}�e���A���w��F

	LPD3DXMESH						pD3DXMesh;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER					pD3DXBuffMat;		// �}�e���A�����ւ̃|�C���^
	DWORD							lNumMat;			// �}�e���A����
};

#endif