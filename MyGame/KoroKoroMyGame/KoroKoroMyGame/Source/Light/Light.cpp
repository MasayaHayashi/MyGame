//������������������������������������������������������
// Light.cpp
// Author : MasayaHayashi
//������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Light.h"
#include "../Camera/camera.h"
#include "../DirectX3D/DirectX3D.h"
#include "../Camera/camera.h"

//��������������������������������������
// ���C�g������
//��������������������������������������
HRESULT Light::initialize()
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	for (auto &light : lightPtrArray)
	{
		light.reset(NEW D3DLIGHT9);
	}

	// D3DLIGHT9�\���̂�0�ŃN���A����
	//ZeroMemory(&pLight[0], sizeof(D3DLIGHT9));

	// ���C�g��ID�ݒ�
	LightID = 0;

	// ���C�g�̃^�C�v�̐ݒ�
	for (auto &light : lightPtrArray)
	{
		light->Type = D3DLIGHT_DIRECTIONAL;

		// �g�U��
		light->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ����
		light->Ambient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

		// ���C�g�̕����̐ݒ�
		vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.80f);
		D3DXVec3Normalize((D3DXVECTOR3*)&light->Direction, &vecDir);

		// ���C�g�������_�����O�p�C�v���C���ɐݒ�
		devicePtr->SetLight(0, light.get());

		// ���C�g�̐ݒ�
		devicePtr->LightEnable(0, true);
	}

	/*
	pLight[0]->Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	pLight[0]->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ����
	pLight[0]->Ambient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&pLight[0]->Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	devicePtr->SetLight(0, pLight[0]);

	// ���C�g�̐ݒ�
	devicePtr->LightEnable(0, TRUE);
	
	
	// D3DLIGHT9�\���̂�0�ŃN���A����
	//ZeroMemory(&pLight[1], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	pLight[1]->Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	pLight[1]->Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	// ����
	pLight[1]->Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(-0.20f, -1.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&pLight[1]->Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	devicePtr->SetLight(1, pLight[1]);

	// ���C�g�̐ݒ�
	devicePtr->LightEnable(1, TRUE);

	// D3DLIGHT9�\���̂�0�ŃN���A����
	//ZeroMemory(pLight[2], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	pLight[2]->Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	pLight[2]->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ����
	pLight[2]->Ambient = D3DXCOLOR(-1.0f, 1.0f, 1.0f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.80f, 0.10f, 0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&pLight[2]->Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	devicePtr->SetLight(2, pLight[2]);

	// ���C�g�̐ݒ�
	devicePtr->LightEnable(2, TRUE);

	// ���C�e�B���O���[�h
	devicePtr->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	*/
	return S_OK;
}

//��������������������������������������
// ���C�g�^�C�v�p�Z�b�^�[
// LightType	: ���s�����A�g�U���Ȃ�
// Diffusecolor	: 
// Ambientcolot : 
// vecDir		: 
// LightID		: ���C�g�̔ԍ�
//��������������������������������������
void Light::SetLight(D3DLIGHTTYPE LightType, D3DXCOLOR Diffusecolor, D3DXCOLOR Ambientcolor, D3DXVECTOR3 vecDir,BYTE LightID)
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();
	/*
	// ���C�g�̃^�C�v�̐ݒ�
	Light.Type = LightType;

	// �g�U���̎��ʐݒ�
	Light.Diffuse = Diffusecolor;

	// �����̎��ʐݒ�
	Light.Ambient = Ambientcolor;

	// ���C�g�̕����x�N�g���̐ݒ�
	vecDir = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light.Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	devicePtr->SetLight(LightID, &Light);

	// ���C�g�̐ݒ�
	devicePtr->LightEnable(LightID, TRUE);
	*/
}

//��������������������������������������
// ���C�g�㏈��
//��������������������������������������
void Light::finalize()
{

}

//��������������������������������������
// ���C�g�X�V
//��������������������������������������
void Light::UpdateLight(Camera *pCamera)
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	D3DXVECTOR3 PosCamera = pCamera->getPos();
	D3DXVECTOR3 LotCamera = pCamera->getLook();
	D3DXVECTOR3 Vec		  = LotCamera - PosCamera;
	D3DXVec3Normalize(&PosCamera, &PosCamera);

	vecDir = Vec;
	// ���C�g�̕����̐ݒ�
//	 = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	pLight[0]->Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	pLight[0]->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ����
	pLight[0]->Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���C�g�̕����̐ݒ�
	D3DXVec3Normalize((D3DXVECTOR3 *)&pLight[0]->Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	devicePtr->SetLight(0, pLight[0]);

	// ���C�g�̐ݒ�
	devicePtr->LightEnable(0, TRUE);
}

//��������������������������������������
// ���C�g�̔ԍ��擾
//��������������������������������������
BYTE* Light::GetLightID()
{
	return &LightID;
}