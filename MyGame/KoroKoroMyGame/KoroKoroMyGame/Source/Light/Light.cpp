//
// Light.cpp
// Author : MasayaHayashi
//

// ===== CN[h =====
#include "Light.h"
#include "../camera/camera.h"
#include "../DirectX3D/DirectX3D.h"
#include "../camera/camera.h"

//
// Cgú»
//
HRESULT Light::InitLight()
{
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	for (auto &light : lightPtrArray)
	{
		light.reset(new D3DLIGHT9);
	}

	// D3DLIGHT9\¢Ìð0ÅNA·é
	//ZeroMemory(&pLight[0], sizeof(D3DLIGHT9));

	// CgÌIDÝè
	LightID = 0;

	// CgÌ^CvÌÝè

	pLight[0]->Type = D3DLIGHT_DIRECTIONAL;

	// gUõ
	pLight[0]->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// Â«õ
	pLight[0]->Ambient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

	// CgÌûüÌÝè
	vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&pLight[0]->Direction, &vecDir);

	// Cgð_OpCvCÉÝè
	pDevice->SetLight(0, pLight[0]);

	// CgÌÝè
	pDevice->LightEnable(0, TRUE);
	
	
	// D3DLIGHT9\¢Ìð0ÅNA·é
	//ZeroMemory(&pLight[1], sizeof(D3DLIGHT9));

	// CgÌ^CvÌÝè
	pLight[1]->Type = D3DLIGHT_DIRECTIONAL;

	// gUõ
	pLight[1]->Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	// Â«õ
	pLight[1]->Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// CgÌûüÌÝè
	vecDir = D3DXVECTOR3(-0.20f, -1.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&pLight[1]->Direction, &vecDir);

	// Cgð_OpCvCÉÝè
	pDevice->SetLight(1, pLight[1]);

	// CgÌÝè
	pDevice->LightEnable(1, TRUE);

	// D3DLIGHT9\¢Ìð0ÅNA·é
	//ZeroMemory(pLight[2], sizeof(D3DLIGHT9));

	// CgÌ^CvÌÝè
	pLight[2]->Type = D3DLIGHT_DIRECTIONAL;

	// gUõ
	pLight[2]->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// Â«õ
	pLight[2]->Ambient = D3DXCOLOR(-1.0f, 1.0f, 1.0f, 1.0f);

	// CgÌûüÌÝè
	vecDir = D3DXVECTOR3(0.80f, 0.10f, 0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&pLight[2]->Direction, &vecDir);

	// Cgð_OpCvCÉÝè
	pDevice->SetLight(2, pLight[2]);

	// CgÌÝè
	pDevice->LightEnable(2, TRUE);

	// CeBO[h
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	
	return S_OK;
}

//
// Cg^CvpZb^[
// LightType	: ½sõ¹AgUõÈÇ
// DiffuseColor	: 
// Ambientcolot : 
// vecDir		: 
// LightID		: CgÌÔ
//
void Light::SetLight(D3DLIGHTTYPE LightType, D3DXCOLOR DiffuseColor, D3DXCOLOR AmbientColor, D3DXVECTOR3 vecDir,BYTE LightID)
{
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();
	/*
	// CgÌ^CvÌÝè
	Light.Type = LightType;

	// gUõÌ¿ÊÝè
	Light.Diffuse = DiffuseColor;

	// Â«õÌ¿ÊÝè
	Light.Ambient = AmbientColor;

	// CgÌûüxNgÌÝè
	vecDir = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light.Direction, &vecDir);

	// Cgð_OpCvCÉÝè
	pDevice->SetLight(LightID, &Light);

	// CgÌÝè
	pDevice->LightEnable(LightID, TRUE);
	*/
}

//
// Cgã
//
void Light::UninitLight()
{

}

//
// CgXV
//
void Light::UpdateLight(camera *pcamera)
{
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	D3DXVECTOR3 Poscamera = pcamera->getPos();
	D3DXVECTOR3 Lotcamera = pcamera->getLook();
	D3DXVECTOR3 Vec		  = Lotcamera - Poscamera;
	D3DXVec3Normalize(&Poscamera, &Poscamera);

	vecDir = Vec;
	// CgÌûüÌÝè
//	 = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	pLight[0]->Type = D3DLIGHT_DIRECTIONAL;

	// gUõ
	pLight[0]->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// Â«õ
	pLight[0]->Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// CgÌûüÌÝè
	D3DXVec3Normalize((D3DXVECTOR3 *)&pLight[0]->Direction, &vecDir);

	// Cgð_OpCvCÉÝè
	pDevice->SetLight(0, pLight[0]);

	// CgÌÝè
	pDevice->LightEnable(0, TRUE);
}

//
// CgÌÔæ¾
//
BYTE* Light::GetLightID()
{
	return &LightID;
}