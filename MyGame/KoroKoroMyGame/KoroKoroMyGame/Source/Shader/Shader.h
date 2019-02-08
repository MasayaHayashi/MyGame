//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Shader.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef SHADER_H
#define SHADER_H

// ===== インクルード部 =====
#include "../DirectX3D/DirectX3D.h"
#include <wrl.h>

// ===== クラス定義 =====
class Shader final
{
public:
	Shader();
	~Shader();

	static HRESULT  make(std::string keyName);
	static ID3DXEffect* get();
	static void RenderStart();
	static void RenderEnd();

private:
	static Microsoft::WRL::ComPtr<ID3DXEffect> directXEffect;
};

#endif