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

// ===== �N���X�̑O���錾 =====

// ===== �N���X��` =====
class Figure
{
public:
	static constexpr  D3DCOLOR HitBoxcolor = D3DCOLOR_ARGB(128, 255, 0, 0);
	static constexpr  D3DCOLOR DeaultBoxcolor = D3DCOLOR_ARGB(128, 0, 255, 255);
	static constexpr  D3DCOLOR EnemyBoxcolor = D3DCOLOR_ARGB(128, 128, 128, 0);
	static constexpr  D3DCOLOR FieldBoxcolor = D3DCOLOR_ARGB(128, 0, 128, 0);

	Figure();							// �R���X�g���N�^
	Figure(D3DXVECTOR3 ,D3DXVECTOR3);	// �R���X�g���N�^
	~Figure();						// �f�X�g���N�^

	HRESULT MakeVtxCube(D3DXVECTOR3 , D3DXVECTOR3 );		// �����̒��_����
	void CreateTexture();									// �e�N�X�`������
	void initializeFigure();
	void initializeFigure(D3DXVECTOR3 , D3DXVECTOR3 , D3DXVECTOR3 );
	void UpdateFigure(D3DXMATRIX,D3DXCOLOR);
	void DrawFigure();
	void UninitFigure();

	D3DXVECTOR3 GetSize();
	D3DXVECTOR3 GetPos();
	void SetPos(D3DXVECTOR3);

	void SetMatcolor(D3DCOLOR);
	void setUsedFlg(bool);
private:
#define FVF_TVERTEX	    (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

	bool							isUsed;				// �g�p�t���O
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
	D3DXCOLOR						color;				// �}�e���A���w��F

	LPD3DXMESH						meshPtr;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER					materialBufferPtr;		// �}�e���A�����ւ̃|�C���^
	DWORD							lNumMat;			// �}�e���A����
};

#endif