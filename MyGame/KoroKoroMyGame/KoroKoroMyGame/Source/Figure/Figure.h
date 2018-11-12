//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Figure.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef FIGURE_H
#define FIGURE_H

// ===== インクルード部 =====
#include "d3dx9.h"
#include <atlbase.h>

// ===== クラスの前方宣言 =====

// ===== クラス定義 =====
class Figure
{
public:
	static constexpr  D3DCOLOR HitBoxcolor = D3DCOLOR_ARGB(128, 255, 0, 0);
	static constexpr  D3DCOLOR DeaultBoxcolor = D3DCOLOR_ARGB(128, 0, 255, 255);
	static constexpr  D3DCOLOR EnemyBoxcolor = D3DCOLOR_ARGB(128, 128, 128, 0);
	static constexpr  D3DCOLOR FieldBoxcolor = D3DCOLOR_ARGB(128, 0, 128, 0);

	Figure();							// コンストラクタ
	Figure(D3DXVECTOR3 ,D3DXVECTOR3);	// コンストラクタ
	~Figure();						// デストラクタ

	HRESULT MakeVtxCube(D3DXVECTOR3 , D3DXVECTOR3 );		// 立方体頂点生成
	void CreateTexture();									// テクスチャ生成
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

	bool							isUsed;				// 使用フラグ
	D3DXVECTOR3						Pos;				// 位置
	D3DXVECTOR3						CenterPos;			// 中心位置
	D3DXVECTOR3						Size;				// 大きさ
	INT								nNumVertex;			// 総頂点数
	INT								nNumVertexIndex;	// 総インデックス数
	INT								nNumPolygon;		// 総ポリゴン数

	CComPtr<IDirect3DVertexBuffer9> cubeVtx;
	CComPtr<IDirect3DTexture9 >		cubeTexture;

	LPD3DXMESH						pBBox;
	D3DXMATRIX						mtxWorld;			// ワールドマトリックス
	D3DXMATRIX						mtxTranslate;
	D3DXMATRIX						mtxRot;
	D3DXMATRIX						mtxScale;

	D3DXVECTOR3						Translate;
	D3DXVECTOR3						Rot;
	D3DXVECTOR3						Scale;

	D3DXMATERIAL					Material;			// マテリアル
	D3DXCOLOR						color;				// マテリアル指定色

	LPD3DXMESH						meshPtr;			// メッシュ情報へのポインタ
	LPD3DXBUFFER					materialBufferPtr;		// マテリアル情報へのポインタ
	DWORD							lNumMat;			// マテリアル数
};

#endif