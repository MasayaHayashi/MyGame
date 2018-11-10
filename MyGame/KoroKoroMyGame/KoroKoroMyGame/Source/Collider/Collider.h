//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Collider.h
//
// 説明：
// [Pawn]クラスに持たせる。あたり判定を取るための情報を持たせたクラス
// UnityのColliderのイメージ
//
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef COLLIDER_H
#define COLLIDER_H

// ===== インクルード部 =====
#include "d3dx9.h"
#include <memory>

// ===== クラスの前方宣言 =====
class Figure;

// ===== クラス定義 =====
class Collider
{
public:
	Collider();				// コンストラクタ
	Collider(D3DXVECTOR3 , D3DXVECTOR3);
	~Collider();				// デストラクタ

	void initializeCollider();
	void initializeCollider(D3DXVECTOR3 Pos, D3DXVECTOR3 CollisionSize, D3DXVECTOR3 CenterPos);
	void UninitializeCollider();
	void UpdateCollider(D3DXMATRIX,D3DXCOLOR);
	void DrawCollider();
	
	void setUsedFlg(bool);
	void setPosition(D3DXVECTOR3 SetPos);
	std::unique_ptr <Figure> figurePtr = nullptr;


private:


	bool							isUsed;				// 使用フラグ
	D3DXVECTOR3						Pos;				// 位置
	D3DXVECTOR3						Size;				// 大きさ
	INT								nNumVertex;			// 総頂点数
	INT								nNumVertexIndex;	// 総インデックス数
	INT								nNumPolygon;		// 総ポリゴン数
	LPDIRECT3DVERTEXBUFFER9			pCubeVtx;			// 立方体頂点
	LPDIRECT3DTEXTURE9				pCubeTexture;		// テクスチャ

	D3DXMATRIX						mtxWorld;			// ワールドマトリックス
	D3DXMATRIX						mtxTranslate;
	D3DXMATRIX						mtxRot;
	D3DXMATRIX						mtxScale;

	D3DXVECTOR3						Translate;
	D3DXVECTOR3						Rot;
	D3DXVECTOR3						Scale;

};

#endif