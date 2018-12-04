//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Transform.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef TRANSFORM_H
#define TRANSFORM_H

// ===== インクルード部 =====
#include <list>
#include <memory>
#include "d3dx9.h"
#include "../GameObjectBase/GameObjectBase.h"
#include "../Mesh/Mesh.h"

// ===== 構造体定義 =====
typedef struct 
{
	D3DXVECTOR3				posData;
	D3DXVECTOR3				velocityData;
	D3DXVECTOR3				rotDegData;
	D3DXVECTOR3				scaleData;
	D3DXVECTOR3				collisionBox;
	D3DXMATRIX				worldMatrix;

	D3DXVECTOR3				cross;
	D3DXVECTOR3				normal;
	D3DXVECTOR3				length;
	D3DXVECTOR3				destvec;

	DWORD					numIndx;
	MESH_VTX*				vertexPtr;
	WORD*					indexPtr;
	GameObjType				objType;
	UINT					idNumber;	// 識別番号
	bool					isHit;		
	bool					isUsed;		// 使用されているか
} TransformData;

class Transform final
{
public :
	Transform();
	~Transform();

	void create();
	TransformData* getData(INT index) const;
	void setData(TransformData *setData);
	void setHit(INT index,bool setFlg);
	void setRayHit(INT index,D3DXVECTOR3 cross, D3DXVECTOR3 normal, D3DXVECTOR3 length, D3DXVECTOR3 destvec);
	void clear();
private:
	std::list< TransformData* > transformList;
};

#endif