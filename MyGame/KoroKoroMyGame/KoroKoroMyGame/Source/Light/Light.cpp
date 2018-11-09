//������������������������������������������������������
// Light.cpp
// Author : MasayaHayashi
//������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Light.h"
#include "../camera/camera.h"
#include "../DirectX3D/DirectX3D.h"
#include "../camera/camera.h"

//��������������������������������������
// ���C�g������
//��������������������������������������
HRESULT Light::InitLight()
{
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	for (auto &light : lightPtrArray)
	{
		light.reset(new D3DLIGHT9);
	}

	// D3DLIGHT9�\���̂�0�ŃN���A����
	//ZeroMemory(&pLight[0], sizeof(D3DLIGHT9));

	// ���C�g��ID�ݒ�
	LightID = 0;

	// ���C�g�̃^�C�v�̐ݒ�

	pLight[0]->Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	pLight[0]->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ����
	pLight[0]->Ambient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&pLight[0]->Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(0, pLight[0]);

	// ���C�g�̐ݒ�
	pDevice->LightEnable(0, TRUE);
	
	
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
	pDevice->SetLight(1, pLight[1]);

	// ���C�g�̐ݒ�
	pDevice->LightEnable(1, TRUE);

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
	pDevice->SetLight(2, pLight[2]);

	// ���C�g�̐ݒ�
	pDevice->LightEnable(2, TRUE);

	// ���C�e�B���O���[�h
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	
	return S_OK;
}

//��������������������������������������
// ���C�g�^�C�v�p�Z�b�^�[
// LightType	: ���s�����A�g�U���Ȃ�
// DiffuseColor	: 
// Ambientcolot : 
// vecDir		: 
// LightID		: ���C�g�̔ԍ�
//��������������������������������������
void Light::SetLight(D3DLIGHTTYPE LightType, D3DXCOLOR DiffuseColor, D3DXCOLOR AmbientColor, D3DXVECTOR3 vecDir,BYTE LightID)
{
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();
	/*
	// ���C�g�̃^�C�v�̐ݒ�
	Light.Type = LightType;

	// �g�U���̎��ʐݒ�
	Light.Diffuse = DiffuseColor;

	// �����̎��ʐݒ�
	Light.Ambient = AmbientColor;

	// ���C�g�̕����x�N�g���̐ݒ�
	vecDir = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light.Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(LightID, &Light);

	// ���C�g�̐ݒ�
	pDevice->LightEnable(LightID, TRUE);
	*/
}

//��������������������������������������
// ���C�g�㏈��
//��������������������������������������
void Light::UninitLight()
{

}

//��������������������������������������
// ���C�g�X�V
//��������������������������������������
void Light::UpdateLight(camera *pcamera)
{
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	D3DXVECTOR3 Poscamera = pcamera->getPos();
	D3DXVECTOR3 Lotcamera = pcamera->getLook();
	D3DXVECTOR3 Vec		  = Lotcamera - Poscamera;
	D3DXVec3Normalize(&Poscamera, &Poscamera);

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
	pDevice->SetLight(0, pLight[0]);

	// ���C�g�̐ݒ�
	pDevice->LightEnable(0, TRUE);
}

//��������������������������������������
// ���C�g�̔ԍ��擾
//��������������������������������������
BYTE* Light::GetLightID()
{
	return &LightID;
}