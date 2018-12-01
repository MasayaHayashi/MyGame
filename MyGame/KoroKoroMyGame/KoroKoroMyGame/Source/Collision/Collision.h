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
#include "../Transform/Transform.h"
#include <list>

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

// ===== クラスの前方宣言 =====
class Pawn;
class Collider;
class Player;

// ===== クラス定義 =====
class Collision
{
public:
	Collision();	// コンストラクタ
	~Collision();	// デストラクタ

	void update();

	static void registerList(TransformData setTransformData,std::string keyName);




	UINT CheckCollisionField(Player *pPlayer, Pawn *pPawnB, Pawn *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, D3DXVECTOR3 &fLength, D3DXVECTOR3 DestVec);
	UINT CheckCollisionWall(Player *pPlayer, Pawn *pPawnB, Pawn *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, D3DXVECTOR3 &fLength, D3DXVECTOR3 DestVec);

	void CheckCollisionBlock(Pawn *pSelectBlock, Pawn *pGameObj);

	bool IsHitSphereToSphere(Pawn *, Pawn*);					// 球と球のあたり判定
	bool IsHitAABB(Pawn * ,Pawn *);								// AABBのあたり判定
	bool IsHitAABBItem(Player *pPlayer, Pawn *pPawn);				// AABBのアイテム用判定
	INT IsHitRayToMesh(Pawn *, Pawn *, LPD3DXVECTOR3 , LPD3DXVECTOR3 , bool , LPD3DXVECTOR3 , LPD3DXVECTOR3 ,LPD3DXVECTOR3);			// レイと三角形のあたり判定

private:
	bool IntersectA(Pawn* pField, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, LPD3DXMATRIX pWorld);
	INT  Intersect(Pawn *,LPD3DXVECTOR3 , LPD3DXVECTOR3 , bool , LPD3DXVECTOR3 , LPD3DXVECTOR3 , LPD3DXVECTOR3 );
	void SwitchHitType(Pawn *, Pawn *);


	static std::unordered_map<std::string, Transform> collisionMapes;
};

#endif