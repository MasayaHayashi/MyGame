//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// MyVector3.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef MY_VECTOR3
#define MY_VECTOR3

// ===== インクルード部 =====
#include "d3dx9.h"

// ===== クラス定義 =====
class MyVector3 final : D3DXVECTOR3
{
public:

	MyVector3();
	~MyVector3();
	
	static bool CalcHermiteCurve(D3DXVECTOR3 *pOut, const D3DXVECTOR3& Pos1, const D3DXVECTOR3& Pos2, const D3DXVECTOR3& Vec1, const D3DXVECTOR3& Vec2, const FLOAT& fMax, const FLOAT& fCurrent);	// エルミート曲線
	static void CalcBezierCurve(D3DXVECTOR3* pOut, const D3DXVECTOR3& Pos1, const D3DXVECTOR3& Pos2, const D3DXVECTOR3& Pos3, const D3DXVECTOR3& Pos4, const FLOAT& fMax, const FLOAT& fCurrent);
	
	// 3×3 2D用
	static D3DXMATRIX* Mtx2DIdentity(D3DXMATRIX* pOut);
	static D3DXMATRIX* Mtx2DScaling(D3DXMATRIX* pOut, FLOAT Sx, FLOAT Sy);
	static D3DXMATRIX* Mtx2DTranslation(D3DXMATRIX* pOut, FLOAT x, FLOAT y);
	static D3DXMATRIX* Mtx2DRotation(D3DXMATRIX* pOut, FLOAT Angle);

	static const FLOAT getLength(const D3DXVECTOR3& setVector);

	static void Lerp(D3DXVECTOR3* pOut, const D3DXVECTOR3& fStart, const D3DXVECTOR3& fEnd, const FLOAT& fMin, const FLOAT& fMax, const FLOAT& fCurrent);																// 線形補間
	static void Slerp(D3DXQUATERNION *out, const D3DXQUATERNION &start, const D3DXQUATERNION &end, const FLOAT& fMax, const FLOAT& fCurrent);	// 球面線形補間(クオータニオン)


	static FLOAT		Vec3Dot(D3DXVECTOR3 VecA, D3DXVECTOR3 VecB);	// 内積
	static D3DXVECTOR3 Vec3Cross(D3DXVECTOR3 VecA, D3DXVECTOR3 VecB);	// 外積

//	bool CalcAngle3(const CNhVector2d& vectorA, const CNhVector2d& vectorB, double& dAngle_rad);
	static FLOAT CalcAngle(D3DXVECTOR3 vectorA, D3DXVECTOR3 vectorB);
	static FLOAT CalcAngleDegree(D3DXVECTOR3 vectorA, D3DXVECTOR3 vectorB);
	static FLOAT Product2Vec(D3DXVECTOR3 VecA, D3DXVECTOR3 VecB);


};

#endif