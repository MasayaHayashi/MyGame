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
const TransformData* Transform::getData() const
{
	auto itr = transformList.begin();

	return nullptr;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝
// データセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝
void Transform::setData(TransformData setData)
{
	transformList.push_back(new TransformData);
}