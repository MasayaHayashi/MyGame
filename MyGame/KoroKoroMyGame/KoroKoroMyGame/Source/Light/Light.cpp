//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Light.cpp
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Light.h"
#include "../Camera/camera.h"
#include "../DirectX3D/DirectX3D.h"
#include "../Camera/camera.h"

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ライト初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT Light::initialize()
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	for (auto &light : lightPtrArray)
	{
		light.reset(NEW D3DLIGHT9);
	}

	// D3DLIGHT9構造体を0でクリアする
	//ZeroMemory(&pLight[0], sizeof(D3DLIGHT9));

	// ライトのID設定
	LightID = 0;

	// ライトのタイプの設定
	for (auto &light : lightPtrArray)
	{
		light->Type = D3DLIGHT_DIRECTIONAL;

		// 拡散光
		light->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 環境光
		light->Ambient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

		// ライトの方向の設定
		vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.80f);
		D3DXVec3Normalize((D3DXVECTOR3*)&light->Direction, &vecDir);

		// ライトをレンダリングパイプラインに設定
		devicePtr->SetLight(0, light.get());

		// ライトの設定
		devicePtr->LightEnable(0, true);
	}

	/*
	pLight[0]->Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	pLight[0]->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 環境光
	pLight[0]->Ambient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&pLight[0]->Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	devicePtr->SetLight(0, pLight[0]);

	// ライトの設定
	devicePtr->LightEnable(0, TRUE);
	
	
	// D3DLIGHT9構造体を0でクリアする
	//ZeroMemory(&pLight[1], sizeof(D3DLIGHT9));

	// ライトのタイプの設定
	pLight[1]->Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	pLight[1]->Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	// 環境光
	pLight[1]->Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(-0.20f, -1.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&pLight[1]->Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	devicePtr->SetLight(1, pLight[1]);

	// ライトの設定
	devicePtr->LightEnable(1, TRUE);

	// D3DLIGHT9構造体を0でクリアする
	//ZeroMemory(pLight[2], sizeof(D3DLIGHT9));

	// ライトのタイプの設定
	pLight[2]->Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	pLight[2]->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 環境光
	pLight[2]->Ambient = D3DXCOLOR(-1.0f, 1.0f, 1.0f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.80f, 0.10f, 0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&pLight[2]->Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	devicePtr->SetLight(2, pLight[2]);

	// ライトの設定
	devicePtr->LightEnable(2, TRUE);

	// ライティングモード
	devicePtr->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	*/
	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ライトタイプ用セッター
// LightType	: 平行光源、拡散光など
// Diffusecolor	: 
// Ambientcolot : 
// vecDir		: 
// LightID		: ライトの番号
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Light::SetLight(D3DLIGHTTYPE LightType, D3DXCOLOR Diffusecolor, D3DXCOLOR Ambientcolor, D3DXVECTOR3 vecDir,BYTE LightID)
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();
	/*
	// ライトのタイプの設定
	Light.Type = LightType;

	// 拡散光の質量設定
	Light.Diffuse = Diffusecolor;

	// 環境光の質量設定
	Light.Ambient = Ambientcolor;

	// ライトの方向ベクトルの設定
	vecDir = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light.Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	devicePtr->SetLight(LightID, &Light);

	// ライトの設定
	devicePtr->LightEnable(LightID, TRUE);
	*/
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ライト後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Light::finalize()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ライト更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Light::UpdateLight(Camera *pCamera)
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	D3DXVECTOR3 PosCamera = pCamera->getPos();
	D3DXVECTOR3 LotCamera = pCamera->getLook();
	D3DXVECTOR3 Vec		  = LotCamera - PosCamera;
	D3DXVec3Normalize(&PosCamera, &PosCamera);

	vecDir = Vec;
	// ライトの方向の設定
//	 = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	pLight[0]->Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	pLight[0]->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 環境光
	pLight[0]->Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライトの方向の設定
	D3DXVec3Normalize((D3DXVECTOR3 *)&pLight[0]->Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	devicePtr->SetLight(0, pLight[0]);

	// ライトの設定
	devicePtr->LightEnable(0, TRUE);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ライトの番号取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
BYTE* Light::GetLightID()
{
	return &LightID;
}