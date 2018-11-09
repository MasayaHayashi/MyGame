//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Skydome.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Skydome.h"
#include "../Camera/camera.h"
#include "../DirectX3D/DirectX3D.h"
#include <string>

// ===== 定数・マクロ定義 =====
#define FILENAME	"data/MODEL/Skydome151009x.x"
#define TEX_FILENAME "data/TEXTURE/ocean.jpg"
#define SCALE (7.0f)

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Skydome::Skydome()
{
	strcpy_s(fileName, FILENAME);
	strcpy_s(texFileName, TEX_FILENAME);
	scale		 = D3DXVECTOR3(SCALE, SCALE, SCALE);
	pos			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	textureSize  = D3DXVECTOR3(10.0f, 10.0f, 10.0f);
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
	makeModel();
	CreateTexture();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スカイドーム後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Skydome::finalize()
{
	// モデル解放
	destroyModel();

	destroyTexture();

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スカイドーム更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Skydome::updateObject()
{
	//C_CAMERA *pCamera = C_CAMERA_MANAGER::getUsedCamera(0);

	//pos = pCamera->getPosCameraP();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スカイドーム描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Skydome::drawObject()
{
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxScale;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ライティングしない
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zバッファ更新を無効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&worldMtx);

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxScale);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &worldMtx);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)uNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pD3DTexture);

		// 描画
		pD3DXMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	// 元に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT Skydome::CreateTexture()
{
	// 例外処理
	if (!texFileName)
		return E_FAIL;

	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, texFileName, &pD3DTexture);

	return S_OK;
}