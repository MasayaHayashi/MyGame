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
	//シェーダーを読み込み
	if (FAILED(D3DXCreateEffectFromFile(DirectX3D::getDevice(), "PostCheck2.fx", nullptr, nullptr, 0, nullptr, &directXEffect, nullptr)))
	{
		MessageBox(nullptr, "シェーダーファイル読み込み失敗", "", MB_OK);
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
	//----- テクニックを指定 --------------------------------------------
	if (FAILED(directXEffect->SetTechnique("main")))
		return;

	//----- レンダリング開始 --------------------------------------------
	if (FAILED(directXEffect->Begin(&numPass, 0)))
		return;

	//---- パスの開始 ---------------------------------------------------
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