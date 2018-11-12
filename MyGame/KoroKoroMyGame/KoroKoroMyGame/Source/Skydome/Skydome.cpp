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
	scale		 = D3DXVECTOR3(Scale, Scale, Scale);
	pos			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	//C_CAMERA *pCamera = C_CAMERA_MANAGER::getUsedCamera(0);

	//pos = pCamera->getPosCameraP();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スカイドーム描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Skydome::draw()
{
	Pawn::draw();

	/*
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxScale;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ライティングしない
	devicePtr->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zバッファ更新を無効
	devicePtr->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&worldMtx);

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxScale);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxTranslate);

	// ワールドマトリックスの設定
	devicePtr->SetTransform(D3DTS_WORLD, &worldMtx);

	// 現在のマテリアルを取得
	devicePtr->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)materialBufferPtr->GetBufferPointer();

	for (int mat = 0; mat < static_cast<INT>(numMat); mat++)
	{
		// マテリアルの設定
		devicePtr->SetMaterial(&pD3DXMat[mat].MatD3D);

		// テクスチャの設定
		devicePtr->SetTexture(0, pD3DTexture);

		// 描画
		meshPtr->DrawSubset(mat);
	}

	// マテリアルをデフォルトに戻す
	devicePtr->SetMaterial(&matDef);

	// 元に戻す
	devicePtr->SetRenderState(D3DRS_LIGHTING, TRUE);
	devicePtr->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	*/
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT Skydome::createTexture()
{
	// 例外処理
	if (!texFileName)
		return E_FAIL;

	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(devicePtr, texFileName, &pD3DTexture);

	return S_OK;
}