#include "shader.h"

Microsoft::WRL::ComPtr<ID3DXEffect> Shader::directXEffect = nullptr;

Shader::Shader()
{

}
Shader::~Shader()
{

}

HRESULT Shader::create()
{
	//�V�F�[�_�[��ǂݍ���
	if (FAILED(D3DXCreateEffectFromFile(DirectX3D::getDevice(), "PostCheck2.fx", nullptr, nullptr, 0, nullptr, &directXEffect, nullptr)))
	{
		MessageBox(nullptr, "�V�F�[�_�[�t�@�C���ǂݍ��ݎ��s", "", MB_OK);
		return E_FAIL;
	}
	else
	{
		return S_OK;
	}
}

ID3DXEffect* Shader::get()
{
	return directXEffect.Get();
}

void Shader::RenderStart()
{
	UINT numPass;
	//----- �e�N�j�b�N���w�� --------------------------------------------
	if (FAILED(directXEffect->SetTechnique("main")))
		return;

	//----- �����_�����O�J�n --------------------------------------------
	if (FAILED(directXEffect->Begin(&numPass, 0)))
		return;

	//---- �p�X�̊J�n ---------------------------------------------------
	if (FAILED(directXEffect->BeginPass(0)))
		return;
}
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