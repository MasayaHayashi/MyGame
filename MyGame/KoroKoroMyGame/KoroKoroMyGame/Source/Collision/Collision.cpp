//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Collision.cpp
//
// 説明：
// 判定を取るための更新処理などをまとめたクラス
// colliderクラスとは別
//
// Auhotor : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Collision.h"
#include "../Player/Player.h"
#include "../Pawn/Pawn.h"
#include "../DirectX3D/DirectX3D.h"
#include "../SceneManager/SceneManager.h"
#include "../MyVector3/MyVector3.h"
#include "../GameManager/GameManager.h"
#include "../MyDelete/MyDelete.h"

// ===== 静的メンバ =====
std::unordered_map<std::string, std::list<		Transform*		 >>	Collision::collisionMapes;
std::unordered_map<std::string, std::vector<	RayHit*>		 >	Collision::rayHitMapes;
std::unordered_map<std::string, std::list<		CameraTransform* >>	Collision::cameraTransforms;
D3DXVECTOR3 Collision::cross;
INT Collision::hitIndex = 0;

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Collision::Collision()
{
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Collision::Collision(Pawn* setPlayerPtr,Pawn* setFieldPtr)
{
	playersPtr.push_back(setPlayerPtr);
	fieldPtres.push_back(setFieldPtr);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Collision::~Collision()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::update()
{
	D3DXVECTOR3 normal, length;

	UINT playerIndex = 0;
	
	checkCollision("Player", "heart.x");

	checkCollisionBlock();

	checkCollision("Player","star.x");

	for (auto fieldPtr : fieldPtres)
	{
		if (!fieldPtr->getUsedFlg())
		{
			continue;
		}

		if (!fieldPtr->getIsFieldObject())
		{
			continue;
		}

		if (checkCollisionField(playersPtr.front(), playersPtr.front(), fieldPtr, cross, normal, rayHitMapes["Player"].front()->length, D3DXVECTOR3(0.0f, -1.0f, 0.0f)))
		{
			if (std::abs(rayHitMapes["Player"].front()->length) < 1.0f)
			{
				rayHitMapes["Player"][playerIndex]->isHit = true;
				return;
			}
			else
			{
				rayHitMapes["Player"][playerIndex]->isHit = false;
			}
		}
		else
		{
			rayHitMapes["Player"][playerIndex]->isHit = false;
		}
	}
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 登録
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::registerList(Transform *setPawn,std::string keyName)
{
	collisionMapes[keyName].push_back( setPawn);

	rayHitMapes[keyName].push_back(NEW RayHit());
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 登録
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::registerList(CameraTransform *setCamera, std::string keyName)
{
	cameraTransforms[keyName].push_back( setCamera);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::release(std::string keyName)
{
	if (collisionMapes[keyName].empty())
	{
		return;
	}

	for (size_t playerIndex = 0; playerIndex < collisionMapes[keyName].size(); playerIndex)
	{
		collisionMapes[keyName].pop_back();
	}
//	collisionMapes[keyName].clear();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::finalize(std::string keyName)
{
	for (auto & rayHitMap : rayHitMapes)
	{
		for (auto &vector : rayHitMap.second)
		{
			Mydelete::safeDelete(vector);
		}
	}

	for (UINT playerIndex = 0; playerIndex < collisionMapes[keyName].size(); playerIndex++)
	{
		Mydelete::safeDelete(collisionMapes[keyName].back());
		collisionMapes[keyName].pop_back();
	}

	collisionMapes[keyName].clear();

	for (UINT playerIndex = 0; playerIndex < rayHitMapes[keyName].size(); playerIndex++)
	{
		Mydelete::safeDelete(rayHitMapes[keyName].back());
		rayHitMapes[keyName].pop_back();
	}

	/*
	for (UINT playerCnt = 0; playerCnt < rayHitMapes["Player"].size(); playerCnt++)
	{
		Mydelete::safeDelete(rayHitMapes["Player"].back());
		rayHitMapes["Player"].pop_back();
	}

	for (UINT fieldCnt = 0; fieldCnt < rayHitMapes["field"].size(); fieldCnt++)
	{
		delete rayHitMapes["field"].back();
		rayHitMapes["field"].pop_back();
	}
	*/
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 地面とプレイヤーの衝突処理更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
UINT Collision::checkCollisionField(Pawn *pPlayer, Pawn *pPawnB, Pawn *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, FLOAT &length, D3DXVECTOR3 DestVec)
{
	INT	nIndx;

	// 線分と三角形の判定
	nIndx = isHitRayToMesh(pField, pPlayer, &pPlayer->getPosition(), &(pPlayer->getPosition() + DestVec), true, &Cross, &Normal, length);

	if (nIndx >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 球と球のあたり判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Collision::IsHitSphereToSphere(Pawn *pPawnA,Pawn *pPawnB)
{
	D3DXVECTOR3 SphereA = pPawnA->getOffset();
	D3DXVECTOR3 SphereB = pPawnB->getPosition();
	FLOAT fRadA = pPawnA->getCollisionRadius() * 1.5f;
	FLOAT fRadB = pPawnB->getCollisionRadius() * 1.5f;

	if (   (SphereA.x - SphereB.x) * (SphereA.x - SphereB.x)
		+  (SphereA.y - SphereB.y) * (SphereA.y - SphereB.y)
		+  (SphereA.z - SphereB.z) * (SphereA.z - SphereB.z)
		<= (fRadA + fRadB) * (fRadA + fRadB))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤーセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::registerPlayer(Pawn* playerPtr)
{
	playersPtr.push_back(playerPtr);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フィールドセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::registerField(Pawn* setFieldPtr)
{
	fieldPtres.push_back(setFieldPtr);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// レイと三角形のあたり判定
// 引数 : 
//		  pPawnA  : フィールドのメッシュ情報
//		  pPawnB  : プレイヤーのメッシュ情報
//		  pCross  : 当たった座標 
//		  pNormal : 法線ベクトル
//		  fLength : 位置からの長さ
// 戻り値:
//		  当たっている		⇒ 当たっている三角形の添え字
//		  当たっていない	⇒ -1
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
INT Collision::isHitRayToMesh(Pawn *pPawnA, Pawn *pPawnB, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, bool bSegment, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, FLOAT &length)
{
	// ワールドマトリックスの逆マトリックスを生成
	D3DXMATRIX mInvWorld;

	D3DXMatrixInverse(&mInvWorld, NULL, pPawnA->getWorldMtx());
	// レイを逆ワールド変換
	D3DXVECTOR3 vRayPos, vRayDir;
	D3DXVec3TransformCoord(&vRayPos, pRayPos, &mInvWorld);

	if (bSegment)
	{
		D3DXVec3TransformCoord(&vRayDir, pRayDir, &mInvWorld);
	}
	else
	{
		D3DXVec3TransformNormal(&vRayDir, pRayDir, &mInvWorld);
	}

	// レイとメッシュの交差判定
	INT nIndex = Intersect(pPawnA, &vRayPos, &vRayDir, bSegment, pCross, pNormal, length);

	if (nIndex >= 0) // 交差している場合
	{
		// 交点、法線をワールド変換
		if (pCross)
		{
			D3DXVec3TransformCoord(pCross, pCross, pPawnA->getWorldMtx());
		}
		if (pNormal)
		{
			D3DXVec3TransformNormal(pNormal, pNormal, pPawnA->getWorldMtx());
		}
	}
	return nIndex;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// AABBのあたり判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const bool Collision::isHitAABB(Transform pPawnA, Transform pPawnB)
{
	if (pPawnA.pos.x - pPawnA.collisionBox.x < pPawnB.pos.x	+ pPawnB.collisionBox.x &&
		pPawnA.pos.x + pPawnA.collisionBox.x > pPawnB.pos.x - pPawnB.collisionBox.x &&
		pPawnA.pos.y + pPawnA.collisionBox.y > pPawnB.pos.y				 &&
		pPawnA.pos.y < pPawnB.pos.y + pPawnB.collisionBox.y * 2 &&
		pPawnA.pos.z + pPawnA.collisionBox.z > pPawnB.pos.z				 &&
		pPawnA.pos.z  < pPawnB.pos.z + pPawnB.collisionBox.z * 2)
	{
		return true;
	}

	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// AABBのあたり判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const bool Collision::isHitAABB(Transform pPawnA, Pawn &pPawnB)
{
	if (pPawnA.pos.x - pPawnA.collisionBox.x < pPawnB.getPosition().x + pPawnB.getCollisionBox().x * 0.75f &&
		pPawnA.pos.x + pPawnA.collisionBox.x > pPawnB.getPosition().x - pPawnB.getCollisionBox().x * 0.75f &&
		pPawnA.pos.y - pPawnA.collisionBox.y < pPawnB.getPosition().y + pPawnB.getCollisionBox().y * 2.5f &&
		pPawnA.pos.y + pPawnA.collisionBox.y > pPawnB.getPosition().y - pPawnB.getCollisionBox().y * 2.5f &&
		pPawnA.pos.z - pPawnA.collisionBox.z < pPawnB.getPosition().z + pPawnB.getCollisionBox().z * 0.9f  &&
		pPawnA.pos.z + pPawnA.collisionBox.z > pPawnB.getPosition().z - pPawnB.getCollisionBox().z * 0.9f)
	{
		return true;
	}

	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 線分の判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
INT Collision::Intersect(Pawn *pField, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, bool bSegment, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, FLOAT &length)
{
	if (!pRayPos || !pRayDir)
	{
		return -1;
	}

	// レイ取得
	D3DXVECTOR3& P0 = *pRayPos;
	D3DXVECTOR3 W = *pRayDir;

	if (bSegment)
	{
		W -= P0;
	}

	DWORD dwNumIndx = pField->getIndxNum();			// 三角形の数取得	
	MESH_VTX *pVtx = pField->getVtxAcess();		// 頂点情報取得
	WORD	 *pIndx = pField->getIndxAcess();		// インデックス情報取得

													// 全三角形を探索
	for (DWORD i = 0; i < dwNumIndx; i += 3)
	{
		D3DXVECTOR3 P1(pVtx[pIndx[i + 0]].VtxPos);
		D3DXVECTOR3 P2(pVtx[pIndx[i + 1]].VtxPos);
		D3DXVECTOR3 P3(pVtx[pIndx[i + 2]].VtxPos);


		// 辺を示すベクトルを取得
		D3DXVECTOR3 V1(P2 - P1);
		D3DXVECTOR3 V2(P3 - P2);

		// 例外処理
		if (V1.y >= pRayDir->y &&
			V2.y >= pRayDir->y)
		{
			continue;
		}

		// 法線ベクトルを取得
		D3DXVECTOR3 N;
		D3DXVec3Normalize(&N, D3DXVec3Cross(&N, &V1, &V2));
		
		// 分母を算出
		FLOAT deno = D3DXVec3Dot(&N, &W);
		if (deno >= 0.0f)
		{
			continue;	// 平行(==0)か裏から表(>0)
		}

		// 内外判定
		D3DXVECTOR3 N1;
		D3DXVec3Cross(&N1, &V1, &W);
		if (D3DXVec3Dot(&N1, &(P0 - P1)) < 0.0f)
		{
			continue;
		}

		D3DXVec3Cross(&N1, &V2, &W);

		if (D3DXVec3Dot(&N1, &(P0 - P2)) < 0.0f)
		{
			continue;
		}

		D3DXVECTOR3 V3(P1 - P3);
		D3DXVec3Cross(&N1, &V3, &W);

		if (D3DXVec3Dot(&N1, &(P0 - P3)) < 0.0f)
		{
			continue;
		}

		// 媒介変数算出
		float T = D3DXVec3Dot(&N, &(P1 - P0)) / deno;

		length = T;

		// 交点を算出
		D3DXVECTOR3 X = P0 + T * W;

		if (pCross)
		{
			*pCross = X;
		}

		// 法線を返す
		if (pNormal)
		{
			*pNormal = N;
		}
		// 見つかったので三角形の番号を返す

		return i / 3;
	}
	// 見つからなかったので負の値を返す
	return -1;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const Transform* Collision::getTransform(std::string keyName,INT index)
{
	if (index < 0)
	{
		throw std::invalid_argument("不正な引数です");

		return nullptr;
	}

	return *std::next(collisionMapes[keyName].begin(), index);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const CameraTransform* Collision::getCameraTransform(std::string keyName, INT index)
{
	if (index < 0)
	{
		throw std::invalid_argument("不正な引数です");
	}
	
	return *std::next(cameraTransforms[keyName].begin(), index);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// レイの判定取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const RayHit* Collision::getRayHitData(std::string keyName,UINT index)
{
	if (index < 0)
	{
		throw std::underflow_error("引数の値がマイナスです");
	}

	return *std::next(rayHitMapes[keyName].begin(), index);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 加速度セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::setVelocity(std::string keyName, UINT index, D3DXVECTOR3 setVelocity)
{
	UINT indexCnt = 0;

	for (auto &collisionMap : collisionMapes[keyName])
	{
		if (indexCnt == index)
		{
			collisionMap->velocity = setVelocity;
		}

		indexCnt++;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// サイズ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const size_t Collision::getSize(std::string keyName)
{
	return collisionMapes[keyName].size();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 交点取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const D3DXVECTOR3 Collision::getCross()
{
	return cross;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ブロック用
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::registerBlock(Pawn &blockPtr)
{
	blockPtres.push_back(&blockPtr);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//　ブロック用あたり判定取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::checkCollisionBlock()
{
	if (GameManager::getGameType() != GameManager::GameType::Playing)
	{
		return;
	}

	size_t blockIndex = 0;

	for (auto &blockPtr : blockPtres)
	{
		if (!blockPtr->getUsedFlg() || 
			blockPtr->getTag() == "star.x")
		{
			continue;
		}

		auto blockItr = std::next(collisionMapes[blockPtr->getTag()].begin(), blockIndex);

		if (isHitAABB(*collisionMapes["Player"].front(), *blockPtr))
		{
			(*blockItr)->isHitAABB = true;
		}
		else
		{
			(*blockItr)->isHitAABB = false;
		}

		blockIndex++;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// AABB判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::checkCollision(std::string keyNameA,std::string keyNameB)
{
	for (auto &collisionMap : collisionMapes[keyNameB])
	{
		if (!collisionMap->isUsed)
		{
			continue;
		}

		if (isHitAABB(*collisionMapes[keyNameA].front(), *collisionMap))
		{
			collisionMap->isHitAABB = true;
		}
		else
		{
			collisionMap->isHitAABB = false;
		}
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 当たっているオブジェクトのインデックス取得
// 現在は複数の判定には未対応
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const INT Collision::getHitIndex(std::string keyName)
{
	hitIndex = 0;

	for (auto & collisionMap : collisionMapes[keyName])
	{
		if (!collisionMap->isUsed)
		{
			continue;
		}

		if (collisionMap->isHitAABB)
		{
			return hitIndex;
		}
		else
		{
			hitIndex++;
		}
	}

	return -1;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::initilize(std::string keyName,size_t index)
{
	const auto itr = std::next(rayHitMapes[keyName].begin(), index);
	(*itr)->cross = D3DXVECTOR3(0.0f, 0.0f, 5.0f);
	(*itr)->isHit = false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::initilize()
{
	for (auto &collisionMap : collisionMapes)
	{
		for (auto & second : collisionMap.second)
		{
			second->isHitAABB = false;
		}
	}
}