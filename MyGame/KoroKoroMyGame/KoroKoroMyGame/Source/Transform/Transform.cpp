//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Transform.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Transform.h"
#include "../DirectX3D/DirectX3D.h"
#include <memory>

//＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝
Transform::Transform()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝
Transform::~Transform()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝
// 生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝
void Transform::create()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝
// データ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝
const TransformData* Transform::getData(INT index) const
{
	if (transformList.empty())
	{
		throw std::runtime_error("transform is empty");
	}

	for (auto transform : transformList)
	{
		if (transform->idNumber == index)
		{
			return transform;
		}
	}

	throw std::runtime_error("index do not match");
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝
// データセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝
void Transform::setData(TransformData *setData)
{
	transformList.push_back(setData);

	setData->idNumber = transformList.size() - 1;	// IDナンパ―は0スタートで統一
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝
// フラグセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝
void Transform::setHit(INT index,bool setFlg)
{
	for (auto transform : transformList)
	{
		if (transform->idNumber == index)
		{
			transform->isHit = setFlg;
		}
	}
}