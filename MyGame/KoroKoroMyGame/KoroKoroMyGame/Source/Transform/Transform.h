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

// ===== 構造体定義 =====
typedef struct 
{
	D3DXVECTOR3				posData;
	D3DXVECTOR3				velocityData;
	D3DXVECTOR3				rotDegData;
	D3DXVECTOR3				scaleData;
	D3DXVECTOR3				collisionBox;
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
	const TransformData* getData(INT index) const;
	void setData(TransformData *setData);
	void setHit(INT index,bool setFlg);
private:
	std::list< TransformData* > transformList;
};

#endif