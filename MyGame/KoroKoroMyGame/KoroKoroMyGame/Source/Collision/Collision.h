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

typedef struct
{
	D3DXVECTOR3 cross;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 length;
	bool isHit = false;
} RayHit;

// ===== クラスの前方宣言 =====
class Pawn;
class Collider;
class Player;

// ===== クラス定義 =====
class Collision
{
public:
	Collision();	
	~Collision();	
	Collision(Pawn* setPlayerPtr, Pawn* setFieldPtr);

	void update();

	static void registerList(Transform *setPawn,std::string keyName);
	INT isHitRayToMesh(Pawn *pPawnA, Pawn *pPawnB, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, bool bSegment, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, LPD3DXVECTOR3 Length);
	INT Intersect(Pawn *pField, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, bool bSegment, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, LPD3DXVECTOR3 pFLength);

	UINT checkCollisionField(Pawn *pPlayer, Pawn *pPawnB, Pawn *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, D3DXVECTOR3 &fLength, D3DXVECTOR3 DestVec);

	bool IsHitSphereToSphere(Pawn *, Pawn*);					// 球と球のあたり判定
	bool isHitAABB(const Pawn pawnA, const Pawn pawnB) const;
	bool IsHitAABBItem(Player *pPlayer, Pawn *pPawn);				// AABBのアイテム用判定

	static const Transform* getTransform(std::string keyName, INT index);
	static const RayHit*    getRayHitData(std::string keyName, UINT index);
	static void setVelocity(std::string keyName, UINT index,D3DXVECTOR3 velocity);
	void finalize(std::string keyName);

private:
	bool IntersectA(Pawn* pField, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, LPD3DXMATRIX pWorld);

	void SwitchHitType(Pawn *, Pawn *);

	void checkPlayerCollision();

	Pawn* playerPtr;
	Pawn* fieldPtr;

	static std::unordered_map<std::string, std::list<Transform*>>	collisionMapes;
	static std::unordered_map<std::string, std::list<RayHit*>>		rayHitMapes;
};

#endif