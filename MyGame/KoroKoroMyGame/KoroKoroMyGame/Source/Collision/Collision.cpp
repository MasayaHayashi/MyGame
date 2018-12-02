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

// ===== 静的メンバ =====
std::unordered_map<std::string, Transform> Collision::collisionMapes;

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Collision::Collision()
{
	
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
 	FLOAT test = collisionMapes["Player"].getData(0)->posData.x;
	DirectX3D::printDebug("posdata %f\n", test);

	if (isHitAABB(*collisionMapes["Player"].getData(0), *collisionMapes["Player"].getData(1)))
	{
		collisionMapes["Player"].setHit(0, true);
		collisionMapes["Player"].setHit(1, true);
		DirectX3D::printDebug("あたる");
	}
	else
	{
		collisionMapes["Player"].setHit(0, false);
		collisionMapes["Player"].setHit(1, false);
		DirectX3D::printDebug("いません");
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 登録
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::registerList(TransformData *setTransformData,std::string keyName)
{
	collisionMapes[keyName].setData(setTransformData);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 地面とプレイヤーの衝突処理更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
UINT Collision::CheckCollisionField(Player *pPlayer, Pawn *pPawnB, Pawn *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, D3DXVECTOR3 &fLength,D3DXVECTOR3 DestVec)
{
	INT	nIndx;

	// 線分と三角形の判定
	nIndx = IsHitRayToMesh(pField, pPlayer, &pPlayer->getPosition(), &(pPlayer->getPosition() + DestVec), true, &Cross, &Normal,&fLength);

	if (nIndx >= 0)
	{
		return RAY_TRUE;
	}
	else
	{
		return RAY_FALSE;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 壁とプレイヤーの衝突処理更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
UINT Collision::CheckCollisionWall(Player *pPlayer, Pawn *pPawnB, Pawn *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, D3DXVECTOR3 &fLength, D3DXVECTOR3 DestVec)
{
	INT	nIndx;

	// 線分と三角形の判定
	nIndx = IsHitRayToMesh(pField, pPlayer, &(pPlayer->getPosition() + D3DXVECTOR3(0.0f,2.0f,0.0f)), &(pPlayer->getPosition() + DestVec), true, &Cross, &Normal, &fLength);

	if (nIndx >= 0)
	{
		return RAY_TRUE;
	}
	else
	{
		return RAY_FALSE;
	}
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 衝突処理(削除用) (オブジェクトとオブジェクト)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::CheckCollisionBlock(Pawn *pSelectBlock, Pawn *pGameObj)
{
	/*
	if (IsHitAABB(pSelectBlock, pGameObj))
	{
		pGameObj->setPosition( D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pGameObj->setUsedFlg(false);
	}
	*/
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

/*
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 説　明 : 線分（有向）と法線付き三角形の当たりチェック
// 引　数 : _segment		光線の線分（start→endが正方向）
//			_tri		法線付き三角形
//			_inter		交点（省略可）
// 戻り値 : 交差しているか否か
// メ　モ : 裏面の当たりはとらない
//--------------------------------------------------------------------------------------------
bool CheckSegment2Triangle(const SEGMENT& _segment, const TRIANGLE& _triangle, D3DXVECTOR3 *_inter)
{
	const float epsilon = -1.0e-5f;	// 誤差吸収用の微小な値
	D3DXVECTOR3 	LayV;		// 線分の終点→始点
	D3DXVECTOR3 	tls;		// 三角形の頂点0→線分の始点
	D3DXVECTOR3 	tle;		// 三角形の頂点0→線分の終点
	float 	distl0;
	float 	distl1;
	float 	dp;
	float 	denom;
	float 	t;
	D3DXVECTOR3	s;			// 直線と平面との交点
	D3DXVECTOR3 	st0;		// 交点→三角形の頂点0
	D3DXVECTOR3 	st1;		// 交点→三角形の頂点1
	D3DXVECTOR3 	st2;		// 交点→三角形の頂点2
	D3DXVECTOR3 	t01;		// 三角形の頂点0→頂点1
	D3DXVECTOR3 	t12;		// 三角形の頂点1→頂点2
	D3DXVECTOR3 	t20;		// 三角形の頂点2→頂点0
	D3DXVECTOR3	m;

	// 線分の始点が三角系の裏側にあれば、当たらない
	tls = _segment.start - _triangle.P0;

	D3DXVECTOR3 test;
	D3DXVec3Normalize(&test,&tls);

	distl0 = D3DXVec3Dot(&tls, &test);

//	distl0 = tls.Dot(_triangle.Normal);	// 線分の始点と平面の距離
	if (distl0 <= epsilon) return false;

	// 線分の終点が三角系の表側にあれば、当たらない
	tle = _segment.end - _triangle.P0;

	D3DXVec3Normalize(&test, &tle);

	distl1 = D3DXVec3Dot(&test, _triangle);

//	distl1 = tle.Dot(_triangle.Normal);	// 線分の終点と平面の距離
	if (distl1 >= -epsilon) return false;

	// 直線と平面との交点sを取る
	denom = distl0 - distl1;
	t = distl0 / denom;
	LayV = _segment.end - _segment.start;	// 線分の方向ベクトルを取得
	s = t * LayV + _segment.start;

	// 交点が三角形の内側にあるかどうかを調べる。
	// 三角形の内側にある場合、交点から各頂点へのベクトルと各辺ベクトルの外積（三組）が、全て法線と同じ方向を向く
	// 一つでも方向が一致しなければ、当たらない。
	st0 = _triangle.P0 - s;
	t01 = _triangle.P1 - _triangle.P0;
	
	D3DXVec3Cross(&m, &st0, &t01);
//	m = st0.Cross(t01);

	dp = D3DXVec3Dot(&m, &_triangle);

//	dp = m.Dot(_triangle.Normal);
	if (dp <= epsilon) return false;

	st1 = _triangle.P1 - s;
	t12 = _triangle.P2 - _triangle.P1;

	D3DXVec3Cross(&m, &st1, &t12);
	//m = st1.Cross(t12);

	dp = D3DXVec3Dot(&m,&_triangle);
	//dp = m.Dot(_triangle.Normal);
	if (dp <= epsilon) return false;

	st2 = _triangle.P2 - s;
	t20 = _triangle.P0 - _triangle.P2;

	D3DXVec3Cross(&m, &st2, &t20);
	//m = st2.Cross(t20);
	
	dp = D3DXVec3Dot(&m, &_triangle);
//	dp = m.Dot(_triangle.Normal);
	if (dp <= epsilon) return false;

	if (_inter)
	{
		*_inter = s;	// 交点をコピー
	}

	return true;
}
*/

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
INT Collision::IsHitRayToMesh(Pawn *pPawnA, Pawn *pPawnB, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, bool bSegment, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal,LPD3DXVECTOR3 Length)
{
		// ワールドマトリックスの逆マトリックスを生成
		D3DXMATRIX mInvWorld;

		D3DXMATRIX test = *pPawnA->getWorldMtx();

		D3DXMatrixInverse(&mInvWorld, nullptr, pPawnA->getWorldMtx());
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
		INT nIndex = Intersect(pPawnA,&vRayPos, &vRayDir, bSegment, pCross, pNormal,Length);

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


	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 線分の判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
INT Collision::Intersect(Pawn *pField,LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, bool bSegment, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal ,LPD3DXVECTOR3 pFLength)
{
	if (!pRayPos || !pRayDir)
		return -1;

	// レイ取得
	D3DXVECTOR3& P0 = *pRayPos;
	D3DXVECTOR3 W = *pRayDir;
	
	if (bSegment)
		W -= P0;

	DWORD dwNumIndx = pField->getIndxNum();			// 三角形の数取得	
	MESH_VTX *pVtx  = pField->getVtxAcess();		// 頂点情報取得
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
			continue;

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
	
		pFLength->x = T;

		// 交点を算出
		D3DXVECTOR3 X = P0 + T * W;
		if (pCross)
			*pCross = X;

		// 法線を返す
		if (pNormal)
			*pNormal = N;
		// 見つかったので三角形の番号を返す

		return i / 3;
	}
	// 見つからなかったので負の値を返す
	return -1;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 線分の判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Collision::IntersectA(Pawn* pField,LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, LPD3DXMATRIX pWorld)
{
	bool ans = false;

	D3DXVECTOR3 W;
	D3DXVECTOR3 H;
	D3DXMATRIX matInv;
	if (pWorld) 
	{
		D3DXMatrixInverse(&matInv, nullptr, pWorld);
		D3DXVec3TransformCoord(&W, pRayPos, &matInv);
		D3DXVec3TransformNormal(&H, pRayDir, &matInv);
	}
	else 
	{
		W = *pRayPos;
		H = *pRayDir;
	}

	LPDIRECT3DVERTEXBUFFER9	lpVertexBuffer;	// 頂点バッファ
	pField->getMesh()->GetVertexBuffer(&lpVertexBuffer);	// 頂点バッファオブジェクトへのポインタをゲット
	LPDIRECT3DINDEXBUFFER9	lpIndexBuffer;	// インデックスバッファ
	pField->getMesh()->GetIndexBuffer(&lpIndexBuffer);		// インデックスバッファオブジェクトへのポインタをゲット
	DWORD dwIdx = pField->getMesh()->GetNumFaces() * 3;		// 面数からインデックス数をゲット

	MESH_VTX* pVtx;
	WORD* pIdx;
	lpVertexBuffer->Lock(0, 0, (void**)&pVtx, 0);	// 頂点バッファをロック
	lpIndexBuffer->Lock(0, 0, (void**)&pIdx, 0);	// インデックスバッファをロック

	D3DXVECTOR3* P[4];
	D3DXVECTOR3 N;
	for (DWORD i = 0; i < dwIdx; ) 
	{
		// 三角形の頂点を抽出
		P[0] = &pVtx[pIdx[i++]].VtxPos;
		P[1] = &pVtx[pIdx[i++]].VtxPos;
		P[2] = &pVtx[pIdx[i++]].VtxPos;
		P[3] = P[0];
		INT j = 0;
		for (; j < 3; j++) {
			// 三角形を囲む平面の法線ベクトルを求める
			D3DXVec3Cross(&N, &H, &(*(P[j + 1]) - *(P[j])));
			// 始点が平面の表なら当たっていない
			FLOAT dot = D3DXVec3Dot(&N, &(W - *(P[j])));

			if (dot > 0.0f) 
			{
				break;
			}
		}
		if (j < 3) 
		{
			continue;
		}

		// 三角形の法線ベクトルを求める
		D3DXVec3Cross(&N, &(*(P[1]) - *(P[0])), &(*(P[2]) - *(P[1])));
		D3DXVec3Normalize(&N, &N);

		// 媒介変数tの分母を求める
		float base = D3DXVec3Dot(&N, &H);
		if (base == 0.0f)
		{
			continue;		// 平面と平行
		}

		// tを求める
		FLOAT t = D3DXVec3Dot(&N, &(*(P[0]) - W)) / base;

	//	PrintDebugProc("aaaaaaa:%f\n", t);

		// 交点がレイの後ろ
		if (t < 0.0f)
		{
			continue;
		}
		// 交点がレイの前
		if (t > 1.0f)
		{
			continue;
		}

		// 交点を求める
		D3DXVECTOR3 X = W + t * H;
		if (pCross) 
		{
			if (pWorld)
			{
				D3DXVec3TransformCoord(pCross, &X, pWorld);
			}
			else 
			{
				*pCross = X;
			}
		}
		if (pNormal)
		{
			if (pWorld) 
			{
				D3DXVec3TransformNormal(pNormal, &N, pWorld);
			}
			else
			{
				*pNormal = N;
			}
		}
		ans = true;
		break;
	}

	lpVertexBuffer->Unlock();		// 頂点バッファをアンロック
	lpIndexBuffer->Unlock();			// インデックスバッファをアンロック
//	SAFE_RELEASE(lpVertexBuffer);	// 頂点バッファオブジェクトを解放
//	SAFE_RELEASE(lpIndexBuffer);		// インデックスバッファオブジェクトを解放

	return ans;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// AABBのあたり判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Collision::isHitAABB(const TransformData transformA, const TransformData transformB) const
{
	// 移動量を足した先を計算する
	D3DXVECTOR3 aBoxPos = transformA.posData + transformA.velocityData;
	D3DXVECTOR3 bBoxPos = transformB.posData + transformB.velocityData;

	D3DXVECTOR3 aBoxSize = transformA.collisionBox;
	D3DXVECTOR3 bBoxSize = transformB.collisionBox;

	if (aBoxPos.x + bBoxSize.x > bBoxPos.x				 &&
		aBoxPos.x			   < bBoxPos.x + bBoxSize.x  &&
		aBoxPos.y + bBoxSize.y > bBoxPos.y				 &&
		aBoxPos.y			   < bBoxPos.y + bBoxSize.y  &&
		aBoxPos.z + bBoxSize.z > bBoxPos.z				 &&
		aBoxPos.z			   < bBoxPos.z + bBoxSize.z )
	{
		return true;
	}
	else
	{
		return false;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤーとアイテム関連用AABB判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Collision::IsHitAABBItem(Player *pPlayer,Pawn *pPawn)
{
	D3DXVECTOR3 BoxASize = pPlayer->getCollisionBox()	* 2;
	D3DXVECTOR3 BoxBSize = pPawn->getCollisionBox()		* 2;

	D3DXVECTOR3 BoxAPos = pPlayer->getOffset();
	D3DXVECTOR3 BoxBPos = pPawn->getColliderPos() + D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	if (BoxAPos.x + BoxASize.x > BoxBPos.x				 &&
		BoxAPos.x			   < BoxBPos.x + BoxBSize.x  &&
		BoxAPos.y + BoxASize.y > BoxBPos.y				 &&
		BoxAPos.y			   < BoxBPos.y + BoxBSize.y  &&
		BoxAPos.z + BoxASize.z > BoxBPos.z				 &&
		BoxAPos.z			   < BoxBPos.z + BoxBSize.z)
	{
		return true;
	}

	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タグに応じて分岐用 (現在A側にプレイヤーを置く仕様)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::SwitchHitType(Pawn *pPawnA, Pawn *pPawnB)
{
	// プレイヤーの処理
	if (pPawnA->getTag() == Pawn::TagType::Player)
	{
		if (pPawnB->getTag() == Pawn::TagType::Enemy)
			pPawnA->setHitFlg(true);
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const TransformData* Collision::getTransformData(std::string keyName,INT index)
{
	return collisionMapes[keyName].getData(index);
}

void Collision::checkPlayerCollision()
{

}