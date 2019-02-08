//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Shader.cpp
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "shader.h"

// ===== 静的メンバ =====
Microsoft::WRL::ComPtr<ID3DXEffect> Shader::directXEffect = nullptr;

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Shader::Shader()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Shader::~Shader()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シェーダー生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT Shader::make(std::string keyName)
{
	//シェーダーを読み込み
	if (FAILED(D3DXCreateEffectFromFile(DirectX3D::getDevice(), keyName.c_str(), nullptr, nullptr, 0, nullptr, &directXEffect, nullptr)))
	{
		MessageBox(nullptr, "シェーダーファイル読み込み失敗", "", MB_OK);
		return E_FAIL;
	}
	else
	{
		return S_OK;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝
// 取得
//＝＝＝＝＝＝＝＝＝＝＝＝
ID3DXEffect* Shader::get()
{
	return directXEffect.Get();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// レンダ―開始
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Shader::RenderStart()
{
	UINT numPass;
	//----- テクニックを指定 --------------------------------------------
	if (FAILED(directXEffect->SetTechnique("tecMinimum")))
		return;

	//----- レンダリング開始 --------------------------------------------
	if (FAILED(directXEffect->Begin(&numPass, 0)))
		return;

	//---- パスの開始 ---------------------------------------------------
	if (FAILED(directXEffect->BeginPass(0)))
		return;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// レンダー終了
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
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