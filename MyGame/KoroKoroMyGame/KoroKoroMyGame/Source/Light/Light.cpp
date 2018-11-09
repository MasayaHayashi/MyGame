//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Light.cpp
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Light.h"
#include "../camera/camera.h"
#include "../DirectX3D/DirectX3D.h"
#include "../camera/camera.h"

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ライト初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT Light::InitLight()
{
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	for (auto &light : lightPtrArray)
	{
		light.reset(new D3DLIGHT9);
	}

	// D3DLIGHT9構造体を0でクリアする
	//ZeroMemory(&pLight[0], sizeof(D3DLIGHT9));

	// ライトのID設定
	LightID = 0;

	// ライトのタイプの設定

	pLight[0]->Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	pLight[0]->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 環境光
	pLight[0]->Ambient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&pLight[0]->Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	pDevice->SetLight(0, pLight[0]);

	// ライトの設定
	pDevice->LightEnable(0, TRUE);
	
	
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
	pDevice->SetLight(1, pLight[1]);

	// ライトの設定
	pDevice->LightEnable(1, TRUE);

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
	pDevice->SetLight(2, pLight[2]);

	// ライトの設定
	pDevice->LightEnable(2, TRUE);

	// ライティングモード
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	
	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ライトタイプ用セッター
// LightType	: 平行光源、拡散光など
// DiffuseColor	: 
// Ambientcolot : 
// vecDir		: 
// LightID		: ライトの番号
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Light::SetLight(D3DLIGHTTYPE LightType, D3DXCOLOR DiffuseColor, D3DXCOLOR AmbientColor, D3DXVECTOR3 vecDir,BYTE LightID)
{
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();
	/*
	// ライトのタイプの設定
	Light.Type = LightType;

	// 拡散光の質量設定
	Light.Diffuse = DiffuseColor;

	// 環境光の質量設定
	Light.Ambient = AmbientColor;

	// ライトの方向ベクトルの設定
	vecDir = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light.Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	pDevice->SetLight(LightID, &Light);

	// ライトの設定
	pDevice->LightEnable(LightID, TRUE);
	*/
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ライト後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Light::UninitLight()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ライト更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Light::UpdateLight(camera *pcamera)
{
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	D3DXVECTOR3 Poscamera = pcamera->getPos();
	D3DXVECTOR3 Lotcamera = pcamera->getLook();
	D3DXVECTOR3 Vec		  = Lotcamera - Poscamera;
	D3DXVec3Normalize(&Poscamera, &Poscamera);

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
	pDevice->SetLight(0, pLight[0]);

	// ライトの設定
	pDevice->LightEnable(0, TRUE);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ライトの番号取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
BYTE* Light::GetLightID()
{
	return &LightID;
}