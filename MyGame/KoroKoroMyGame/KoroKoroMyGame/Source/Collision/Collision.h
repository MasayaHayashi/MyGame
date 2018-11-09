//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Collision.h
// 
// 説明：
// 判定を取るための更新処理などをまとめたクラス
// colliderと混同しない。
// 
// Author: Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef COLLISION_H
#define COLLISION_H

// ===== インクルード部 =====
#include "../Pawn/Pawn.h"

// ===== 列挙体定義 =====

// ===== 構造体定義 =====
typedef struct
{
	D3DXVECTOR3 start;
	D3DXVECTOR3 end;
} SEGMENT;

typedef struct
{
	D3DXVECTOR3 P0;
	D3DXVECTOR3 P1;
	D3DXVECTOR3 P2;

} TRIANGLE;

// ===== クラスの前方定義 =====
class C_3DPAWN;
class Collider;
class C_PLAYER;

// ===== クラス定義 =====
class C_COLLISION
{
public:
	C_COLLISION();	// コンストラクタ
	~C_COLLISION();	// デストラクタ

	UINT CheckCollisionField(C_PLAYER *pPlayer, C_3DPAWN *pPawnB, C_3DPAWN *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, D3DXVECTOR3 &fLength, D3DXVECTOR3 DestVec);
	UINT CheckCollisionWall(C_PLAYER *pPlayer, C_3DPAWN *pPawnB, C_3DPAWN *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, D3DXVECTOR3 &fLength, D3DXVECTOR3 DestVec);

	void CheckCollisionBlock(C_3DPAWN *pSelectBlock, C_3DPAWN *pGameObj);

	bool IsHitSphereToSphere(C_3DPAWN *, C_3DPAWN*);					// 球と球のあたり判定
	bool IsHitAABB(C_3DPAWN * ,C_3DPAWN *);								// AABBのあたり判定
	bool IsHitAABBItem(C_PLAYER *pPlayer, C_3DPAWN *pPawn);				// AABBのアイテム用判定
	INT IsHitRayToMesh(C_3DPAWN *, C_3DPAWN *, LPD3DXVECTOR3 , LPD3DXVECTOR3 , bool , LPD3DXVECTOR3 , LPD3DXVECTOR3 ,LPD3DXVECTOR3);			// レイと三角形のあたり判定

private:
	bool IntersectA(C_3DPAWN* pField, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, LPD3DXMATRIX pWorld);
	INT  Intersect(C_3DPAWN *,LPD3DXVECTOR3 , LPD3DXVECTOR3 , bool , LPD3DXVECTOR3 , LPD3DXVECTOR3 , LPD3DXVECTOR3 );
	void SwitchHitType(C_3DPAWN *, C_3DPAWN *);
};

#endif