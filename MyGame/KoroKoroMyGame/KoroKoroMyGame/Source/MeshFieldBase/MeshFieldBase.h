//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_MeshField.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef MESHFIELD_BASE_H
#define MESHFIELD_BASE_H

// ===== インクルード部 =====
#include "../Pawn/Pawn.h"

class C_MESH_FIELD_BASE
{
public:
	C_MESH_FIELD_BASE();
	~C_MESH_FIELD_BASE();

	virtual	void initializeMeshField()   = 0;
	virtual	void UninitMeshField() = 0;
	virtual	void UpdateMeshField() = 0;
	virtual	void DrawMeshField();		// フィールド描画
	void CreateTexture();				// テクチャ生成
	HRESULT MakeModel();				// Xファイルから地形生成
	void DestroyFieldMesh();
	void SetTexture();

protected:
	virtual HRESULT MakeVertexField();		// フィールド頂点情報生成
	float fSizeBlockX, fSizeBlockZ;			// 1ブロックの大きさ
	int nNumBlockX, nNumBlockZ;				// 総ブロック数
	int nNumVertex;							// 総頂点数
	int nNumVertexIndex;					// 総インデックス数
	int nNumPolygon;						// 総ポリゴン数
	
	INT currentAnimPattern;
	INT animPattern;
	INT texPatternDivideX, texPatternDivideY;
	FLOAT texUV_SizeX, texUV_SizeY;

	char	fileName[256];					// メッシュのファイル名
	char	texFileName[256];					// テクスチャのファイル名

	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffField;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  pD3DIdxBuffField;	// インデックスバッファインターフェースへのポインタ

	LPDIRECT3DTEXTURE9	pD3DTextureField;		// テクスチャへのポインタ
	LPD3DXMESH			pD3DXMeshField;			// メッシュ情報へのポインタ
	LPD3DXBUFFER		pD3DXBuffMatField;		// マテリアル情報へのポインタ
	DWORD				nNumMatField;			// マテリアル情報の数
	D3DXMATRIX			mtxWorldField;			// ワールドマトリックス

	D3DXVECTOR3			PosField;				// 現在の位置
	D3DXVECTOR3			MoveField;				// ポーン移動量
	D3DXVECTOR3			RotField;				// 現在の向き
	D3DXVECTOR3			ScaleField;				// 拡大率
	D3DXVECTOR3			CenterPos;				// 中心座標
	D3DXVECTOR3			collitionBox;			// あたり判定用ボックス

	D3DXVECTOR3			MaxVtx;					// 最大頂点位置
	D3DXVECTOR3			MinVtx;					// 最小頂点位置

	BOOL				bLoadMesh;				// Xファイルから地形を読み込むか、自作で地形を作るか
	FLOAT				fCollisionRadus;		// あたり判定用半径

private:
	DWORD				dwNumVtx;				// 頂点数
	MESH_VTX			*pVtx;					// 頂点情報へのアクセス用ポインタ
};

#endif
