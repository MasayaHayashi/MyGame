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
class TransformData final
{
public :
	D3DXVECTOR3				posData;	// 座標
	D3DXVECTOR3				rotDegData;
	D3DXVECTOR3				scaleData;	// 拡大
	//GameObjType				objType;	// どのオブジェクトか
	UINT					idNumber;	// 識別番号
	bool					isUsed;		// 使用されているか
};

class Transform final
{
public :
	Transform();
	~Transform();

	void create();
	const TransformData* getData() const;
	void setData(TransformData setData);
private:
	std::list< std::unique_ptr<TransformData> > transformList;
};

#endif