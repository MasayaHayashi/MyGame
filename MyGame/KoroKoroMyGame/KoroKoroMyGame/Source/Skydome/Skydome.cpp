//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Skydome.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Skydome.h"
#include "../Camera/camera.h"
#include "../DirectX3D/DirectX3D.h"
#include <string>

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Skydome::Skydome()
{
	strcpy_s(fileName, ModelFilePass.c_str());
	strcpy_s(texFileName, TextureFilePass.c_str());

	myTransform.scale = (D3DXVECTOR3(Scale,Scale,Scale));
	textureSize  = D3DXVECTOR3(10.0f, 10.0f, 10.0f);

	isUsed = true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Skydome::~Skydome()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スカイドーム初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Skydome::initialize()
{
	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スカイドーム後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Skydome::finalize()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スカイドーム更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Skydome::update()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スカイドーム描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Skydome::draw()
{
	Pawn::draw();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT Skydome::createTexture()
{
	// 例外処理
	if (!texFileName)
	{
		return E_FAIL;
	}

	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(devicePtr, texFileName, &pD3DTexture);

	return S_OK;
}