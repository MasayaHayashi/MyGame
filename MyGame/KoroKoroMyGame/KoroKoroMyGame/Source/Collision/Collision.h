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
	D3DXVECTOR3 Start;
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

typedef struct
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 look = D3DXVECTOR3(0.0f, 0.0f, 10.0f);
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 lookDest = D3DXVECTOR3(0.0f,0.0f,10.0f);

	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 fowerd = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

} CameraTransform;

// ===== クラスの前方宣言 =====
class Pawn;
class Collider;
class Player;

// ===== クラス定義 =====
class Collision final
{
public:
	static constexpr FLOAT HitLength = 1.5f;

	Collision();	
	~Collision();	
	Collision(Pawn* setPlayerPtr, Pawn* setFieldPtr);

	void update();

	static void registerList(Transform *setPawn,std::string keyName);
	static void registerList(CameraTransform *setCamera, std::string keyName);

	static void release(std::string keyName);

	INT isHitRayToMesh(Pawn *pPawnA, Pawn *pPawnB, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, bool bSegment, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, LPD3DXVECTOR3 Length);
	INT Intersect(Pawn *pField, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, bool bSegment, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, LPD3DXVECTOR3 &pFLength);

	UINT checkCollisionField(Pawn *pPlayer, Pawn *pPawnB, Pawn *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, D3DXVECTOR3 &fLength, D3DXVECTOR3 DestVec);

	bool IsHitSphereToSphere(Pawn *, Pawn*);

	static const Transform*		  getTransform(std::string keyName, INT index);
	static const CameraTransform* getCameraTransform(std::string keyName, INT index);
	static const RayHit*    getRayHitData(std::string keyName, UINT index);
	static const std::list<Transform*> getTransform(std::string keyName);
	static const size_t			  getSize(std::string keyName);
	static void setVelocity(std::string keyName, UINT index,D3DXVECTOR3 velocity);
	void finalize(std::string keyName);

	void registerPlayer(Pawn* playerPtr);
	void registerField(Pawn* fieldPtr);

	const bool isHitAABB(Transform pPawnA, Transform pPawnB);

	static const D3DXVECTOR3 getCross();

private:
	bool IntersectA(Pawn* pField, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, LPD3DXMATRIX pWorld);

	std::list<Pawn*> playersPtr;
	std::list<Pawn*> fieldPtres;

	static D3DXVECTOR3 cross;

	static std::unordered_map < std::string, std::list<Transform*>> collisionMapes;
	static std::unordered_map < std::string, std::vector<RayHit* >> rayHitMapes;
	static std::unordered_map < std::string, std::list<CameraTransform*>> cameraTransforms;

};

#endif