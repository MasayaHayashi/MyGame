//����������������������������������������
// Shader.cpp
// Author : MasayaHayashi
//����������������������������������������

// ===== �C���N���[�h�� =====
#include "shader.h"

// ===== �ÓI�����o =====
Microsoft::WRL::ComPtr<ID3DXEffect> Shader::directXEffect = nullptr;

//��������������������������������
// �R���X�g���N�^
//��������������������������������
Shader::Shader()
{

}

//��������������������������������
// �f�X�g���N�^
//��������������������������������
Shader::~Shader()
{

}

//����������������������������������������
// �V�F�[�_�[����
//����������������������������������������
HRESULT Shader::make(std::string keyName)
{
	//�V�F�[�_�[��ǂݍ���
	if (FAILED(D3DXCreateEffectFromFile(DirectX3D::getDevice(), keyName.c_str(), nullptr, nullptr, 0, nullptr, &directXEffect, nullptr)))
	{
		MessageBox(nullptr, "�V�F�[�_�[�t�@�C���ǂݍ��ݎ��s", "", MB_OK);
		return E_FAIL;
	}
	else
	{
		return S_OK;
	}
}

//������������������������
// �擾
//������������������������
ID3DXEffect* Shader::get()
{
	return directXEffect.Get();
}

//��������������������������������������
// �����_�\�J�n
//��������������������������������������
void Shader::RenderStart()
{
	UINT numPass;
	//----- �e�N�j�b�N���w�� --------------------------------------------
	if (FAILED(directXEffect->SetTechnique("tecMinimum")))
		return;

	//----- �����_�����O�J�n --------------------------------------------
	if (FAILED(directXEffect->Begin(&numPass, 0)))
		return;

	//---- �p�X�̊J�n ---------------------------------------------------
	if (FAILED(directXEffect->BeginPass(0)))
		return;
}

//��������������������������������������
// �����_�[�I��
//��������������������������������������
void Shader::RenderEnd()
{
	auto hr = directXEffect->EndPass();
	if (FAILED(hr))
	{
		return;
	}

	hr = directXEffect->End();

	if (FAILED(hr))
	{
		return;
	}
}